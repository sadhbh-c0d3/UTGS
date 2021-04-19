#ifndef __INCLUDED_USELESS_GDI_TRANSFER_H__
#define __INCLUDED_USELESS_GDI_TRANSFER_H__

#include <assert.h>
#include <windows.h>
#include "Useless/Graphic/Transfer/PixelTransfer.h"
#include "Useless/Util/Storage/MemBlock.h"

namespace Useless {

class USELESS_API GDITransfer : public PixelTransfer
{
public:
    GDITransfer( HDC hdc, HBITMAP bmp );
    virtual ~GDITransfer();

    enum Type { SOURCE=1, DESTINATION=2, TRANSFORM=4 };

    //! Obtain transfer type information (combination of flags)
    virtual int  Info() { return DESTINATION; }

    //! Fetch data from source
    virtual void Fetch( const void *source, const Rect &area );

    //! Store data into destination
    virtual void Store( void *destination, const Rect &area ) {}

    //! Transform data from input buffer to destination buffer
    virtual void Transform();

    //! Set source pixel-stream type
    virtual void SetSource( int  width,
                            int  height,
                            int  pitch,
                            int  pixel_format, //!< use ImageFormat::Format or BPP
                     const void *palette = 0,
                            int  colors = 0);
    
    //! Set destination pixel-stream type
    virtual void SetDestination( int  width,
                                 int  height,
                                 int  pitch,
                                 int  pixel_format, //!< use ImageFormat::Format or BPP
                                void *palette = 0,
                                 int  colors = 0) { assert(0); }

    //! Get source pixel-stream type
    virtual void GetSource( int  *width,
                            int  *height,
                            int  *pixel_format,
                     const void **palette,
                            int  *colors );

    //! Get destination pixel-stream type
    virtual void GetDestination( int  *width,
                                 int  *height,
                                 int  *pixel_format,
                                void **palette,
                                 int  *colors ) { assert(0); }

    //! Set source buffer pointer
    virtual void SetSourcePointer( void *source) { _source=source;}

    //! Set destination area pointer
    virtual void SetDestinaitonPointer( void *destination) { assert(0);}
    
private:
    int  _width;
    int  _height;
    int  _pitch;
    int  _bmp_pitch;
    int  _pixel_format;
    const void *_palette;
    int         _colors;
    void       *_source;

    HDC        _hdc;
    HBITMAP    _bmp;
    MemBlock   _buffer;

    struct BITMAPINFO_PAL256
    {
        BITMAPINFOHEADER bmiHeader;
        RGBQUAD          bmiColors[256];
    };
    
    BITMAPINFO_PAL256 _binf;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_GDI_TRANSFER_H__
