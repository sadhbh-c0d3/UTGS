#ifndef __INCLUDED_USELESS_DATA_TYPES_H__
#define __INCLUDED_USELESS_DATA_TYPES_H__

#include "Useless/System/Assert.h"
#include "Useless/GUI/Misc/DataStream.h"
#include "Useless/GUI/Text/FontPainter.h"
#include "Useless/Graphic/Planes/Image.h"
#include "Useless/Util/Storage/cell_division_vector.h"


namespace Useless {

/*! \defgroup DataTypes DataTypes
 *  \ingroup DataTypes
 *  Note:
 *  placement attribute is preffered view alignment
 *  in percentages of area width
 */

/*! Visitable None data type */
struct USELESS_API DT_None : public Useless::DataObject
{
    DataObject *CloneData() const { return new DT_None(); }
};

/*! Visitable Image data type */
struct USELESS_API DT_Image : public DataObject, public Image
{
    DEFINE_CYCLIC_VISITABLE(DataObject);
    DT_Image( const Image &i, int placement=50 ): Image(i), _placement(placement) {}
    DataObject *CloneData() const { return new DT_Image( *this ); }
    int _placement;
};

/*! Visitable Text data type */
struct USELESS_API DT_Text : public DataObject, public Text
{
    DEFINE_CYCLIC_VISITABLE(DataObject);
    DT_Text( const Text &t, int placement=0 ): Text(t), _placement(placement) {}
    DataObject *CloneData() const { return new DT_Text( *this ); }
    int _placement;
};

/*! Visitable association name => value */
struct USELESS_API DT_PayEntry : public DataObject
{
    DEFINE_CYCLIC_VISITABLE(DataObject);

    DT_PayEntry( const Text &name,
                 const Text &value,
                 int         name_placement=50,
                 int         value_placement=50 )
        :_name(name), _value(value),
         _name_placement(name_placement),
         _value_placement(value_placement)
    {}
    
    DataObject *CloneData() const { return new DT_PayEntry( *this ); }

    Text _name;
    Text _value;
    int  _name_placement;
    int  _value_placement;
};

/*! Visitable association name => (value[0], value[1], ...) */
struct USELESS_API DT_PayEntries : public DataObject
{
    DEFINE_CYCLIC_VISITABLE(DataObject);

    DT_PayEntries( const Text &name, int columns=1, int name_placement=50, int value_placement=0 )
        :_name(name), _columns(columns),
         _name_placement(name_placement),
         _value_placement(value_placement)
    {}
    
    DataObject *CloneData() const { return new DT_PayEntries( *this ); }

    void Add( const Text &value ) { _values.push_back(value); }

    Text _name;
    int  _columns;
    int  _name_placement;
    int  _value_placement;
    std::vector<Text> _values;
};

/*! Visitable formated data stream */
struct USELESS_API DT_SingleRow : public DataObject
{
    DEFINE_CYCLIC_VISITABLE(DataObject);

    DT_SingleRow() {}
    
    DataObject *CloneData() const { return new DT_SingleRow( *this ); }

    void Add( DataObject *p_obj, int size )
    {
        _stream.Lock().push_back(p_obj);
        _cells.push_back(size);
    }

    DataStream           _stream;
    cell_division_vector _cells;
};


/*! DataStream constructor */
struct USELESS_API DataStreamer : Useless::DataStream
{
    DataStreamer()
    {}

    //parse string of form "|value1|value2|..."
    DataStreamer( const std::string &s, char sep='|' )
    {
        int p=0, q=0;
        do {
            q = s.find( sep, p );
            this->Text( s.substr( p, q-p ));
            p = q + 1;
        } while ( -1 != q );
    }

    DataStreamer & Image( const Useless::Image &img, int placement=50 )
    {
        return Add( new DT_Image( img, placement ));
    }

    DataStreamer & Text( const Useless::Text &text, int placement=50 )
    {
        return Add( new DT_Text( text, placement ));
    }

    DataStreamer & None()
    {
        return Add( new DT_None() );
    }

    DataStreamer & Add( DataObject *ptr )
    {
        Lock().push_back( ptr );
        return (*this);
    }
};


};//namespace Useless
#endif//__INCLUDED_USELESS_DATA_TYPES_H__
