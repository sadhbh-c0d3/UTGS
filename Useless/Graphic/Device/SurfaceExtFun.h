#ifndef __INCLUDED_SURF_EXT_FUN_H__
#define __INCLUDED_SURF_EXT_FUN_H__

namespace Useless { namespace SurfExt {

    typedef void (*PfAlphaFun)(Surface *p, int f, bool e);
    typedef void (*PfDepthFun)(Surface *p, int f, bool e);
    typedef void (*PfBlendFun)(Surface *p, int sf, int df, bool e);
    typedef void (*PfConstColor)(Surface *p, int r, int g, int b);
    typedef void (*PfConstMatrix)(Surface *p, float *matrix);

    template< class _A, class _S, void (_S::*_F)( _A ) >
        struct Thunk1
        {
            static void apply( Surface *p, _A s )
            {
                (((_S*)p)->*_F)( s );
            }
        };

    template< class _A, class _B, class _S, void (_S::*_F)( _A, _B ) >
        struct Thunk2
        {
            static void apply( Surface *p, _A s, _B d )
            {
                (((_S*)p)->*_F)( s, d );
            }
        };

    template< class _A, class _B, class _C, class _S, void (_S::*_F)( _A, _B, _C ) >
        struct Thunk3
        {
            static void apply( Surface *p, _A s, _B d, _C w )
            {
                (((_S*)p)->*_F)( s, d, w );
            }
        };
    
    template< class _A, class _B, class _C, class _D, class _S, void (_S::*_F)( _A, _B, _C, _D ) >
        struct Thunk4
        {
            static void apply( Surface *p, _A s, _B d, _C w, _D v )
            {
                (((_S*)p)->*_F)( s, d, w, v );
            }
        };


};//SurfExt
}; //Useless

#endif//__INCLUDED_SURF_EXT_FUN_H__
