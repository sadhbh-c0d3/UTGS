#ifndef __INCLUDED__USELESS_PARM_TYPES_H__
#define __INCLUDED__USELESS_PARM_TYPES_H__

#include "Useless/Util/CTC.h"

#include "Useless/Util/CopyingFactory.h"

/* Used to rename types, to make their names unique */
//@{
namespace CTC
{
template< typename T, int I=0 >
struct Rename
{
    Rename(): x() {}
    Rename( const T &x ): x(x) {}
    operator const T& () const { return x; }
    operator       T& ()       { return x; }
    const T& operator()() const { return x; }
          T& operator()()       { return x; }
    T x;
};

template< typename T, int I=0 >
struct RenameConstReference
{
    RenameConstReference(): px(0) {}
    RenameConstReference( const T *x ): px(x) {}
    RenameConstReference( const T &x ): px(&x) {}
    operator const T* () const { return  px; }
    operator const T& () const { return *px; }
    const T& operator()() const { return *px; }
    const T *px;
};

template< typename T, int I=0 >
struct RenameReference
{
    RenameReference(): px(0) {}
    RenameReference( T *x ): px(x) {}
    RenameReference( T &x ): px(&x) {}
    operator const T* () const { return  px; }
    operator const T& () const { return *px; }
    operator       T* ()       { return  px; }
    operator       T& ()       { return *px; }
    const T& operator()() const { return *px; }
          T& operator()()       { return *px; }
    T *px;
};

template< typename T, int I=0 >
struct RenameCopy
{
    RenameCopy() {}
    RenameCopy(const T &x): spx( &Useless::CopyOf<T>(x) ) {}
    RenameCopy(Useless::SPointer<T> spx): spx(spx)  {}

    const T& operator()() const { return *spx; }
          T& operator()()       { return *spx; }

    operator Useless::SPointer<T>() const { return &Useless::CopyOf<T>(*spx); }
    operator T&() { return *spx; }
    operator const T&() const { return *spx; }

    Useless::SPointer<T> spx;
};

};//namespace CPT
//@}};

/* Useless Parameter Types
 */
namespace CPT 
{
    static  struct Parms {} use_parms;

    typedef CTC::Rename<int,0> px;     // position x
    typedef CTC::Rename<int,1> py;     // position y
    typedef CTC::Rename<int,2> pz;     // position z or just 1D position
    typedef CTC::Rename<int,3> sx;     // size x
    typedef CTC::Rename<int,4> sy;     // size y
    typedef CTC::Rename<int,5> sz;     // size z or just 1D size
    typedef CTC::Rename<int,6> spc_l;  // left space size
    typedef CTC::Rename<int,7> spc_t;  // top space size
    typedef CTC::Rename<int,8> spc_r;  // right space size
    typedef CTC::Rename<int,9> spc_b;  // bottom space size
    typedef CTC::Rename<int,10> spc_h;  // vertical space size
    typedef CTC::Rename<int,11> spc_v;  // horizontal space size
    typedef CTC::Rename<int,12> align;     // alignment
    typedef CTC::Rename<int,13> dist;      // distance
    typedef CTC::Rename<int,14> shift;     // shift
    typedef CTC::Rename<int,15> range;     // range


    typedef CTC::Rename<bool,0> alw_sel;     // always selected
    typedef CTC::Rename<bool,1> sel_sel;     // selector selects
    typedef CTC::Rename<bool,2> full_follow; // selector follows hilite
    typedef CTC::Rename<bool,3> drag_follow; // selector follows dragging
};


#include "Useless/Graphic/Rect.h"
namespace Useless
{
    class Skin;
    class SkinMan;
    class TableSkin;
    class SizerSkin;
    class FrameSkin;
    class Text;
    class Font;
    class FontSet;
    class AdvListWidget;
    class MenuWidget;
};


namespace CPT
{
    typedef CTC::RenameCopy< Useless::Skin, 0 >                   skin;   // skin
    typedef CTC::RenameCopy< Useless::SkinMan, 0 >                skins;  // skin manager
    typedef CTC::RenameCopy< Useless::FrameSkin, 0 >              frame;  // frame skin
    typedef CTC::RenameCopy< Useless::TableSkin, 0 >              table;  // table skin
    typedef CTC::RenameCopy< Useless::SizerSkin, 0 >              sizer;  // sizer skin

    typedef CTC::RenameConstReference< Useless::Text, 0 >         text;   // text
    typedef CTC::Rename< Useless::Font, 0 >                       font;   // font
    typedef CTC::Rename< Useless::FontSet, 0 >                    fonts;  // font manager

    typedef CTC::RenameConstReference< Useless::Text, 1 >         caption;// caption (title)
    typedef CTC::RenameConstReference< Useless::Rect, 0 >         area;   // rect-area
    
    typedef CTC::Rename< Useless::Pos, 0 >                        pos;    // position
    typedef CTC::Rename< Useless::Pos, 1 >                        dim;    // dimentions

    typedef CTC::RenameCopy< Useless::Skin, 1 >                   selector; // skin for selector
    typedef CTC::RenameConstReference< Useless::Font, 1 >         hilite;   // fonst for hilites

    typedef CTC::type<Useless::AdvListWidget>                     list_widget;
    typedef CTC::type<Useless::MenuWidget>                        menu_widget;
};

#endif//__INCLUDED__USELESS_PARM_TYPES_H__
