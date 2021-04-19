#include "UselessPch.h"

#include "Useless/GUI/Widgets/WidgetTableWidget.h"
#include "Useless/GUI/WidgetEnvironment.h"

namespace Useless {

    WidgetTableWidget::WidgetTableWidget(  const ArraySkin &skin  ): HubWidget(0,0), TableWidget(skin)
    {
    }

    WidgetTableWidget::WidgetTableWidget( const TableStyle &style ): HubWidget(0,0), TableWidget(style)
    {
    }

    WidgetTableWidget::~WidgetTableWidget()
    {
    }


    bool WidgetTableWidget::Insert( int column, int row, Widget *client, int align_flg )
    {
        Rect::Alignment align = Rect::Alignment(align_flg);
        if ( column < GetNumColumns() && row < GetNumRows() && !Settled( column, row) )
        {
            Rect area = client->GetBoundingRect().Align( ClientRect( column, row), align ) ;
            Cell cell;
            cell._alignment = align;
            cell._settler = client;
            _settlers.Insert( Pos(column,row), cell);
            _placement.Insert( client, Pos(column,row) );
            HubWidget::Insert( client, area.GetX(), area.GetY() );
            return true;
        }
        else { assert(!"No such column or row"); return false; }
    }

    Widget* WidgetTableWidget::Retrieve( int column, int row)
    {
        SettlerMap::ConstIterator iter;
        Cell *pcell = _settlers.Find( Pos(column,row), &iter );
        if (pcell)
        {
            Widget *client = pcell->_settler;
            HubWidget::Retrieve( client);
            _settlers.RemoveAt( iter);
            _placement.Remove( client);
            return client;
        }
        else { assert(!"No such column or row"); return NULL; }
    }

    bool WidgetTableWidget::Remove ( int column, int row)
    {
        Widget *client = WidgetTableWidget::Retrieve( column, row);
        if (client)
        {
            if ( const WidgetEnvironment *penv = GetEnvironment() ) { penv->GetTrashCan().Trash(client); }
            else { delete client; }
            return true;
        }
        return false;
    }


    bool WidgetTableWidget::Retrieve( Widget *client)
    {
        Pos *ppos = _placement.Find( client);
        if (ppos)
        {
            return WidgetTableWidget::Retrieve( ppos->x, ppos->y);
        }
        return false;
    }

    bool WidgetTableWidget::Remove ( Widget *client)
    {
        Pos *ppos = _placement.Find( client);
        if (ppos)
        {
            return WidgetTableWidget::Remove( ppos->x, ppos->y);
        }
        return false;
    }

    bool WidgetTableWidget::Settled( int column, int row) const
    {
        return !!_settlers.Find( Pos( column, row) );
    }

    Widget* WidgetTableWidget::Settler( int column, int row) const
    {
        Cell *pcell = _settlers.Find( Pos( column, row) );
        return pcell ? pcell->_settler : NULL;
    }

    void WidgetTableWidget::Resize( int w, int h)
    {
        TableWidget::Resize( w, h );
        //HubWidget::Resize( GetWidth(), GetHeight() ); @NoGetWidth
    }


    Rect WidgetTableWidget::GetClientRect(const Widget *sender) const
    {
        if ( HubWidget::IsChild(sender) )
        {
            const Pos *ppos = _placement.Find( (Widget*)sender);
            assert(ppos && "TableWidget::GetClientRect(): Integrity error");
            return ClientRect( ppos->x, ppos->y);
        }
        else return Rect(0,0,GetWidth(),GetHeight());
    }


    void WidgetTableWidget::UpdateTable()
    {
        TableWidget::UpdateTable();
        UpdateTableImpl();
    }

    void WidgetTableWidget::UpdateTableImpl()
    {
        SettlerMap::Iterator iter = _settlers.Begin();
        for (; !!iter; ++iter)
        {
            Rect clientrect = ClientRect( iter.Key().x, iter.Key().y );
            Rect boundrect  = iter.Value()._settler->GetBoundingRect();
            Rect area = boundrect.Align( clientrect, iter.Value()._alignment ) ;
            Widget *p_child = iter.Value()._settler;
            DirtyLock guard_dirty(p_child);
            p_child->SetPosition( Pos( area.GetX(), area.GetY() ) );
        }
        SetDimensions( GetWidth(), GetHeight() );
    }

    void WidgetTableWidget::Repaint( const WidgetPainter &painter )
    {
        TableWidget::Repaint(painter);
        RepaintImpl(painter);
    }

    void WidgetTableWidget::RepaintImpl( const WidgetPainter &painter )
    {
        //HubWidget::Repaint(painter); -- nonsense

        Rect client_area = GetParent()->GetClientRect(this)-GetPosition();
        Rect influenced = GetCrossing( client_area );
        Pos p( influenced.GetX1(), influenced.GetY1() );

        for ( int j=0; j!=influenced.GetW(); ++j, ++p.x )
        {
            SettlerMap::Iterator iter;
            _settlers.Find( p, (SettlerMap::ConstIterator*)&iter);
            for ( int i=0; i!=influenced.GetH(); ++i, ++iter )
            {
                if (!iter) { break; }//< W/A GetCrossing() does not work corectly
                Widget &child = *iter->_settler;
                if ( child.IsVisible() )
                {
                    WidgetPainter child_painter( painter, child );
                    if ( child_painter.Intersect() ) 
                    {
                        child.Repaint(child_painter);
                        child_painter.RestoreClipper();
                    }
                }
            }
        }
    }

    void WidgetTableWidget::RemoveColumns( int column, int num_columns )
    {
        int columns = GetNumColumns();
        {
            DirtyLock guard_dirty(this);
            {
                for( int j=column; j < column+num_columns; ++j )
                {
                    for( int i=0; i < GetNumRows(); ++i )
                    {
                        if( Settled( j,i ) ) Retrieve( j,i );
                    }
                }
            }
            {
                for( int j=column; j < columns - num_columns; ++j )
                {
                    for( int i=0; i < GetNumRows(); ++i )
                    {
                        if( Settled( j+num_columns, i ) )
                        {
                            Cell *pcell = _settlers.Find( Pos( j+num_columns, i ) );
                            Widget *client = pcell->_settler;
                            Rect::Alignment align = pcell->_alignment;
                            Retrieve( pcell->_settler );
                            Insert( j,i,client,align );
                        }
                    }
                }
            }
        }
        TableWidget::RemoveColumns( column,num_columns );	
    }


};//namespace Useless
