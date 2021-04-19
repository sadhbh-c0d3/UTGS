#ifndef __INCLUDED_USELESS_CAPTION_WIDGET_H__
#define __INCLUDED_USELESS_CAPTION_WIDGET_H__

#include "Useless/GUI/Misc/SkinSelector.h"
#include "Useless/GUI/Misc/FontSelector.h"
#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Text/FontPainter.h"
#include "Useless/Util/CPT.h"

namespace Useless {

struct CaptionStyle;

/*! \ingroup Widgets
 *  Kind of LogicWidget that paints text over skin.
 */
class USELESS_API CaptionWidget : virtual public LogicWidget, virtual public IWCaption
{
public:
    enum Alignment{ INNER=0, INNER_LEFT=1, INNER_RIGHT=2, OUTER_LEFT=3, OUTER_RIGHT=4 };

#   include "CaptionWidget.inl" // ParmList Ctor

    CaptionWidget(       const Text &caption, 
                      const SkinMan &skins, 
                      const FontSet &fonts,
                       unsigned int  alignment=INNER,
                                Pos  distance=Pos()
                 );

    CaptionWidget( const Text &caption, const CaptionStyle &style );
    virtual ~CaptionWidget();

    virtual CaptionWidget& Caption() { return *this; }

    void SetCaption(const Text &caption);
    Text GetCaption() const;


     virtual void  Repaint( const WidgetPainter &painter );
     virtual void  Resize(int w, int h);

     virtual  int  GetState () const;
     virtual bool  SetState (int state);
     virtual bool  HasState (int state) const;

/*-------------------------------------------------------------------*/
protected:
              int GetSkinPos()  const;
              int GetFontPos()  const;
     virtual void UpdateCaption( int w,int h, bool update_dimentions=false );

private:

     SkinSelector       _skins;
     FontSelector       _fonts;
      FontPainter       _fn_painter;
     unsigned int       _fp_alignment;
     unsigned int       _alignment;
              Pos       _distance;
             Text       _caption;

              int       _caption_pos;
              int       _skin_pos;
              int       _line_spacing;
};
/*-------------------------------------------------------------------*/
inline int CaptionWidget::GetSkinPos()  const
{
    return _skin_pos;
}

inline int CaptionWidget::GetFontPos()  const
{
    return _caption_pos;
}
/*-------------------------------------------------------------------*/


struct CaptionStyle
{
    CaptionStyle(
                  const SkinMan &skins, 
                  const FontSet &fonts,
                  unsigned int  alignment=0,
                           Pos  distance=Pos()
                ): skins(skins), fonts(fonts), alignment(alignment), distance(distance) {}

    const SkinMan &skins;
    const FontSet &fonts;
     unsigned int  alignment;
              Pos  distance;
};

};//namespace Useless


#endif//__INCLUDED_USELESS_CAPTION_WIDGET_H__
