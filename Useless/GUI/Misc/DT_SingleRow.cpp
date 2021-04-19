#include "UselessPch.h"

#pragma warning(disable:4786) // debug name turncation
#include "Useless/GUI/Misc/DT_Painter.h"
#include "Useless/GUI/Misc/DT_Inspector.h"

namespace Useless {

int DT_Inspector::Visit( DT_SingleRow &row )
{
    DataStream::Iterator it=row._stream.begin(), iend=row._stream.end();
    row._cells.calc_positions( _area.GetW() );
    int last = row._cells.size()-1;
    _filled_area = Rect();

    for ( int is=0; it!=iend; ++it, ++is )
    {
        Rect ch_area = _area;
        ch_area.SetX( _area.GetX() + row._cells.at(is).position );
        ch_area.SetW( row._cells.get_size(is) );

        DT_Inspector dt_inspector(_header_font, _normal_font, ch_area );
        (*it)->AcceptVisitor(dt_inspector);
        _filled_area |= dt_inspector._filled_area;
    }

    return _filled_area.GetH();
}


int DT_Painter::Visit( DT_SingleRow &row )
{
    DataStream::Iterator it=row._stream.begin(), iend=row._stream.end();
    row._cells.calc_positions( _area.GetW() );
    int last = row._cells.size()-1;
    _filled_area = Rect();

    for ( int is=0; it!=iend; ++it, ++is )
    {
        Rect ch_area = _area;
        ch_area.SetX( _area.GetX() + row._cells.at(is).position );
        ch_area.SetW( row._cells.get_size(is) );

        // center vertically
        DT_Inspector dt_inspector(_header_font, _normal_font, ch_area );
        (*it)->AcceptVisitor(dt_inspector);
        ch_area.SetY( ch_area.GetY() + (ch_area.GetH() - dt_inspector._filled_area.GetH())/2 );

        // paint data object
        DT_Painter dt_painter( _painter, _header_font, _normal_font, ch_area );
        (*it)->AcceptVisitor(dt_painter);
        _filled_area |= dt_painter._filled_area;
    }
    return _filled_area.GetH();
}


};//namespace Useless
