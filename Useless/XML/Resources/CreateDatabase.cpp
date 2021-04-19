#include "ResourcesPch.h"

#include "Useless/TemplateConfig.h"
#include "Useless/XML/Resources/CreateDatabase.h"
#include "Useless/XML/Resources/CreateText.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/GUI/Misc/DT_Painter.h"
#include "Useless/GUI/Misc/DT_Inspector.h"
#include "Useless/GUI/Misc/FontSelector.h"
#include "Useless/GUI/Widgets/AdvTableSystemWidget.h" //< DF_ShowColumns

namespace Useless {

struct _OrderedBase
{
    typedef std::string Order;
    
    _OrderedBase( Order order ): _order( order ) {}
    Order _order;
};
    
template< class _D >
struct _Ordered : _D, _OrderedBase
{
    USELESS_EMPTY_SPECIALIZATION
        _Ordered( Order order )
            : _OrderedBase( order ) {}

    template< class _T1 >
        _Ordered( Order order, const _T1 &a1 )
            : _OrderedBase( order ), _D( a1 ) {}
    
    template< class _T1, class _T2 >
        _Ordered( Order order, const _T1 &a1, const _T2 &a2 )
            : _OrderedBase( order ), _D( a1, a2 ) {}

    virtual DataObject *CloneData() const
        {
            return (new _Ordered( *this ));
        }
};

struct _OrderBy
{
    _OrderBy( int column, bool ascending )
        : _column( column )
        , _ascending( ascending )
        {}

    bool operator()( const DataStream &d1, const DataStream &d2 )
        {
            if ( d1.size() <= _column )
            {
                if ( d2.size() > _column )
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            if ( d2.size() <= _column )
            {
                return true;
            }
                
            DataStream::const_iterator
                i1 = d1.begin(),
                i2 = d2.begin();

            for ( int c = _column; 0 != c; --c, ++i1, ++i2 )
            {
                assert( i1 != d1.end() );
                assert( i2 != d2.end() );            
            }

            _OrderedBase *o1 = dynamic_cast< _OrderedBase *>(*i1);
            _OrderedBase *o2 = dynamic_cast< _OrderedBase *>(*i2);
            assert( o1 );
            assert( o2 );
            if ( _ascending )
                return (o1->_order < o2->_order);
            else
                return (o1->_order > o2->_order);
        }
    
    int _column;
    bool _ascending;
};

struct _DoTableDataFiltering : Visitor< DF_ShowColumns >
{
    _DoTableDataFiltering( const DataStream &data )
        : _data( data ) {}
    const DataStream &_data;
    DataStream _out;
    
    virtual int Visit( DF_ShowColumns &filter )
    {
        int column=0;
        for ( DataStream::const_iterator it=_data.begin();
                it != _data.end(); ++it, ++column )
        {
            if ( 0 == filter._columnHidden.GetBit( column ) )
            {
                _out.Lock().push_back( (*it)->CloneData() );
            }
        }
        return _out.size();
    }
};

struct _DoDataCompare
    : Visitor< DT_Text >
    , Visitor< DataObject >
{
    _DoDataCompare( const DataStream &data )
        : _data( data )
        , _current(0)
        , _size( data.size() ) {}
    const DataStream &_data;
    int _current;
    int _size;
    
    int operator()( const DataStream &data )
    {
        int matched=0;
        for ( _current=0; _current < _size; ++_current )
        {
            if ( 0 != data.Nolock()[ _current ]->AcceptVisitor( *this ))
                { ++matched; }
        }
        return matched;
    }
    
    virtual int Visit( DT_Text &text )
    {
        DT_Text *p = dynamic_cast< DT_Text *>( _data.Nolock()[ _current ] );
        if ( !p )
            return 1;
        
        return ((Text &)*p == (Text &)text);
    }

    virtual int Visit( DataObject & )
    {
        return 1;
    }
};


class DataTable : public DataConnector
{
public:
    DataTable( const FontSet &fonts )
        : _fonts( fonts )
        , _sortColumn( -1 )
        , _sortDirection( 0 )
    {}

    ~DataTable()
    {
        _header = DataStream();
        _entries.clear();
    }
    
    void Insert( int where, const DataStream &data )
    {
        Entries::iterator it =
            (-1 == where ? _entries.end() : _entries.begin());
        
        for (; it != _entries.end() && where > 0; ++it, --where ) {}
        
        if ( 0 == where || -1 == where )
        {
            _entries.insert( it, data );
        }
    }

