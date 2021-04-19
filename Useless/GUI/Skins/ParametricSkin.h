#ifndef __INCLUDED__USELESS_PARAMETRIC_SKIN_H__
#define __INCLUDED__USELESS_PARAMETRIC_SKIN_H__

#include "Useless/GUI/Skins/ArraySkin.h"
#include "Useless/Util/ParmList.h"
#include "Useless/Util/CPT.h"

namespace CPT {

typedef CTC::Rename<int, 100 >   border_type;
typedef CTC::Rename<int, 101 >   border_width;
typedef CTC::Rename<int, 102 >   h_divider_width;
typedef CTC::Rename<int, 103 >   v_divider_width;
typedef CTC::Rename<int, 104 >   h_divider_margin;
typedef CTC::Rename<int, 105 >   v_divider_margin;
typedef CTC::Rename<int, 106 >   cross_h_margin;
typedef CTC::Rename<int, 107 >   cross_v_margin;
typedef CTC::Rename<int, 108 >   border_flags;

typedef CTC::Rename<bool,101 >   use_inner_bevel;
typedef CTC::Rename<bool,102 >   use_outer_bevel;
typedef CTC::Rename<bool,103 >   use_h_dividers;
typedef CTC::Rename<bool,104 >   use_v_dividers;
typedef CTC::Rename<bool,105 >   use_body;


using Useless::NormalPixel;
typedef CTC::Rename<NormalPixel, 101 >   border_color;
typedef CTC::Rename<NormalPixel, 102 >   highlight_color;
typedef CTC::Rename<NormalPixel, 103 >   shadow_color;
typedef CTC::Rename<NormalPixel, 104 >   body_color;
typedef CTC::Rename<NormalPixel, 105 >   h_divider_color;
typedef CTC::Rename<NormalPixel, 106 >   v_divider_color;
typedef CTC::Rename<NormalPixel, 107 >   cycle_row_colors;
typedef CTC::Rename<NormalPixel, 108 >   cycle_col_colors;

};//namespace CPT

namespace Useless {

class USELESS_API ParametricSkin : public ArraySkin
{
public:
    enum BorderType { BORDER_3D, BORDER_2D, BEVEL_IN, BEVEL_OUT, BEVEL_3D=BEVEL_OUT, NO_BORDER };
    enum BorderFlags { LEFT=1, TOP=2, RIGHT=4, BOTTOM=8 };
    template< class PL > ParametricSkin( const PL &pl )
    {
        Set(pl);
    }

    template< class PL > void Set( const PL &pl )
    {
        int _border_type =(int)GetParm( CPT::border_type(), pl, (int)BORDER_3D );
        _border_width    =(int)GetParm( CPT::border_width(), pl, 1 );
        _h_divider_width =(int)GetParm( CPT::h_divider_width(), pl, 1 );
        _v_divider_width =(int)GetParm( CPT::v_divider_width(), pl, 1 );
        _h_divider_margin=(int)GetParm( CPT::h_divider_margin(), pl, 2 );
        _v_divider_margin=(int)GetParm( CPT::v_divider_margin(), pl, 2 );
        _cross_h_margin  =(int)GetParm( CPT::cross_h_margin(), pl, 0 );
        _cross_v_margin  =(int)GetParm( CPT::cross_v_margin(), pl, 0 );
        _use_inner_bevel =(bool)GetParm( CPT::use_inner_bevel(),pl, _border_type==BORDER_3D || _border_type==BEVEL_IN);
        _use_outer_bevel =(bool)GetParm( CPT::use_outer_bevel(),pl, _border_type==BORDER_3D || _border_type==BEVEL_OUT);
        _use_h_dividers  =(bool)GetParm( CPT::use_h_dividers(),pl, true );
        _use_v_dividers  =(bool)GetParm( CPT::use_v_dividers(),pl, true );
        _cycle_row_colors=(bool)GetParm( CPT::cycle_row_colors(), pl, false );
        _cycle_col_colors=(bool)GetParm( CPT::cycle_col_colors(), pl, false );
        _use_body        =(bool)GetParm( CPT::use_body(),pl, true );
        _use_border = (_border_type!=NO_BORDER);
        _border_flags    =(int)GetParm( CPT::border_flags(), pl, (int)(LEFT|RIGHT|TOP|BOTTOM) );

        _border_color    =(NormalPixel)GetParm( CPT::border_color(), pl, (NormalPixel)0xA0A0A0 );
        _body_color      =(NormalPixel)GetParm( CPT::body_color(), pl, (NormalPixel)0xB0B0B0 );
        _highlight_color =(NormalPixel)GetParm( CPT::highlight_color(), pl, (NormalPixel)0xF0F0F0 );
        _shadow_color    =(NormalPixel)GetParm( CPT::shadow_color(), pl, (NormalPixel)0x000000 );
        _body_color      =(NormalPixel)GetParm( CPT::body_color(), pl, (NormalPixel)0xD0D0D0 );
        _h_divider_color =(NormalPixel)GetParm( CPT::h_divider_color(), pl, (NormalPixel)0xC0C0C0 );
        _v_divider_color =(NormalPixel)GetParm( CPT::v_divider_color(), pl, (NormalPixel)0x808080 );
    }

