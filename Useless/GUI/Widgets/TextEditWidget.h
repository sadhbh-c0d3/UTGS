#ifndef __INCLUDED_USELESS_TEXT_EDIT_WIDGET_H__
#define __INCLUDED_USELESS_TEXT_EDIT_WIDGET_H__

#include "Useless/GUI/Text/Font.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Skins/Skin.h"

namespace Useless {

/*! \ingroup Widgets
 *  TextEditWidget Widget is a single line editable textfield.
 */
class USELESS_API TextEditWidget : public ActiveWidget
{
public:
    TextEditWidget( int w, int h, int x, const Font &font,CopyOf<Skin> frame_skin,
                    CopyOf<Skin> marker_skin, const Text &text="...");

    virtual ~TextEditWidget();

    /////////////////////////////////////////////////////////////////////////
    //               S I G N A L S
    //       TYPE                    | NAME                      | ARGS
    //-----------------------------------------------------------------------
    mutable Signal_1<Text>           OnConfirm;                  // none

    void PutChar( char ch);
    void BackSpace();
    void Delete();
    void CarBack();
    void CarForward();
    void CarHome();
    void CarEnd();
    void Confirm();
    void ToggleSelect(bool t);

    void BeginSelect( const Pos &press_pos);
    void UpdateSelect( const Pos &pos);
    void UpdateCar();

    virtual void Repaint( const WidgetPainter &painter );

       void SetText(const Text &text);
       Text GetText() const { return _text; }
      Font& GetFont() { return _font; }

    DEFINE_CYCLIC_VISITABLE(Widget);

private:
    Pos        _text_position;
    int        _x_scroll;
    int        _begin;
    int        _end;
    int        _click;
    int        _sel_car;
    int        _car;
    int        _car_end;
    int        _car_w[2];
    bool       _key_select;
    Font       _font;
    Text       _text;

    Font::Letters  _letters;
    SPointer<Skin> _sp_frame_skin;
    SPointer<Skin> _sp_marker_skin;
   
};

inline void __obsolete_text_button() { STATIC_WARNING(USING_OBSOLETE_TEXT_EDIT); }

};//namespace Useless
#endif//__INCLUDED_USELESS_TEXT_EDIT_WIDGET_H__