    void Remove( int index )
    {
        Entries::iterator it = _entries.begin();
        for (; it != _entries.end(); ++it, --index )
        {
            if ( 0 == index )
            {
                _entries.erase( it ); break;
            }
        }
    }

    void Clear()
    {
        _entries.clear();
        _cache_lastRow = std::make_pair( 0, DataStream());
    }

    void Reset( int where, const DataStream &data )
    {
        Entries::iterator it = _entries.begin();
        for (; it != _entries.end(); ++it, --where )
        {
            if ( 0 == where )
            {
                (*it) = data; break;
            }
        }
    }

    void SetHeader( const DataStream &header )
    {
        _header = header;
    }

    virtual int GetMinWidth( long column ) const
    {
        return 0;
    }

    virtual int GetMinHeight( long row ) const
    {
        return 0;
    }
    
    virtual void SetFilter( SPointer< DataFilter > filter )
    {
        DataConnector::SetFilter( filter );
    }

    DataStream ApplyFilter( const DataStream &data ) const
    {
        if ( !!GetFilter() )
        {
            _DoTableDataFiltering do_filter( data );
            GetFilter()->AcceptVisitor( do_filter );
            return do_filter._out;
        }
        else
        {
            return data;
        }
    }

    virtual DataStream GetHeaders() const
    {
        return ApplyFilter( _header );
    }

    virtual DataStream GetData( int row ) const
    {
        if ( 0 == _cache_lastRow.second.size() ||
                _cache_lastRow.first != row )
        {
            _cache_lastRow.first = row;
            _cache_lastRow.second = DataStream();

            Entries::const_iterator it = _entries.begin();
            for(; it != _entries.end(); ++it, --row )
            {
                if ( 0 == row )
                {
                    _cache_lastRow.second = ApplyFilter( *it );
                    break;
                }
            }
        }
        return _cache_lastRow.second;
    }

    virtual void Paint
        ( long row,
          long col,
          const Rect &cell_space,
          const Painter &painter,
          DisplayState state )
    {
        DataStream data( GetData( row ));
        DataStream::iterator it = data.begin();
        for (; it != data.end(); ++it, --col )
        {
            if ( 0 == col )
            {
                Font font;
                if ( _fonts.Select( state ))
                {
                    int fontID = _fonts.GetFontID();
                    font = _fonts.Get().GetFont( fontID );
                }
                // adjust y offset if necessary, so each entry is centered
                Pos render_offset;
                DT_Inspector dt_inspector( font, font, cell_space );
                (*it)->AcceptVisitor( dt_inspector );
                int content_h = dt_inspector._filled_area.GetH();
                int inner_h = cell_space.GetH();                
                if ( content_h <= cell_space.GetH() )
                {
                    render_offset.y = (cell_space.GetH() - content_h + 1) /2;
                }
                // render entry in table cell space
                DT_Painter dt_painter( painter, font, font, cell_space + render_offset );
                (*it)->AcceptVisitor( dt_painter );
            }
        }
    }

    virtual void SortData( int column, bool ascending = true )
    {
        _sortColumn = column;
        _sortDirection = ( ascending ? 1 : -1 );
        std::stable_sort( _entries.begin(), _entries.end()
                , _OrderBy( column, ascending ));
        OnDataChange.Send_4( 0, 0, _header.size(), _entries.size() );
    }

    std::pair< int, int > GetSortInfo() const
    {
        return std::make_pair( _sortColumn, _sortDirection );
    }

    virtual int FindNext( const DataStream &data, int from=-1 ) const
    {
        _DoDataCompare docmp( data );
        Entries::const_iterator it=_entries.begin();
        for (int i=0; it != _entries.end(); ++it, ++i )
        {
            if ( from <= i )
            {
                if ( docmp( *it ) == data.size() )
                {
                    return i;
                }
            }
        }
        return -1;
    }

    void SetFontSet( const FontSet &fonts ) { _fonts = fonts; }
    const FontSet & GetFontSet() const { return _fonts.Get(); }
            
    
private:
    typedef std::list< DataStream > Entries;
    Entries     _entries;
    DataStream  _header;
    FontSelector _fonts;
    int          _sortColumn;
    int          _sortDirection;
    
