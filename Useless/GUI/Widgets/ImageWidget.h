#ifndef __INCLUDED_USELESS_IMAGE_WIDGET_H__
#define __INCLUDED_USELESS_IMAGE_WIDGET_H__

/*
 *
 *    NAME
 *        ImageWidget
 *
 *    PURPOSE
 *        Image GUI primitive
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/GUI/Skins/ImageSkin.h"
#include "Useless/Util/CtorWrapper.h"
#include "Useless/Util/CPT.h"

namespace Useless {

/*! \ingroup Widgets
 *  Simple Image Widget.
 */
class USELESS_API ImageWidget : virtual public Widget
{
public:
#   include "ImageWidget.inl" // ParmList Ctor

    typedef CtorWrapper_2< Skin, ImageSkin, ImageBase,
                                 ImageSkin, const char * >  RefSkin;

    ImageWidget( const RefSkin &ref_skin );
    virtual ~ImageWidget();

    virtual void Repaint( const WidgetPainter& painter );
    virtual void Resize ( int w, int h );

    const Skin &GetSkin() const                   { return *_sp_skin; }
          Skin &GetSkin()                         { return *_sp_skin; }
          void  SetSkin( const RefSkin &ref_skin );

private:
    SPointer< Skin > _sp_skin;
};

};// namespace Useless

#endif// __INCLUDED_USELESS_IMAGE_WIDGET_H__
