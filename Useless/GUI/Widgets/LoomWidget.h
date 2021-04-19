#ifndef __INCLUDED_USELESS_LOOM_WIDGET_H__
#define __INCLUDED_USELESS_LOOM_WIDGET_H__

#include "Useless/GUI/Widgets/BridgeWidget.h"
#include "Useless/System/Timer.h"

namespace Useless {

class USELESS_API LoomWidget : public BridgeWidget
{
public:
    enum Technique { SLIDE_DOWN, SLIDE_UP, SLIDE_LEFT, SLIDE_RIGHT };

    LoomWidget( Technique technique, double loom_duration=1.0 );
    virtual ~LoomWidget();

    /////////////////////////////////////////////////////////////////////////
    //               S I G N A L S
    //       TYPE                    | NAME                      | ARGS
    //-----------------------------------------------------------------------
    mutable Signal                    OnLoomStop;                // none
    mutable Signal                    OnVanishStop;              // none



    virtual void Repaint( const WidgetPainter &painter );
    virtual bool IsOver(const Pos &pos) const;

    virtual void    AttachWidget( Widget *widget, int x=0, int y=0);
    virtual Widget* DetachWidget();

    void LoomUp();
    void Vanish();

    bool IsUp() const;
    void LoomVanish();

    void SetMinVisible( int min_visible_pixels );

private:
          double  _loom_duration;
           Timer  _loom_time;
          double  _stage;
            bool  _loom_mode;
            bool  _running;
       Technique  _technique;
             int  _min_visible_pixels;
  Signal::FuncID  _on_resize;
            bool  _one_more_dirty;

  void UpdateSize();
  void UpdateStage();

};

};//namespace Useless

#endif//__INCLUDED_USELESS_LOOM_WIDGET_H__
