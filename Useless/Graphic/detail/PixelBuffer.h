#ifndef __INCLUDED_CODE_PIXEL_BUFFER_H__
#define __INCLUDED_CODE_PIXEL_BUFFER_H__

#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/Graphic/Transfer/GenericTransfer.h"

namespace Useless {

/*! PixelBuffer uses ImageLoader to load pixels
*/
class USELESS_API PixelBuffer
{
public:
    PixelBuffer();
    
    void AllocatePixels( int w, int h, int fmt, int alignment_x, int alignment_y, int border_w, int border_h );
    void FreePixels();
    bool BoundCheck( char *ptr, int w=1, int h=1 ) const;

    ~PixelBuffer();

    int GetWidth ()const { return _w; }
    int GetHeight()const { return _h; }
    int GetFormat()const { return _fmt;}
    int GetPitch() const { return _pitch;}
    int GetBytesPP()const{ return _bytes_pp;}
    int GetRowLength()const { return (GetPitch() / GetBytesPP()); }
    char *GetData();
    const char *GetData()const;
    int GetBufferSize() const { return _buffer_size; }

    operator char* () { return GetData(); }
    operator const char* () const { return GetData(); }

private:
    int   _w;
    int   _h;
    int   _fmt;
    int   _pitch;
    int   _bytes_pp;
    char *_data;
    int   _buffer_size;

    PixelBuffer( const PixelBuffer &);
    PixelBuffer &operator =( const PixelBuffer &);
};


}; //namespace Useless
#endif//__INCLUDED_CODE_PIXEL_BUFFER_H__
