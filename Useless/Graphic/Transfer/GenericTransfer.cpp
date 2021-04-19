#include "UselessPch.h"

#include "GenericTransfer.h"
#include "Useless/Graphic/Pixel/Dither.h"
#include "Useless/Graphic/Pixel/CopyPixels.h"

namespace Useless {

GenericTransfer::GenericTransfer( void *source, void *destination )
{
    Initialize( source, destination);
}

void GenericTransfer::Initialize( void *source, void *destination )
{
    _source = source;
    _destination = destination;
    _input_dest = _destination; // destination for Fetch() operation
    _out_source = _source;      // source for Store() operation
    _s_width = 0;
    _s_height = 0;
    _s_pitch = 0;
    _s_pixel_format = 0;
    _s_palette_colors = 0;
    _s_palette = 0;
    _d_width = 0;
    _d_height = 0;
    _d_pitch = 0;
    _d_pixel_format = 0;
    _d_palette_colors = 0;
    _d_palette = 0;
    _need_transform = false;
    _need_fetch = false;
    _need_store = false;
}

void GenericTransfer::SetSource(
        int  width,
        int  height,
        int  pitch,
        int  pixel_format,
 const void *palette,
        int  colors )
{
    _s_width = width;
    _s_height = height;
    _s_pitch = pitch;
    _s_pixel_format = pixel_format;
    _s_palette_colors = colors;
    _s_palette = palette;
    PrepareConverter(TRANSFORM);
}

void GenericTransfer::SetDestination(
        int  width,
        int  height,
        int  pitch,
        int  pixel_format,
       void *palette,
        int  colors )
{
    _d_width = width;
    _d_height = height;
    _d_pitch = pitch;
    _d_pixel_format = pixel_format;
    _d_palette_colors = colors;
    _d_palette = palette;
    PrepareConverter(TRANSFORM);
}

void GenericTransfer::GetSource(
        int  *width,
        int  *height,
        int  *pixel_format,
 const void **palette,
        int  *colors )
{
    *width = _s_width;
    *height = _s_height;
    *pixel_format = _s_pixel_format;
    *colors = _s_palette_colors;
    *palette = _s_palette;
}

void GenericTransfer::GetDestination(
        int  *width,
        int  *height,
        int  *pixel_format,
       void **palette,
        int  *colors )
{
    *width = _d_width;
    *height = _d_height;
    *pixel_format = _d_pixel_format;
    *colors = _d_palette_colors;
    *palette = _d_palette;
}

void GenericTransfer::SetSourcePointer( void *source)
{
    _source = source;
}

void GenericTransfer::SetDestinaitonPointer( void *destination)
{
    _destination = destination;
}

void* AllocPixelBuffer( MemBlock &memory, int height, int pitch )
{
    if ( pitch>0 )
    {
        if ( memory.GetSize() != (height*pitch))
            memory.Alloc( height*pitch );
        return memory.GetPtr();
    }
    else
    {
        if ( memory.GetSize() != (height*(-pitch)))
            memory.Alloc( height*(-pitch) );
        return (memory.GetPtrUB() + (height-1)*(-pitch));
    }
}

bool InsidePixelBuffer( void *pointer, const MemBlock &memory, int height, int pitch )
{
    int p = (pitch>0)? pitch : -pitch;

    const void *p_begin = memory.GetConstPtrUB();
    const void *p_end = memory.GetConstPtrUB() + height*p;

    return ( pointer>=p_begin && pointer<p_end );
}

/* Transfer convention:
 *
 *   SOURCE        -(Fetch)->     input_buffer
 *   input_buffer  -(Transform)-> output_buffer
 *   output_buffer -(Store)->     DESTINATION
 */
void GenericTransfer::PrepareConverter( int which )
{
    if ( _d_pixel_format != 0 && _s_pixel_format != 0 )
    {
        if ( _d_pixel_format != _s_pixel_format )
        {
            if ( !_source ) // will use output buffer
            {
                _input_dest = AllocPixelBuffer( _input_buffer, _s_height, _s_pitch);
            }
            else // will read directly from source
            {
                _input_dest = _source;
            }
            if ( !_destination ) // will use output buffer
            {
                _out_source = AllocPixelBuffer( _output_buffer, _d_height, _d_pitch);
            }
            else // will write directly to destination
            {
                _out_source = _destination;
            }
            _need_transform = true;
        }
        else if ( _source)
        {
            // if source and destination was given, then set flag
            // for transfering data from source to destination.
            if ( _destination && (_source != _destination))
            {
                _need_store = true;
            }
            _out_source = _source; //< set storing source
        }
        else if ( _destination)
        {
            // if source and destination was given, then set flag
            // for transfering data from source to destination.
            if ( _source && (_source != _destination))
            {
                _need_fetch = true;
            }
            _input_dest = _destination; //< set fetching destination
        }
    }
}

/*! Fetch() - loads bytes from 'source' into '_input_dest'
 *
 *  '_input_dest' may be direct destination (in ctor) or internal input_buffer used for convertions
 */
void GenericTransfer::Fetch( const void *source, const Rect &area )
{
    C_verify( _input_dest);
    // Do we write directly to the output ?
    int d_pitch = ( _input_dest==_destination )? _d_pitch : _s_pitch;

    int bytes = ImageFormat::GetBpp( ImageFormat::Format(_s_pixel_format));
    int offset = area.GetY() * d_pitch + area.GetX()*bytes;
    int line_s = area.GetW() * bytes;

    char *s_ptr = (char*)source; // input source
    char *d_ptr = (char*)_input_dest + offset; // input buffer

    int h = area.GetH();
    while ( 0 < h-- )
    {
        memcpy( d_ptr, s_ptr, line_s);
        s_ptr += _s_pitch;
        d_ptr += d_pitch;
    }
    _need_fetch = false;
}

/*! Store() - stores bytes from '_out_source' into 'destination'
 *
 *  '_out_source' may be direct source (i ctor) or internal output_buffer used for convertions
 *
 */
void GenericTransfer::Store( void *destination, const Rect &area )
{
    C_verify( _out_source);
    // Do we read directly from the input ?
    int s_pitch = ( _out_source==_source )? _s_pitch : _d_pitch;

    int bytes = ImageFormat::GetBpp( ImageFormat::Format(_d_pixel_format));
    int offset = area.GetY() * s_pitch + area.GetX()*bytes;
    int line_s = area.GetW() * bytes;

    char *s_ptr = (char*)_out_source + offset; // output buffer
    char *d_ptr = (char*)destination; // output destination

    int h = area.GetH();
    while ( 0 < h-- )
    {
        memcpy( d_ptr, s_ptr, line_s);
        s_ptr += s_pitch;
        d_ptr += _d_pitch;
    }
    _need_store = false;
}

void GenericTransfer::FetchWholeImage()
{
    Fetch( _source, Rect(0,0,_s_width,_s_height) );
    _need_fetch=false;
}

void GenericTransfer::StoreWholeImage()
{
    Store( _destination, Rect(0,0,_d_width,_d_height) );
    _need_store=false;
}

void GenericTransfer::Transform()
{
    /*! I include support only for dithering to lower BPP
     *  Resizing or palette is not suported right now
     */
    assert( _s_width == _d_width );
    assert( _s_height == _d_height );
    //assert( _s_palette_colors == 0 && _s_palette == 0 );
    //assert( _d_palette_colors == 0 && _d_palette == 0 );
    assert( _input_dest && _out_source && _input_dest != _out_source || !_need_transform );

    if (_need_fetch) FetchWholeImage();

    // Apply transformation only if input and output are initialized
    // If one of them is uninitialized it means
    // that Fetch() or Store() are direct operations
    try{
    if (_need_transform)
    __PIXEL_SWITCH( _s_pixel_format,
    __PIXEL_SWITCH( _d_pixel_format, TransformImp(s_ptr,d_ptr), d_ptr ), s_ptr );
    }
    // try-catch W/A - on Relese some bad things are happening after alt-tab in airlines
    catch(...) { assert(!"GenericTransfer::Transorm(): segmentation fault");}

    _need_transform=false;

    if (_need_store) StoreWholeImage();
}

bool GenericTransfer::NeedTransform() const
{
    return _need_transform;
}


/*! Transform implementation for specific source and destination PixelTypes
 */
template< class SrcPixelType, class DstPixelType >
void GenericTransfer::TransformImp( const SrcPixelType&, const DstPixelType& )
{
    assert(_input_dest && _out_source && _input_dest!=_out_source);

    GenericPixelPtr< SrcPixelType > s_ptr( _input_dest, _s_pitch );
    GenericPixelPtr< DstPixelType > d_ptr( _out_source, _d_pitch );

    int s_bpp = ImageFormat::GetBpp( ImageFormat::Format(_s_pixel_format));
    int d_bpp = ImageFormat::GetBpp( ImageFormat::Format(_d_pixel_format));

    // If source BPP > dest BPP then dither
    if ( !_fastest && (s_bpp > 2 && d_bpp == 2 ))
    {
        Dither( s_ptr, d_ptr, _d_width, _d_height );
    }
    // Just copy pixels from source to destination
    else
    {
        CopyPixels( s_ptr, d_ptr, _d_width, _d_height );
    }

    // Stretch W/H and Swap RGB transforms are not implemented yet
}

}; //namespace Useless
