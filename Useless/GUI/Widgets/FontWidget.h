#ifndef __INCLUDED__USELESS__FONT_WIDGET_H__
#define __INCLUDED__USELESS__FONT_WIDGET_H__

#include "Useless/GUI/Text/Font.h"
#include "Useless/GUI/Widgets/Widget.h"

namespace Useless {

class USELESS_API FontWidget : virtual public Widget
{
public:
    virtual void SetFont( const Font &font ) = 0;
    virtual const Font& GetFont() const = 0;
};

};//namespace Useless
#endif//__INCLUDED__USELESS__FONT_WIDGET_H__
