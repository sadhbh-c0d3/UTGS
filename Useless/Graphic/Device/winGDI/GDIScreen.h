#ifndef __INCLUDED_USELESS_GDI_SCREEN_H__
#define __INCLUDED_USELESS_GDI_SCREEN_H__

#include "Useless/Graphic/Device/Screen.h"
#include "GDISurface.h"

namespace Useless {

class USELESS_API GDIScreen : public Screen
{
public:
    /* Screen overloads
    -----------------------------------*/
    GDIScreen( const char *title);
    // Destroy screen
    virtual ~GDIScreen();
    // Open fullscreen in given color depth
    virtual void  Open( int width, int height, int bpp, float refresh );
    // Open windowed screen
    virtual void  OpenWindowed( int width, int height);
    // Close screen
    virtual void  Close();
    // Swap buffers back <-> front
    virtual void  Swap();
    // Copy buffer back -> front
    virtual void  Copy();
    // Create ImageBuffer object from current backbuffer
    virtual ImageBuffer GetBackBuffer();
    // Tell if screen is valid and may blit
    virtual bool IsValid() const { return true;}
    // Retrieve x,y zoom factors in windowed mode
    virtual Point<double> GetZoomFactors() const;

    // Debug methods @{
    virtual void  PrintFPS ( int x, int y, double delta_t );
    virtual void  PrintNum ( int x, int y, int fixed, int fraction );
    virtual void  PrintText( int x, int y, const char* text );
    //@}


    /* OGraphics overloads
    -----------------------------------*/
    //! Get dimentions
    virtual int      GetWidth()  const { return _w;}
    virtual int      GetHeight() const { return _h;}
    //! Get out buffer surface
    virtual const Surface* GetSurface() const;
    virtual Surface* GetSurface();
    //! Clear rectangular area
    virtual void Clear( NormalPixel color=0L, const Useless::Rect &area = Useless::Rect() );
    //! Set clip region
    virtual void SetClipper( const RectList &rect_list );
    //! Get cropped clip region
    virtual RectList GetClipper( const Rect &crop = Rect() ) const;
    //! Query interface to IGraphics - NULL when N/A
    virtual SPointer<IGraphics> QueryIGraphics( const Rect &cut );

private:
    int        _w;
    int        _h;
    HDC        _hdc;
    SPointer<GDISurface> _back_buffer;
    
public:
    static HWND s_hwnd;
};


};//namespace Useless
#endif//__INCLUDED_USELESS_GDI_SCREEN_H__
