#ifndef __INCLUDED_USELESS_CONTENT_WIDGET_H__
#define __INCLUDED_USELESS_CONTENT_WIDGET_H__

#include "Useless/GUI/Misc/SkinSelector.h"
#include "Useless/GUI/Misc/FontSelector.h"
#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Text/FontPainter.h"
#include "Useless/GUI/Misc/DataStream.h"
#include <memory>

namespace Useless {

/*! \ingroup Widgets
 *  Kind of LogicWidget that paints data object over skin.
 *  May be used as button
 */
class USELESS_API ContentWidget : virtual public LogicWidget, virtual public IWContent
{
public:
    ContentWidget( const InternalPointer< DataObject > &content,
                   const SkinMan &skins,
                   const FontSet &fonts,
                   const Pos &distance = Pos() );

    virtual ~ContentWidget();

    virtual ContentWidget& Content() { return *this; }

    void  SetContent( InternalPointer< DataObject > caption );
    InternalPointer< DataObject > GetContent() const;

    virtual void  Repaint( const WidgetPainter &painter );
    virtual void  Resize( int w, int h );

private:
    InternalPointer< DataObject >   _content;
    Rect            _content_area;
    SkinSelector    _skins;
    FontSelector    _fonts;
    Pos             _distance;

    void UpdateContent();

public:
     virtual  int  GetState () const;
     virtual bool  SetState (int state );
     virtual bool  HasState (int state ) const;
};


};//namespace Useless
#endif//__INCLUDED_USELESS_CONTENT_WIDGET_H__
