#ifndef __INCLUDED_SCREEN_H__
#define __INCLUDED_SCREEN_H__

#include "Useless/System/Window.h"
#include "Useless/Graphic/Planes/OGraphics.h"

namespace Useless {

class ImageBuffer;

/////////////////////////////////////////////////////
//  CLASS: DDScreen
//
/*! \defgroup gGfxAPI System Level
 *  \ingroup Graphics
 */

/*! \ingroup gGfxAPI
 *  \ingroup GfxPlanes
 *  Abstract screen class.
 */
class USELESS_API Screen : public OGraphics, virtual public Window
{
public:
    // Destroy screen
    virtual ~Screen() {}
    // Open fullscreen in color depth with refresh rate
    virtual void  Open( int width=0, int height=0, int bpp=0, float refresh=0.0f) = 0;
    // Open windowed screen
    virtual void  OpenWindowed( int width, int height) = 0;
    // Close screen
    virtual void Close() = 0;
    // Swap buffers back <-> front
    virtual void  Swap() = 0;
    // Copy buffer back -> front
    virtual void  Copy() = 0;
    // Create ImageBuffer object from current backbuffer
    virtual ImageBuffer GetBackBuffer() = 0;
    // Tell if screen is valid and may blit
    virtual bool IsValid() const = 0;
    // Retrieve x,y zoom factors in windowed mode
    virtual Point<double> GetZoomFactors() const = 0;

    // Debug methods @{
    virtual void  PrintFPS ( int x, int y, double delta_t ) = 0;
    virtual void  PrintNum ( int x, int y, int fixed, int fraction ) = 0;
    virtual void  PrintText( int x, int y, const char* text ) = 0;
    //@}
};

};//namespace Useless
#endif //__INCLUDED_SCREEN_H__
