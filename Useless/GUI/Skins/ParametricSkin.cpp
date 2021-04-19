#include "UselessPch.h"

#include "ParametricSkin.h"

namespace Useless {

void ParametricSkin::PaintBackdrop(const Useless::Painter &p,const Useless::Point<int> &x)
{
    if ( !_w || !_h ) { return; }
    
    Rect cr = GetClipRect(); if (!cr) { cr=Rect(x.x,x.y,_w,_h); }

    if ( _use_body )
    {   p.PaintRectangle( _body_color, Rect(0,0,_w,_h)+x );
    }

    AssocVector<int, NormalPixel>::Iterator it = _row_colors.Begin();
    for (; !!it; ++it )
    {
        int row_no = it.Key();
        if ( row_no>=0 && row_no<=_h_d.Size() )
        {
            int Y0 = (row_no>0) ? _h_d[row_no-1] : 0;
            int Y1 = (row_no<_h_d.Size()) ? _h_d[row_no] : _h;
            p.PaintRectangle( it.Value(), cr&Rect( 0, Y0, _w, Y1-Y0 )+x );
        }
    }

    it = _column_colors.Begin();
    for (; !!it; ++it )
    {
        int col_no = it.Key();
        if ( col_no>=0 && col_no<=_v_d.Size() )
        {
            int X0 = (col_no>0) ? _v_d[col_no-1] : 0;
            int X1 = (col_no<_v_d.Size()) ? _v_d[col_no] : _w;
            p.PaintRectangle( it.Value(), cr&Rect( X0, 0, X1-X0, _h )+x );
        }
    }
}

void ParametricSkin::PaintGrid(const Useless::Painter &p,const Useless::Point<int> &x)
{
    if ( !_w || !_h ) { return; }

    Rect cr = GetClipRect(); if (!cr) { cr=Rect(x.x,x.y,_w,_h); }
    Pos P1,P2,P3,Q1,Q2,Q3;
    Pos P0 = Pos(0,0);
    Pos Q0 = Pos(_w,_h);

    if (_use_border)
    {
        P1 = (_use_outer_bevel)? Pos(1,1) : Pos(0,0);
        P2 = Pos(_border_width,_border_width) + P1;
        P3 = P2 + ((_use_inner_bevel)? Pos(1,1) : Pos(0,0));
        Q1 = Q0 - P1;
        Q2 = Q0 - P2;
        Q3 = Q0 - P3;

        if ( _use_outer_bevel )
        {
            if ( _border_flags&TOP )  p.PaintRectangle( _highlight_color, cr&Rect( P0.x, P0.y, Q0.x-P0.x, 1 )+x );
            if ( _border_flags&LEFT ) p.PaintRectangle( _highlight_color, cr&Rect( P0.x, P0.y, 1, Q0.y-P0.y )+x );
            if ( _border_flags&BOTTOM ) p.PaintRectangle( _shadow_color, cr&Rect( P1.x, Q1.y, Q0.x-P1.x, 1 )+x );
            if ( _border_flags&RIGHT ) p.PaintRectangle( _shadow_color, cr&Rect( Q1.x, P1.y, 1, Q0.y-P1.y )+x );
        }

        if ( _use_inner_bevel )
        {
            if ( _border_flags&TOP ) p.PaintRectangle( _shadow_color, cr&Rect( P2.x, P2.y, Q2.x-P2.x, 1 )+x );
            if ( _border_flags&LEFT )p.PaintRectangle( _shadow_color, cr&Rect( P2.x, P2.y, 1, Q2.y-P2.y )+x );
            if ( _border_flags&BOTTOM ) p.PaintRectangle( _highlight_color, cr&Rect( P3.x, Q3.y, Q2.x-P3.x, 1 )+x );
            if ( _border_flags&RIGHT )  p.PaintRectangle( _highlight_color, cr&Rect( Q3.x, P3.y, 1, Q2.y-P3.y )+x );
        }

        if ( !_use_outer_bevel || _use_inner_bevel )
        {
            if ( _border_flags&TOP )   p.PaintRectangle( _border_color, cr&Rect( P1.x, P1.y, Q1.x-P1.x, _border_width )+x );
            if ( _border_flags&LEFT )  p.PaintRectangle( _border_color, cr&Rect( P1.x, P1.y, _border_width, Q1.y-P1.y )+x );
            if ( _border_flags&BOTTOM )p.PaintRectangle( _border_color, cr&Rect( P1.x, Q2.y, Q1.x-P1.x, _border_width )+x );
            if ( _border_flags&RIGHT ) p.PaintRectangle( _border_color, cr&Rect( Q2.x, P1.y, _border_width, Q1.y-P1.y )+x );
        }
    }
    Pos P4 = P3 + Pos(_h_divider_margin,_v_divider_margin);
    Pos Q4 = Q0 - P4;

    if ( _use_h_dividers )
    for ( int i=0; i!=_h_d.Size(); ++i )
    {
        p.PaintRectangle( _h_divider_color, cr&Rect(P4.x,_h_d[i]-_h_divider_width/2,Q4.x-P4.x,_h_divider_width)+x );
    }

    if ( _use_v_dividers )
    for ( int j=0; j!=_v_d.Size(); ++j )
    {
        p.PaintRectangle( _v_divider_color, cr&Rect(_v_d[j]-_v_divider_width/2,P4.y,_v_divider_width,Q4.y-P4.y)+x );
    }
}

int ParametricSkin::GetDimension( int dimID ) const
{
    if ( dimID==LEFT_MARGIN && _border_flags&LEFT ||
         dimID==RIGHT_MARGIN && _border_flags&RIGHT ||
         dimID==TOP_MARGIN && _border_flags&TOP ||
         dimID==BOTTOM_MARGIN && _border_flags&BOTTOM )
    {
        if ( !_use_border ) { return 0; }
        else { return _border_width + (int)_use_inner_bevel + (int)_use_outer_bevel; }
    }
    else if ( dimID==VERTI_DIV ) { return  _v_divider_width; }
    else if ( dimID==HORIZ_DIV ) { return  _h_divider_width; }
    else { return 0; }
}


};//namespace Useless
