#ifndef __INCLUDED__USELESS_SLIDER_WIDGET_H__
#define __INCLUDED__USELESS_SLIDER_WIDGET_H__

#pragma warning (disable:4250)
#include "Useless/GUI/Widgets/EnumWidget.h"
#include "Useless/GUI/Widgets/BridgeWidget.h"
#include "Useless/GUI/Widgets/DraggerWidget.h"
#include "Useless/GUI/Skins/SkinSet.h"

namespace Useless {

//USES
    class SliderStyle;


class USELESS_API SliderWidget : private BridgeWidget, virtual public DraggerWidget, virtual public EnumWidget
{
public:
    enum Images { LANE, LANE_PRESSED, LANEHL, LANEDIS, GRIP, GRIP_PRESSED, GRIPHL, GRIPDIS };

    SliderWidget( int size, const SkinSet &skins );
    SliderWidget( const SliderStyle &style);
    virtual ~SliderWidget();

    virtual void Repaint(const WidgetPainter &painter );
    virtual void Resize (int w, int h);

    virtual  int GetSelected() const;
    virtual void SetSelected(int s);

    virtual bool Accept ( const Request& request);

            void SetNumStates(int n);
             int GetNumStates() const { return _num_states; }

            void SetLength (int len)   { Resize( _horizontal ? len:GetWidth(), _horizontal ? GetHeight():len); }
             int GetLength () const    { return (_horizontal)? GetWidth() : GetHeight(); }
            bool IsHorizontal() const  { return _horizontal; }

            void SetGripSize(int s);
             int GetGripSize() const { return IsHorizontal() ? _grip->GetWidth():_grip->GetHeight(); }

    void Snap();//!< Tie Snap() to OnChange for snap while dragging

private:
    bool            _horizontal;
    int             _num_states;
    bool            _dragging_grip;
    DraggerWidget  *_grip;

    void GotoPosition(const Pos &pos);
    void GripSlide(const Pos &pos);
    void CaptureGrip(bool t);
    void SetGripPosition(int p);

    void Init( int size, const SkinSet &skins );
    static SkinSet ExtractLaneSkin( const SkinSet &skins );
};


class SliderStyle
{
public:
    SliderStyle( int size, const SkinSet &skins ):
        size(size), skins(skins) {}

    int            size;
    const SkinSet &skins;
};


};//namespace Useless

#endif//__INCLUDED__USELESS_SLIDER_WIDGET_H__