    mutable std::pair< int, DataStream > _cache_lastRow;
};


DT_Text *       _CreateDT_Text      ( XMLIterator i );
DT_Image *      _CreateDT_Image     ( XMLIterator i );
DT_SingleRow *  _CreateDT_SingleRow ( XMLIterator i );

DT_Text *   _CreateDT_Text( XMLIterator i )
{
    int place = 0;
    try_attribute_i( (*i)._attributes, "place", &place );
    std::string order = make_string( (*i)._value );
    try_attribute( (*i)._attributes, "order", &order );
    return new _Ordered< DT_Text >( order, MakeText( (*i)._value ), place);
}

DT_Image *  _CreateDT_Image( XMLIterator i )
{
    int place = 0;
    try_attribute_i( (*i)._attributes, "place", &place );
    std::string order = "";
    try_attribute( (*i)._attributes, "order", &order );
    
    return new _Ordered< DT_Image >( order
            , dynamic_cast< Image &>(
            *MakeImage(
                make_string( (*i)._value )
                ))
            , place );
}

DT_SingleRow *  _CreateDT_SingleRow( XMLIterator i )
{
    std::string order = "";
    try_attribute( (*i)._attributes, "order", &order );
    DT_SingleRow *data = new _Ordered< DT_SingleRow >( order );
    
    try {
        for (; !!i; ++i)
        {
            int size = 1;
            try_attribute_i( (*i)._attributes, "place", &size );

            std::string name = (*i)._name;
            if ( "text" == name )
            {
                data->Add( _CreateDT_Text( i ), size );
            }
            else if ( "image" == name )
            {
                data->Add( _CreateDT_Image( i ), size );
            }
            else if ( "hbox" == name )
            {
                data->Add( _CreateDT_SingleRow(
                            XMLIterator( i ).StepInto() ), size );
            }
            else
            {
                throw Error("");
            }
        }
    }
    catch(...)
    {
        delete data;
        throw;
    }
    return data;
}

DataStream _CreateDataStream( XMLIterator i )
{
    DT_SingleRow *r = _CreateDT_SingleRow( i );
    DataStream s( r->_stream );
    delete r;
    return s;
}


void _DoCreateDbTable( XMLIterator i, XMLCreatorEnv *env, DataTable *dataTable )
{
    const XMLAttributes &attr = (*i)._attributes;
    std::string s;

    if ( try_attribute( attr, "fontset", &s ))
    {
        dataTable->SetFontSet( QueryFontSet( s ));
    }

    i.StepInto();
    for (; !!i; ++i )
    {
        if ( (*i)._name == "head" )
        {
            dataTable->SetHeader(
                    _CreateDataStream(
                        XMLIterator( i ).StepInto()
                        ));
        }
        else if ( (*i)._name == "body" )
        {
            XMLIterator j(i); j.StepInto();
            for (; !!j; ++j )
            {
                if ( (*j)._name == "row" )
                {
                    dataTable->Insert( -1,
                            _CreateDataStream(
                                XMLIterator( j ).StepInto()
                                ));
                }
            } 
        }
        else
        {
            throw Error("CreateDbTable: Unsupported tag %s", (*i)._name.c_str() );
        }
    }
}
    

Resource* CreateDbTable( XMLIterator i, XMLCreatorEnv *env )
{
    DataTable *dataTable = new DataTable( FontSet() );
    Share< DataConnector > shd( dataTable );
    _DoCreateDbTable( i, env, dataTable );
    return new AnyResource< Share< DataConnector > >( shd );
}


Resource* UpdateDbTable( XMLIterator i, XMLCreatorEnv *env )
{
    std::string tableName = get_attribute( (*i)._attributes, "table" );
    Resource *r = Resources::Instance().QueryResource( "database", tableName );
    Share< DataConnector > *shd = r->DynaCast< Share< DataConnector > >();
    if ( !r || !shd )
    {
        throw Error("UpdateDbTable: table not found %s", (*i)._name.c_str() );
    }
    DataTable *dataTable = dynamic_cast< DataTable *>( shd->GetTarget() );
    std::pair< int, int > sortInfo = dataTable->GetSortInfo();
    dataTable->Clear();
    
    _DoCreateDbTable( i, env, dataTable );
    
    if ( 0 != sortInfo.second )
    {
        dataTable->SortData( sortInfo.first, 1 == sortInfo.second );
    }

    return NULL;
}

}//Useless
