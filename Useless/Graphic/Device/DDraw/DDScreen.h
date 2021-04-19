#ifndef __INCLUDED_DDScreen_H__
#define __INCLUDED_DDScreen_H__

/*
**  $Id: DDScreen.h,v 1.7 2003/10/08 09:29:30 koolas Exp $
**
**  NAME
**      DDScreen (DDraw version)
**
**  PURPOSE
**      Display graphics via DirectDraw
**
**  NOTE
**      DDScreen is the only one who creates surfaces.
**      Use Validate function to create surface.
**      Validate(Surface *p, ImageDesc d) will check (*p) surface
**      and if it's nessesary it will (re)create this surface
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**
*/

#include "Useless/Graphic/Device/Screen.h"
#include "Useless/System/w32/DirectX/DDrawWindow.h"
#include "Useless/Graphic/detail/ImageDesc.h"
#include "Useless/Util/Shares.h"

namespace Useless {

// REQUIRE:
    class ImageBuffer;

/////////////////////////////////////////////////////
//  CLASS: DDScreen
//
/*! \defgroup gGfxAPI System Level
 *  \ingroup Graphics
 */

/*! \ingroup gGfxAPI
 *  \ingroup GfxPlanes
 *  Class attended for use with DDSurface
 *
 *  Uses DDCreator to init DDSurfaces. Only DirectDraw compatibile Surfaces
 *  can be blited on it.
 */
class DDScreen : public Screen, virtual public DDrawWindow
{
public:
    DDScreen( const char *title = "IAV_Game" );
    DDScreen( int w, int h, int bpp, const char *title = "IAV_Game" );
    virtual ~DDScreen();

    virtual const Surface* GetSurface() const;
    virtual       Surface* GetSurface();

    virtual int      GetWidth () const { return _w; }
    virtual int      GetHeight() const { return _h; }

    virtual void     Clear( NormalPixel color=0L, const Rect &area=Rect() );

    virtual void SetClipper( const RectList &region );
    virtual RectList GetClipper( const Rect &crop = Rect() ) const;

    virtual SPointer<IGraphics> QueryIGraphics( const Rect &cut );

    void  Open        (int w, int h, int bpp, float refresh);
    void  OpenWindowed(int w, int h);
    void  Close();
    void  Swap();
    void  Copy();
               

    void  PrintFPS ( int x, int y, double delta_t );
    void  PrintNum ( int x, int y, int fixed, int fraction );
    void  PrintText( int x, int y, const char* text );

    void        RestoreDisplay();
    ImageBuffer GetBackBuffer();

    bool IsValid() const;

    Point<double> GetZoomFactors() const;

private:
    int _w, _h, _bpp;

    RectList _region_cache;

    friend class Clipper;
    friend class Renderer;

    Share<Surface> _emul_sh_surface;
};

};// namespace Useless
#endif //__INCLUDED_DDScreen_H__
