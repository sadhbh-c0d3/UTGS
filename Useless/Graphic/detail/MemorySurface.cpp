#include "UselessPch.h"

#include "MemorySurface.h"
#include "Useless/Graphic/Pixel/PixelPtr.h"
#include "Useless/Graphic/Pixel/PixelTypes.h"
#include "Useless/System/MMX.h"
#include <math.h>
#pragma warning(disable:4244)

namespace Useless {

MemorySurface::MemorySurface()
    : _width(0), _height(0), _color_bits(0), _invert_alpha(0), _alpha_0(0), _alpha_1(256)
{
}

MemorySurface::MemorySurface( const Surf::Properties &properties ): _invert_alpha(0), _alpha_0(0), _alpha_1(256)
{
    _width = properties.width;
    _height = properties.height;
    _color_bits = 8*ImageFormat::GetBpp( properties.pixelformat);

    if ( properties.alpha ) { _alpha_plane.Alloc(_width*_height); }
    if ( properties.color ) { _color_plane.Alloc(_width*_height*_color_bits/8); }
}


//--------------------------------------------------------------------------------
/* This macro is used in this .cpp file to avoid repetitions
 */
#define __BPP_SWITCH( Command, ptr_name, surf_ptr )\
    switch( surf_ptr.format )\
    {\
        case 32:\
            {GenericPixelPtr<PixelType8888> ptr_name( surf_ptr.ptr, surf_ptr.pitch );\
            Command;}\
            break;\
        case 24:\
            {GenericPixelPtr<PixelType888> ptr_name( surf_ptr.ptr, surf_ptr.pitch );\
            Command;}\
            break;\
        case 16:\
            {GenericPixelPtr<PixelType565> ptr_name( surf_ptr.ptr, surf_ptr.pitch );\
            Command;}\
            break;\
        case 15:\
            {GenericPixelPtr<PixelType555> ptr_name( surf_ptr.ptr, surf_ptr.pitch );\
            Command;}\
            break;\
        case 14:\
            {GenericPixelPtr<PixelType4444> ptr_name( surf_ptr.ptr, surf_ptr.pitch );\
            Command;}\
            break;\
    }

namespace {
/* Inline templates used in this .cpp file to avoid repetitions
 */
template< class SrcPixelType, class DstPixelType >
inline void BlendPixels( DstPixelType &d, SrcPixelType s, unsigned char alpha )
{
    if ( alpha>2 )  d = s.Get().c.Blend( alpha, d.Get().c );
}

template< class DstPixelType >
inline void BlendPixel( DstPixelType &d, NormalPixel color, unsigned char alpha )
{
    if ( alpha>2 ) d = color.c.Blend( alpha, d.Get().c );
}

template< class SrcPixelType, class DstPixelType >
inline void DrawPixels( DstPixelType &d, SrcPixelType s )
{
    d = s.Get().c;
}

template< class DstPixelType >
inline void FillPixel( DstPixelType &d, NormalPixel color )
{
    d = color.c;
}

/* Blend blit routine - colour map and alpha mask is used
 */
template< class DstPixelType, class SrcPixelType >
inline void __BlendBlit(
    GenericPixelPtr<SrcPixelType> s_line,
    GenericPixelPtr<DstPixelType> d_line,
    const unsigned char          *a_line,
    int                           a_pitch,
    int     w,
    int     h,
    bool    inverted,
    int     alpha_0,
    int     alpha_1 )
{
    size_t line_size = w * sizeof(DstPixelType);
    MemBlock d_cache( line_size );

    while ( h-- )
    {
        MemCopy( d_cache.GetPtr(), (DstPixelType*)d_line, line_size );

        SrcPixelType *s_ptr = s_line;
        DstPixelType *d_ptr = (DstPixelType*)d_cache.GetPtr();
        const unsigned char *a_ptr = a_line;

        if ( alpha_0>0 || alpha_1<256 )
        {
            if (!inverted)
                for ( int i=w; i--; ++a_ptr, ++d_ptr, ++s_ptr )
                {
                    BlendPixels( *d_ptr, *s_ptr, *a_ptr *(alpha_1-alpha_0)/256+alpha_0 );
                }
            else
                for ( int i=w; i--; ++a_ptr, ++d_ptr, ++s_ptr )
                {
                    BlendPixels( *d_ptr, *s_ptr, (255-*a_ptr)*(alpha_1-alpha_0)/256+alpha_0 );
                }
        }
        else if (!inverted)
            for ( int i=w; i--; ++a_ptr, ++d_ptr, ++s_ptr )
            {
                BlendPixels( *d_ptr, *s_ptr, *a_ptr );
            }
        else
            for ( int i=w; i--; ++a_ptr, ++d_ptr, ++s_ptr )
            {
                BlendPixels( *d_ptr, *s_ptr, 255-*a_ptr );
            }

        MemCopy( (DstPixelType*)d_line, d_cache.GetPtr(), line_size );

        s_line.Next();
        d_line.Next();
        a_line += a_pitch;
    }
}

/* Blend blit starter - source pixel format selection
 */
template< class DstPixelType >
inline void BlendBlit(
    GenericPixelPtr<DstPixelType> d_line,
    const Surf::Ptr             &source,
    const unsigned char          *a_ptr,
    int                           a_pitch,
    const Rect                   &s_rc,
    bool  inverted,
    int   alpha_0,
    int   alpha_1 )
{
#   define __BLEND_BLIT_RUN\
        s_line += s_rc.GetPos();\
        __BlendBlit( s_line, d_line, a_ptr, a_pitch,\
            s_rc.GetW(), s_rc.GetH(), inverted, alpha_0, alpha_1 );
//  __BLEND_BLIT_RUN

    __BPP_SWITCH( __BLEND_BLIT_RUN, s_line, source )
}

/* Opaque blit routine - only colour map is used
 */
template< class DstPixelType, class SrcPixelType >
inline void __OpaqueBlit(GenericPixelPtr<SrcPixelType> s_line,
                         GenericPixelPtr<DstPixelType> d_line,
                                                int w, int h )
{
    size_t line_size = w * sizeof(DstPixelType);
    MemBlock d_cache( line_size );

    while ( h-- )
    {
        MemCopy( d_cache.GetPtr(), (DstPixelType*)d_line, line_size );

        SrcPixelType *s_ptr = s_line;
        DstPixelType *d_ptr = (DstPixelType*)d_cache.GetPtr();
    
        for ( int i=w; i--; ++d_ptr, ++s_ptr )
        {
            DrawPixels( *d_ptr, *s_ptr );
        }
    
        MemCopy( (DstPixelType*)d_line, d_cache.GetPtr(), line_size );

        s_line.Next();
        d_line.Next();
    }
}

/* Opaque blit starter - source pixel format selection
 */
template< class DstPixelType >
inline void OpaqueBlit( GenericPixelPtr<DstPixelType> d_line,
                                   const Surf::Ptr &source,
                                         const Rect &s_rc )
{
#   define __OPAQUE_BLIT_RUN\
        s_line += s_rc.GetPos();\
        __OpaqueBlit( s_line, d_line, s_rc.GetW(), s_rc.GetH() );\
//  __OPAQUE_BLIT_RUN

    __BPP_SWITCH( __OPAQUE_BLIT_RUN, s_line, source )
}

//--------------------------------------------------------------------------------

/* Blend blit routine with rotation - colour map and alpha mask is used
 */
template< class DstPixelType, class SrcPixelType >
inline void __RotoBlendBlit(
    GenericPixelPtr<SrcPixelType> s_line,
    GenericPixelPtr<DstPixelType> d_line,
    const unsigned char          *a_line,
    int                           a_pitch,
    int    w,
    int    h,
    bool   inverted,
    int    alpha_0,
    int    alpha_1,
    double angle,
    int    dest_w,
    int    dest_h,
    int    center_x,
    int    center_y )
{   
    size_t line_size = dest_w * sizeof(DstPixelType);
    MemBlock d_cache( line_size );
    int sn = 1024*sin(angle);
    int cs = 1024*cos(angle);
    int x_ = -cs*(center_x) + sn*(center_y);
    int y_ = -cs*(center_y) - sn*(center_x);
    int a_sgn_pitch = (sn>0)? a_pitch:-a_pitch;
    int s_sgn_pitch = (sn>0)? s_line.GetPitch() : -s_line.GetPitch();
    
    int quoter = (sn>=0 && cs>=0)? 1: (sn>=0 && cs<0)? 2: (sn<0 && cs<0)? 3:4;

    for ( int v=dest_h; v--; )
    {
        MemCopy( d_cache.GetPtr(), (DstPixelType*)d_line, line_size );

        DstPixelType *d_ptr = (DstPixelType*)d_cache.GetPtr();

        int x=x_;
        int y=y_;

        int a1 = -w*512 - x_;
        int a2 =  w*512 - x_;
        int b1 = -h*512 - y_;
        int b2 =  h*512 - y_;
        int a=0,b=0;

        if ( sn && cs )
            switch(quoter)
            {
            case 1:
                a = ( a1/cs > b1/sn )? a1/cs : b1/sn;
                b = ( a2/cs < b2/sn )? a2/cs : b2/sn; break;
            case 2:
                a = ( a2/cs > b1/sn )? a2/cs : b1/sn;
                b = ( a1/cs < b2/sn )? a1/cs : b2/sn; break;
            case 3:
                a = ( a2/cs > b2/sn )? a2/cs : b2/sn;
                b = ( a1/cs < b1/sn )? a1/cs : b1/sn; break;
            case 4:
                a = ( a1/cs > b2/sn )? a1/cs : b2/sn;
                b = ( a2/cs < b1/sn )? a2/cs : b1/sn; break;
            }
        else
            if ( cs ) // 0 or 180 degrees
            {
                if ( y_>=-512*h && y_<512*h )
                {
                    a = center_x - w/2;
                    b = center_x + w/2;
                }
            }
            else // 90 or 270 degrees
            {
                if ( x_>=-512*w && x_<512*w )
                {
                    a = center_x - h/2;
                    b = center_x + h/2;
                }
            }

        int k = 0;
        b = (b<dest_w)? b : dest_w;

        if ( a>=b ) { d_line.Next(); x_ += -sn; y_ += cs; continue; }

        for (; k<=a; ++k, ++d_ptr, x+=cs, y+=sn )
        {}


        Pos s_pos( x/1024 + (w/2), y/1024 +(h/2) );

        GenericPixelPtr<SrcPixelType> s_ptr( &*(s_line+s_pos), s_sgn_pitch );

        int i = x/1024 + (w/2); x%=1024;
        int j = a_pitch * (y/1024 +(h/2)); y%=1024;

#define __ALPHA_BLIT_ROTATION_LOOP( Command ) \
    { for (; k<b ; ++k, ++d_ptr ) \
    { \
        Command; \
        x += cs; i+=x/1024; s_ptr+=x/1024; x%=1024;\
        y += sn; if (y/1024) { j+=a_sgn_pitch; s_ptr.Next(); y%=1024; }\
    }}

        if ( alpha_0>0 || alpha_1<256 )
        {
          if (!inverted)
            __ALPHA_BLIT_ROTATION_LOOP(
                BlendPixels( *d_ptr, *s_ptr, a_line[j+i] *(alpha_1-alpha_0)/256+alpha_0 ) )
          else
            __ALPHA_BLIT_ROTATION_LOOP(
                BlendPixels( *d_ptr, *s_ptr, (255-a_line[j+i]) *(alpha_1-alpha_0)/256+alpha_0 ) )
        }
        else
        {
          if (!inverted)
            __ALPHA_BLIT_ROTATION_LOOP( BlendPixels( *d_ptr, *s_ptr, a_line[j+i] ) )
          else
            __ALPHA_BLIT_ROTATION_LOOP( BlendPixels( *d_ptr, *s_ptr, 255-a_line[j+i] ) )
        }

#undef __ALPHA_BLIT_ROTATION_LOOP

        MemCopy( (DstPixelType*)d_line, d_cache.GetPtr(), line_size );

        d_line.Next();
        x_ += -sn;
        y_ += cs;
    }
}

/* Blend blit starter - source pixel format selection
 */
template< class DstPixelType >
inline void RotoBlendBlit(
    GenericPixelPtr<DstPixelType> d_line,
    const Surf::Ptr             &source,
    const unsigned char          *a_ptr,
    int                           a_pitch,
    const  Rect                  &s_rc,
    bool   inverted,
    int    alpha_0,
    int    alpha_1,
    double angle,
    int    dest_w,
    int    dest_h,
    int    center_x,
    int    center_y )
{
#   define __ROTO_BLEND_BLIT_RUN\
        s_line += s_rc.GetPos();\
        __RotoBlendBlit( s_line, d_line, a_ptr, a_pitch,\
            s_rc.GetW(), s_rc.GetH(), inverted, alpha_0, alpha_1,\
            angle, dest_w, dest_h, center_x, center_y );
//  __ROTO_BLEND_BLIT_RUN

    __BPP_SWITCH( __ROTO_BLEND_BLIT_RUN, s_line, source )
}

//---------------------------------------------------------------

/* Blend color fill routine - single color and alpha mask is used
 */
template< class DstPixelType >
inline void BlendFill(
    NormalPixel                   color,
    GenericPixelPtr<DstPixelType> d_line,
    const unsigned char          *a_line,
    int                           a_pitch,
    int     w,
    int     h,
    bool    inverted,
    int     alpha_0,
    int     alpha_1 )
{
    size_t line_size = w * sizeof(DstPixelType);
    MemBlock d_cache( line_size );

    while ( h-- )
    {
        MemCopy( d_cache.GetPtr(), (DstPixelType*)d_line, line_size );

        DstPixelType *d_ptr = (DstPixelType*)d_cache.GetPtr();
        const unsigned char *a_ptr = a_line;

        if ( alpha_0>0 || alpha_1<256 ) 
        {
            if (!inverted)
                for ( int i=w; i--; ++a_ptr, ++d_ptr )
                {
                    BlendPixel( *d_ptr, color, *a_ptr *(alpha_1-alpha_0)/256+alpha_0 );
                }
            else
                for ( int i=w; i--; ++a_ptr, ++d_ptr )
                {
                    BlendPixel( *d_ptr, color, (255-*a_ptr) *(alpha_1-alpha_0)/256+alpha_0 );
                }
        }
        else if (!inverted)
            for ( int i=w; i--; ++a_ptr, ++d_ptr )
            {
                BlendPixel( *d_ptr, color, *a_ptr );
            }
        else
            for ( int i=w; i--; ++a_ptr, ++d_ptr )
            {
                BlendPixel( *d_ptr, color, 255-*a_ptr );
            }
        MemCopy( (DstPixelType*)d_line, d_cache.GetPtr(), line_size );

        d_line.Next();
        a_line += a_pitch;
    }
}

/* Blend color fill routine with rotation - single color and alpha mask is used
 */
template< class DstPixelType >
inline void RotoBlendFill (
    NormalPixel color,
    GenericPixelPtr<DstPixelType> d_line,
    const unsigned char          *a_line,
    int                           a_pitch,
    int    w,
    int    h,
    bool   inverted,
    int    alpha_0,
    int    alpha_1,
    double angle,
    int    dest_w,
    int    dest_h,
    int    center_x,
    int    center_y )
{   
    size_t line_size = dest_w * sizeof(DstPixelType);
    MemBlock d_cache( line_size );
    int sn = 1024*sin(angle);
    int cs = 1024*cos(angle);
    int x_ = -cs*(center_x) + sn*(center_y);
    int y_ = -cs*(center_y) - sn*(center_x);
    int a_sgn_pitch = (sn>0)? a_pitch:-a_pitch;
    
    int quoter = (sn>=0 && cs>=0)? 1: (sn>=0 && cs<0)? 2: (sn<0 && cs<0)? 3:4;

    for ( int v=dest_h; v--; )
    {
        MemCopy( d_cache.GetPtr(), (DstPixelType*)d_line, line_size );

        DstPixelType *d_ptr = (DstPixelType*)d_cache.GetPtr();

        int x=x_;
        int y=y_;

        int a1 = -w*512 - x_;
        int a2 =  w*512 - x_;
        int b1 = -h*512 - y_;
        int b2 =  h*512 - y_;
        int a=0,b=0;

        if ( sn && cs )
            switch(quoter)
            {
            case 1:
                a = ( a1/cs > b1/sn )? a1/cs : b1/sn;
                b = ( a2/cs < b2/sn )? a2/cs : b2/sn; break;
            case 2:
                a = ( a2/cs > b1/sn )? a2/cs : b1/sn;
                b = ( a1/cs < b2/sn )? a1/cs : b2/sn; break;
            case 3:
                a = ( a2/cs > b2/sn )? a2/cs : b2/sn;
                b = ( a1/cs < b1/sn )? a1/cs : b1/sn; break;
            case 4:
                a = ( a1/cs > b2/sn )? a1/cs : b2/sn;
                b = ( a2/cs < b1/sn )? a2/cs : b1/sn; break;
            }
        else
            if ( cs ) // 0 or 180 degrees
            {
                if ( y_>=-512*h && y_<512*h )
                {
                    a = center_x - w/2;
                    b = center_x + w/2;
                }
            }
            else // 90 or 270 degrees
            {
                if ( x_>=-512*w && x_<512*w )
                {
                    a = center_x - h/2;
                    b = center_x + h/2;
                }
            }

        int k = 0;
        b = (b<dest_w)? b : dest_w;

        for (; k<=a; ++k, ++d_ptr, x+=cs, y+=sn )
        {}

        int i = x/1024 + (w/2); x%=1024;
        int j = a_pitch * (y/1024 +(h/2)); y%=1024;

#define __ALPHA_ROTATION_LOOP( Command ) \
    { for (; k<b ; ++k, ++d_ptr ) \
    { \
        Command; \
        x += cs; i += x/1024; x %= 1024; \
        y += sn; j += (y/1024)? a_sgn_pitch : 0; y %= 1024; \
    }}

        if ( alpha_0>0 || alpha_1<256 )
        {
          if (!inverted)
            __ALPHA_ROTATION_LOOP( BlendPixel( *d_ptr, color, a_line[j+i] *(alpha_1-alpha_0)/256+alpha_0 ) )
          else
            __ALPHA_ROTATION_LOOP( BlendPixel( *d_ptr, color, (255-a_line[j+i]) *(alpha_1-alpha_0)/256+alpha_0 ) )
        }
        else
        {
          if (!inverted)
            __ALPHA_ROTATION_LOOP( BlendPixel( *d_ptr, color, a_line[j+i] ) )
          else
            __ALPHA_ROTATION_LOOP( BlendPixel( *d_ptr, color, 255-a_line[j+i] ) )
        }

#undef __ALPHA_ROTATION_LOOP

        MemCopy( (DstPixelType*)d_line, d_cache.GetPtr(), line_size );

        d_line.Next();
        x_ += -sn;
        y_ += cs;
    }
}

/* Opaque fill routine - single color is used (paints color rectangle)
 */
template< class DstPixelType >
inline void OpaqueFill(                  NormalPixel color,
                       GenericPixelPtr<DstPixelType> d_line,
                                              int w, int h )
{
    size_t line_size = w * sizeof(DstPixelType);
    MemBlock d_cache( line_size );

    while ( h-- )
    {
        MemCopy( d_cache.GetPtr(), (DstPixelType*)d_line, line_size );

        DstPixelType *d_ptr = (DstPixelType*)d_cache.GetPtr();

        for ( int i=w; i--; ++d_ptr )
        {
            FillPixel( *d_ptr, color );
        }
        MemCopy( (DstPixelType*)d_line, d_cache.GetPtr(), line_size );

        d_line.Next();
    }
}


};//unnamed namespace
//--------------------------------------------------------------------------------


/* Blend blit executor - selects destination pixel format and executes blitting routine
 */
void MemorySurface::Blit( int x, int y, const Surf::Ptr &dest, Rect s_rc ) const
{
    s_rc &= Rect(0,0,_width,_height);

    Surf::Ptr source( (void*)_color_plane.GetConstPtr(), _width * _color_bits/8, _color_bits );
    const unsigned char * a_ptr = _alpha_plane.GetConstPtrUB();
    a_ptr += s_rc.GetY()*_width + s_rc.GetX();
    bool alpha = _alpha_plane.GetSize() > 0;
    bool color = _color_plane.GetSize() > 0;

#   define __BLEND_BLIT_COMMAND\
            d_line += Pos(x,y);\
            if (alpha)\
                BlendBlit( d_line, source, a_ptr, _width,\
                    s_rc, _invert_alpha, _alpha_0, _alpha_1 );\
            else\
                OpaqueBlit( d_line, source, s_rc );
//  __BLEND_BLIT_COMMAND

    if ( color )
    __BPP_SWITCH( __BLEND_BLIT_COMMAND, d_line, dest )
}



/* Color fill executor - selects destination pixel format and executes filling routine
 */
void MemorySurface::ColorFill( int x, int y, const Surf::Ptr &dest, Rect s_rc,
                               NormalPixel color ) const
{
    s_rc &= Rect(0,0,_width,_height);

    const unsigned char * a_ptr = _alpha_plane.GetConstPtrUB();
    a_ptr += s_rc.GetY()*_width + s_rc.GetX();
    bool alpha = _alpha_plane.GetSize() > 0;

#   define __BLEND_FILL_COMMAND\
            d_line += Pos(x,y);\
            if(alpha)\
            BlendFill( color, d_line, a_ptr, _width,\
                s_rc.GetW(), s_rc.GetH(), _invert_alpha, _alpha_0, _alpha_1 );\
            else\
            OpaqueFill( color, d_line, s_rc.GetW(), s_rc.GetH() );
//  __BLEND_FILL_COMMAND

    __BPP_SWITCH( __BLEND_FILL_COMMAND, d_line, dest )
}



/* Blend blit with rotation executor - selects destination pixel format
 * and executes roto filling routine
 */
void MemorySurface::RotoBlit( Rect d_rc, const Surf::Ptr &dest, Rect s_rc,
                              double angle, Pos pivot ) const
{
    s_rc &= Rect(0,0,_width,_height);

    Surf::Ptr source( (void*)_color_plane.GetConstPtr(), _width * _color_bits/8, _color_bits );
    const unsigned char * a_ptr = _alpha_plane.GetConstPtrUB();
    a_ptr += s_rc.GetY()*_width + s_rc.GetX();
    bool alpha = _alpha_plane.GetSize() > 0;
    bool color = _color_plane.GetSize() > 0;
    assert(alpha);
    pivot -= d_rc.GetPos();

#   define __ROTO_BLEND_BLIT_COMMAND\
            d_line += d_rc.GetPos();\
                RotoBlendBlit( d_line, source, a_ptr, _width,\
                    s_rc, _invert_alpha, _alpha_0, _alpha_1,\
                    angle, d_rc.GetW(), d_rc.GetH(), pivot.x, pivot.y );
//  __ROTO_BLEND_BLIT_COMMAND

    if ( color )
    __BPP_SWITCH( __ROTO_BLEND_BLIT_COMMAND, d_line, dest )
}


/* Color fill with rotation executor - selects destination pixel format
 * and executes roto filling routine
 */
void MemorySurface::RotoColorFill( Rect d_rc, const Surf::Ptr &dest, Rect s_rc,
                                   NormalPixel color, double angle, Pos pivot ) const
{
    s_rc &= Rect(0,0,_width,_height);

    const unsigned char * a_ptr = _alpha_plane.GetConstPtrUB();
    a_ptr += s_rc.GetY()*_width + s_rc.GetX();
    bool alpha = _alpha_plane.GetSize() > 0;
    assert(alpha);
    pivot -= d_rc.GetPos();

#   define __ROTO_BLEND_FILL_COMMAND\
        d_line += d_rc.GetPos();\
        RotoBlendFill( color, d_line, a_ptr, _width,\
            s_rc.GetW(), s_rc.GetH(), _invert_alpha, _alpha_0, _alpha_1,\
            angle, d_rc.GetW(), d_rc.GetH(), pivot.x, pivot.y );
//  __ROTO_BLEND_FILL_COMMAND

    __BPP_SWITCH( __ROTO_BLEND_FILL_COMMAND, d_line, dest )
}




/* Method sets alpha factors
 * alpha_0 is alpha value [0..255] of most black alpha image pixel
 * alpha_1 is alpha value [0..255] of most white alpha image pixel
 * If alpha_0 > alpha_1 then alpha image is inversed
 */
void MemorySurface::SetFactors( int alpha_0, int alpha_1 )
{
    /* prevent value from being out-of-range */
    _alpha_0 = Saturate(alpha_0);
    _alpha_1 = Saturate(alpha_1);

    /* dividing by 256 instead of 255 needs this trick */
    if ( _alpha_0==255 && _alpha_1==0 ) { _alpha_1 = -1 ; } else
    if ( _alpha_1==255 && _alpha_0==0 ) { _alpha_1 = 256; }
}


};//namespace Useless
