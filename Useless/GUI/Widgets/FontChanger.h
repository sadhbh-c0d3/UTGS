#ifndef __INCLUDED__USELESS__FONT_CHANGER_H__
#define __INCLUDED__USELESS__FONT_CHANGER_H__

#include "Useless/GUI/Widgets/FontWidget.h"
#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Text/FontSet.h"
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Misc/DisplayState.h"

namespace Useless {

/*! An overlay for abstract FontWidget
 *  An overlayed FontWidget will change font on state changes
 *
 *  In Useless there is some convention:
 *   Base{NAME}    :is base type
 *   {NAME}Widget  :defines input-transparent version
 *   {NAME}AndFont :defines state-dependant font-changing version
 *   HL{NAME}      :defines version with HiliteLogic
 *
 *  Example:
 *   BaseTextWidget :is base for any simple text-widget
 *   TextWidget     :is input-transparent version of this simple text-widget
 *   TextAndFont    :is version which may change font
 *   HLText         :is version that would highlite if mouse was over it
 *
 *
 */
class USELESS_API FontChanger : virtual public FontWidget, virtual public LogicWidget
{
public:
    enum { DEFAULT=DisplayState::NORMAL };

    FontChanger(): _state(DEFAULT) {}
    virtual ~FontChanger() {}

    virtual bool SetState( int state )
    {
        if (_fonts.Exists(state) ) 
        {
            _state=state;
            SetFont( _GetFont() );
        }
        return ( _state==state ); 
    }

    virtual bool HasState( int state ) const { return _fonts.Exists(state); }
    virtual int  GetState() const { return _state; }

    void UseFontSet( const FontSet &fonts ) 
    {
        _fonts = fonts;
        SetFont( _GetFont() ); 
    }

private:
    int     _state;
    FontSet _fonts;

//-- inline helpers
    const Font& _GetFont() const
    {
        if ( _fonts.Exists(_state) ) { return _fonts.GetFont(_state); }
        else return _fonts.GetFont(DEFAULT);
    }

    Font& _GetFont()
    {
        if ( _fonts.Exists(_state) ) { return _fonts.GetFont(_state); }
        else return _fonts.GetFont(DEFAULT);
    }
};

template< class WidgetType >
struct MultiFont
{
    typedef TemplateWidget< WidgetType, FontChanger > Widget;
};

template< class WidgetType, class LogicType >
struct ActiveMultiFont
{
    typedef USELESS_TYPENAME TemplateWidget< USELESS_TYPENAME MultiFont<WidgetType>::Widget, LogicType > Widget;
};

};//namespace Useless
#endif//__INCLUDED__USELESS__FONT_CHANGER_H__