    void ColorColumn( int column, NormalPixel color )
    {
        _column_colors[ column ] = color;
    }

    void ColorRow( int row, NormalPixel color )
    {
        _row_colors[ row ] = color;
    }

    void ColorCell( int column, int row, NormalPixel color )
    {
        _cell_colors[ Pos(column,row) ] = color;
    }

    void SetHDividers(const Useless::SortedVector<int> &d) { _h_d=d; }
    void SetVDividers(const Useless::SortedVector<int> &d) { _v_d=d; }
    bool IsHDividable() const { return true; }
    bool IsVDividable() const { return true; } 
    void PaintBackdrop(const Useless::Painter &p,const Useless::Point<int> &x);
    void PaintGrid(const Useless::Painter &p,const Useless::Point<int> &x);
    int  GetWidth(void) const { return _w; }
    int  GetHeight(void) const { return _h; }
    void SetSize(const Useless::Point<int> &k) { _w=k.x; _h=k.y; }
    Pos  GetDirection() const { return Pos(1,1); }
    bool IsColorKeyed() const { return false; }
    Useless::SPointer<Useless::Skin> CreateSubSkin(int) const 
    {
        return new ParametricSkin( _PL(CPT::body_color(0xC0C0C0), CPT::border_type(NO_BORDER)) ); 
    }
    int GetDimension(int) const;

private:
    int _w,_h,_bg,_fg;
    SortedVector<int> _v_d, _h_d;
    AssocVector<int, NormalPixel> _column_colors;
    AssocVector<int, NormalPixel> _row_colors;
    AssocVector<Pos, NormalPixel> _cell_colors;

    NormalPixel _border_color;
    NormalPixel _highlight_color;
    NormalPixel _shadow_color;
    NormalPixel _body_color;
    NormalPixel _h_divider_color;
    NormalPixel _v_divider_color;

    int _border_width;
    int _h_divider_width;
    int _v_divider_width;
    int _h_divider_margin;
    int _v_divider_margin;
    int _cross_h_margin;
    int _cross_v_margin;
    int _border_flags;

    bool _use_border;
    bool _use_inner_bevel;
    bool _use_outer_bevel;
    bool _use_h_dividers;
    bool _use_v_dividers;
    bool _use_body;
    bool _cycle_row_colors;
    bool _cycle_col_colors;
};
//----------------------------------------------------------------------------------------------


};//namespace Useless
REGISTER_COPIER(Useless,Skin,ParametricSkin);
REGISTER_COPIER(Useless,ArraySkin,ParametricSkin);

#endif//__INCLUDED__USELESS_PARAMETRIC_SKIN_H__
