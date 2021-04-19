#ifndef __INCLUED_USELESS_GL_TEXTURE_SURFACE_H__
#define __INCLUED_USELESS_GL_TEXTURE_SURFACE_H__

#include "Useless/Graphic/Device/Surface.h"
#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/Graphic/Device/GL/GLImageTesselator.h"

namespace Useless {

    struct USELESS_API GLTextureSurface : Surface
    {
        GLTextureSurface();
        GLTextureSurface( Surf::Properties &properties );

        ~GLTextureSurface();
    
        Surface *
            CreateSurface( Surf::Properties &properties ) const;
    
        SPointer< SurfacePainter >
            CreatePainter()
                { assert(0); return 0; }

        bool IsValidBlitSource( const Surface &surface ) const
                { assert(0); return false; }

        SPointer< PixelTransfer >
            CreateReader( int channelFormat = COLOR ) const;
        
        SPointer< PixelTransfer >
            CreateWriter( int channelFormat = COLOR );
        
        void Blit
            ( int x, int y,
              const Surface &src,
              const Useless::Rect &src_rect,
              const Surf::BlitFX & fx
            ) { assert(0); }

        void Clear
            ( unsigned long color,
              const Useless::Rect &drc
            ) { assert(0); }


        int  GetWidth () const { return _pixelBuffer.GetWidth(); }
        int  GetHeight() const { return _pixelBuffer.GetHeight(); }
        bool IsColorKeyed () const;
        bool SetColorKey  ( unsigned long color );
        void SetNoColorKey();
        void GetProperties( Surf::Properties *prop ) const;
        bool SetProperties( const Surf::Properties &prop ) { assert(0); return 0; }
    
        bool SetClipper( const RectList &region ) { return false; }
        RectList GetClipper( const Rect &crop ) const { return RectList(); }
    
        void SetConstantAlpha( int alpha ) { assert(0); }
        int  GetConstantAlpha() const { return 255; }
    
        bool operator == ( const Surface& s );
        bool operator != ( const Surface& s );
        
        void * GetExtFun( const std::string &name );
        void IsDirty( int *dirty ) { *dirty = _dirty; }

    public:
        GLuint  _colorKey;
        bool    _useColorKey;
        bool    _useAlpha;

        PixelBuffer  _pixelBuffer;
        mutable bool _dirty;
    };


}; //Useless


#endif//__INCLUED_USELESS_GL_TEXTURE_SURFACE_H__
