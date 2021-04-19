#include "UselessPch.h"

#include "Useless/GUI/Skins/Tiler.h"

namespace Useless {

Rect Tiler::RectFill1D( const Rect &src, const Rect &area, int align, int shift)
{
    Rect c_area = (!! _clip_rect) ? (area & _clip_rect) : area;

    Pos   d = D( c_area.GetPos() - area.GetPos() );
    Pos   i;
    Pos   a = c_area.GetP1();
    Pos   b = c_area.GetP2();
    Dim2i s = Dim2i(   src.GetW(),   src.GetH() );
    Pos psh = T(P(shift));

    for ( ; X(i)<X(b)-X(a); i+=D(s) )
    {
        Rect destrc = ( src  + (A(area*2,align)-A(src*2,align) )/2 + (i+d) +psh) & area;
        Rect srcrc =  ( area + (A(src*2,align)-A(area*2,align) )/2 - (i+d) -psh) & src;
        if (!!srcrc)
        {
            _rect_list.push_back( srcrc );
            _positions.push_back( destrc.GetPos() +_pos );
        }
    }
    return area;
}

Rect Tiler::RectFill2D( const Rect &src, const Rect &area, int shift)
{
    Rect c_area = (!! _clip_rect) ? (area & _clip_rect) : area;

    Pos   i = c_area.GetPos() - area.GetPos();
    Pos   a = c_area.GetP1();
    Pos   b = c_area.GetP2();
    Dim2i s = Dim2i(   src.GetW(),   src.GetH() );
    Pos psh = T(P(shift));

    for ( i.y=a.y; i.y<b.y; i.y+=s.y )
    {
        for ( i.x=a.x; i.x<b.x; i.x+=s.x )
        {
            Rect destrc = (src-src.GetPos()+i) & area;
            if (!!destrc)
            {
                _rect_list.push_back(destrc-i + src.GetPos());
                _positions.push_back(i + _pos + psh );
            }
        }
    }
    return area;
}

Rect Tiler::RectCut( const Rect &src, const Rect &area, int align, int ortho, int parallel )
{

    Pos psh = T(P(ortho))+P(parallel);
    Rect destrc = ( src  + (A(area*2,align)-A(src*2,align) )/2 +psh) & area;

    if ( !_clip_rect || (destrc && _clip_rect) )
    {
        Rect srcrc =  ( area + (A(src*2,align)-A(area*2,align) )/2 -psh) & src;
        if (!!srcrc)
        {
            _rect_list.push_back( srcrc );
            _positions.push_back( (destrc).GetPos() +_pos );
        }
    }
    return destrc;
}

};//namespace Useless
