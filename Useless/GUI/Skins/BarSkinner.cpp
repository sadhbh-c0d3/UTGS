#include "UselessPch.h"

#include "Useless/GUI/Skins/BarSkinner.h"
#include "Useless/GUI/Skins/Tiler.h"

namespace Useless {

BarSkinner::BarSkinner( const RefImage& image, Orientation orient, int x1, int x2, int x3 )
    : ImageDicer(image)
{
    BarDice( image.get().GetRect(),orient,x1,x2,x3 );
}

BarSkinner::BarSkinner(const RefImage& image, const Rect& boundry, Orientation orient, int x1, int x2, int x3 )
    : ImageDicer(image)
{
    BarDice( boundry,orient,x1,x2,x3 );
}

void BarSkinner::BarDice(const Rect& boundry, Orientation orient, int x1, int x2, int x3 )
{
    AssocVector<int ,Rect> rect_list;
    Pos p0 = boundry.GetPos();

    if (orient==HORIZONTAL)
    {
        int x[5] = { 0, x1, x2, x3, boundry.GetW() };
        int y[2] = { 0, boundry.GetH() };
        rect_list.Insert(HEAD   , CheckedRect( x[0], y[0], x[1]-x[0], y[1]-y[0] ) + p0);
        rect_list.Insert(BODY   , CheckedRect( x[1], y[0], x[2]-x[1], y[1]-y[0] ) + p0);
        rect_list.Insert(HANDLE , CheckedRect( x[2], y[0], x[3]-x[2], y[1]-y[0] ) + p0);
        rect_list.Insert(TAIL   , CheckedRect( x[3], y[0], x[4]-x[3], y[1]-y[0] ) + p0);
    }
    else if (orient==VERTICAL)
    {
        int x[2] = { 0, boundry.GetW() };
        int y[5] = { 0, x1, x2, x3, boundry.GetH() };
        rect_list.Insert(HEAD   , CheckedRect( x[0], y[0], x[1]-x[0], y[1]-y[0] ) + p0);
        rect_list.Insert(BODY   , CheckedRect( x[0], y[1], x[1]-x[0], y[2]-y[1] ) + p0);
        rect_list.Insert(HANDLE , CheckedRect( x[0], y[2], x[1]-x[0], y[3]-y[2] ) + p0);
        rect_list.Insert(TAIL   , CheckedRect( x[0], y[3], x[1]-x[0], y[4]-y[3] ) + p0);
    }
    else
        throw Error("BarSkinner::BarDice(): Orientation can only be HORIZONTAL or VERTICAL");

    ImageDicer::Dice(rect_list);
    _orient = orient;
    SetSize( Dim2i( GetImage(HEAD).GetWidth(), GetImage(HEAD).GetHeight() ) );
}

void BarSkinner::Paint( const Painter& painter, const Pos& p )
{
    if ( Exists(SOURCE) )
    {
        RectList    rects;
        PointList   points;
        Think( points, rects, p );
        painter.ProjectImages( GetImage(SOURCE), points, rects );
    }
}


void BarSkinner::Think( PointList& points, RectList& rects, const Pos& p )
{
    Tiler tiler( points, rects, p);
    Rect rHEAD = R(0, _size/2 );
    Rect rTAIL = R( X( rHEAD.GetW(), rHEAD.GetH() ), X(_size-rHEAD.GetW(), _size-rHEAD.GetH() ) );
    Rect rCENTER;

    if ( Exists(HEAD) )
    {
        IGraphics &g = ImageDicer::GetImage(HEAD);
        rHEAD = tiler.RectCut( g.GetRect(), rHEAD, Tiler::HALIGNLEFT|Tiler::VALIGNTOP);
    }
    else
        rHEAD = R( X( rHEAD.GetX1(), rHEAD.GetY1() ), 0 );

    if ( Exists(TAIL) )
    {
        IGraphics &g = ImageDicer::GetImage(TAIL);
        rTAIL = tiler.RectCut( g.GetRect(), rTAIL, Tiler::HALIGNRIGHT|Tiler::VALIGNBOTTOM);
    }
    else
        rTAIL = R( X( rTAIL.GetX2(), rTAIL.GetY1() ) , 0 );

    int x1 = X( rHEAD.GetX2(), rHEAD.GetY2() );
    int x2 = X( rTAIL.GetX1(), rTAIL.GetY1() );
    rCENTER = R( x1, x2-x1 );

    tiler.Orient(_orient);
    if ( Exists(BODY) )
    {
        IGraphics &g = ImageDicer::GetImage(BODY);
        tiler.RectFill1D( g.GetRect(), rCENTER, Tiler::VALIGNTOP);
    }

    if ( Exists(HANDLE) )
    {
        IGraphics& g = GetImage(HANDLE);
        tiler.RectCut( g.GetRect(), rCENTER+P(1), Tiler::HALIGNCENTER|Tiler::VALIGNCENTER);
    }
}

void BarSkinner::GetMargins( int &frame_left, int &frame_top, int &frame_right, int &frame_bottom )
{
    frame_left   = _orient==HORIZONTAL && Exists(HEAD) ? GetImage(HEAD).GetWidth()  : 0;
    frame_right  = _orient==HORIZONTAL && Exists(TAIL) ? GetImage(TAIL).GetWidth()  : 0;
    frame_top    = _orient==VERTICAL   && Exists(HEAD) ? GetImage(HEAD).GetHeight() : 0;
    frame_bottom = _orient==VERTICAL   && Exists(TAIL) ? GetImage(TAIL).GetHeight() : 0;
}

int BarSkinner::GetDimension( int dimID ) const
{
    switch( dimID )
    {
    case LEFT_MARGIN:   return (_orient==HORIZONTAL && Exists(HEAD) ? GetImage(HEAD).GetWidth()  : 0);
    case TOP_MARGIN:    return (_orient==VERTICAL   && Exists(HEAD) ? GetImage(HEAD).GetHeight() : 0);
    case RIGHT_MARGIN:  return (_orient==HORIZONTAL && Exists(TAIL) ? GetImage(TAIL).GetWidth()  : 0);
    case BOTTOM_MARGIN: return (_orient==VERTICAL   && Exists(TAIL) ? GetImage(TAIL).GetHeight() : 0);
    default: return 0;
    }
}


};//namespace Useless
