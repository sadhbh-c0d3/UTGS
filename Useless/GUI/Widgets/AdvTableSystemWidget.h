#ifndef __INCLUDED_USELESS_ADVTALBLE_SYSTEM_WIDGET_H__
#define __INCLUDED_USELESS_ADVTALBLE_SYSTEM_WIDGET_H__

/*
** $Id: AdvTableSystemWidget.h,v 1.6 2004/01/09 20:05:05 koolas Exp $
**
** NAME 
**    AdvTableSystemWidget
**
** PURPOSE 
**    AdvTableSystemWidget
**
** AUTHOR(S)
**      Sebastian Szymanski (szyman@iavision.com )
*/

#include "Useless/GUI/Misc/SkinSelector.h"
#include "Useless/GUI/Misc/FontSelector.h"
#include "Useless/GUI/Misc/DataFilter.h"
#include "Useless/GUI/Skins/SkinMan.h"
#include "Useless/GUI/Text/FontSet.h"
#include "Useless/GUI/Widgets/FrameWidget.h"
#include "Useless/GUI/Widgets/AdvScrollbarWidget.h"
#include "Useless/GUI/Widgets/TableSystemWidget.h"
#include "Useless/GUI/Widgets/ContentWidget.h"

namespace Useless {

struct DF_ShowColumns : DataFilter
{
    DEFINE_CYCLIC_VISITABLE(DataFilter);
    BitVector _columnHidden;
};

class USELESS_API AdvTableSystemWidget : public TableSystemWidget
{
public:
    AdvTableSystemWidget::AdvTableSystemWidget
        ( const SkinMan             * content_button_skins,
          const FontSet             * content_button_fonts,
          const ScrollbarStyle      * VScrollStyle,
          const ScrollbarStyle      * HScrollStyle,
          const FrameStyle          * frameStyle );
 
    virtual ~AdvTableSystemWidget();
    
    virtual void Repaint( const WidgetPainter &painter );

	    void ShowColumn ( int column );
  	    void HideColumn ( int column );
            void Sort       ( int column );

            void Insert ( Widget *pwidget );
    
private:
	    void UpdateTableHeaders();
            void DataConnected();

            class ColumnProperty
            {
            public:
                ColumnProperty( int width, bool fixed, bool visible )
                    : _width( width )
                    , _fixed( fixed )
                    , _visible( visible ) {}
                
                int  _width;
                bool _fixed;
                bool _visible;
            };
            
    typedef std::vector< ColumnProperty > ColumnProperties;
    typedef std::vector< ContentWidget *> ContentWidgetsVector; 

    ColumnProperties     _columns_properties;
    SkinSelector         _content_button_skins;
    FontSelector         _content_button_fonts;
    SPointer< DF_ShowColumns >  _columnFilter;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_ADVTALBLE_SYSTEM_WIDGET_H__
