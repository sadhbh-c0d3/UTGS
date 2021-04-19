#ifndef __INCLUDED_GVM_CONTEXT_H__
#define __INCLUDED_GVM_CONTEXT_H__

#include <assert.h>
#include <vector>
#include "Useless/Graphic/Linear.h"
#include "Dynamo/bits.h"

namespace Useless {

    namespace GVM {

        struct Message;

        struct USELESS_API Texture : virtual Dynamo::IInterface
        {
            enum Color  { /*3, 4, RGB=0x1907, RGBA=0x1908,*/ RGB8=0x8051, RGBA8=0x8058, RGB16=0x8054, RGBA16=0x805B, ALPHA8=0x803C };
            enum Format { RGB=0x1907, RGBA=0x1908, BGR=0x80E0, BGRA=0x80E1, ALPHA=0x1906 };
            enum Type   { UBYTE=0x1401, USHORT=0x1403, UINT=0x1405, FLOAT=0x1406 };
            enum Filter { NEAREST=0x2600, LINEAR=0x2601,
                          NEAREST_MIPMAP_NEAREST=0x2700, LINEAR_MIPMAP_NEAREST=0x2701,
                          NEAREST_MIPMAP_LINEAR=0x2702, LINEAR_MIPMAP_LINEAR=0x2703 };
            enum Wrap   { CLAMP=0x2900, REPEAT=0x2901 };

            virtual ~Texture()
            {
            }

            virtual void LoadSubPixels( void *data, int level, int x, int y, int width, int height, int rowLen, int pixType, int pixFmt ) = 0;
            virtual void SetFilter( int minFlt, int magFlt ) = 0;
            virtual void SetWrap( int wrapS, int wrapT ) = 0;
            virtual void SetPriority( float priority ) = 0;

            virtual Message BindMsg() = 0;

            virtual int GetWidth() = 0;
            virtual int GetHeight() = 0;
        };

        struct USELESS_API Context
        {
            virtual ~Context()
            {
            }

            // Window properites
            virtual void GetViewport ( int *rect ) = 0;
            virtual void InitViewport( int x, int y, int w, int h ) = 0;
            virtual void SetViewport ( int x, int y, int w, int h ) = 0;

            // Camera properties
            virtual void SetOrtho2D    ( double left, double right, double bottom, double top ) = 0; 
            virtual void SetPerspective( double fov, double aspect, double znear, double zfar ) = 0;
            virtual void SetFrustum    ( double left, double right, double bottom, double top, double znear, double zfar ) = 0;
            virtual void SetOrtho      ( double left, double right, double bottom, double top, double znear, double zfar ) = 0;

            virtual Texture * CreateTexture( int width, int height, int color ) = 0;

            // Dispatch message
            virtual void Invoke( const Message &msg ) = 0;
        };

    };// namespace GVM
};// namespace Useless
#endif//__INCLUDED_GVM_CONTEXT_H__
