#ifndef __INCLUDED__USELESS__PIXEL_PTR_H__
#define __INCLUDED__USELESS__PIXEL_PTR_H__
#include "Useless/Graphic/Pixel/Pixel.h"

namespace Useless {

/*! Generic Pixel Pointer
--------------------------------------------------------------*/
template< class TPixel >
class GenericPixelPtr
{
public:
    typedef TPixel PixelType;
    GenericPixelPtr( void *ptr, int pitch ): _p((PixelType*)ptr), _s(pitch) {}

    PixelType& operator *() { return *_p; }
    operator PixelType *() { return _p; }

         void Write( const PixelType &pixel ) { *_p=pixel; }
    PixelType Read () const { return *_p; }

    GenericPixelPtr& operator +=( Pos d )
    {
        ((char*&)_p) += _s*d.y; _p += d.x;
        return *this;
    }

    GenericPixelPtr& operator +=( int pixels )
    {
        _p += pixels;
        return *this;
    }

    GenericPixelPtr& operator ++() { ++_p; return *this; }
    GenericPixelPtr& operator --() { --_p; return *this; }

    GenericPixelPtr& Next() 
    {
        ((char*&)_p) += _s; return *this; 
    }

    GenericPixelPtr& Prev() 
    {
        ((char*&)_p) -= _s; return *this; 
    }

	GenericPixelPtr operator + ( const Pos &p )
	{
		GenericPixelPtr newPtr(*this);
		return newPtr += p;
	}

    operator unsigned char*() { return (unsigned char*)_p; }
    int GetPitch() { return _s; }

private:
    PixelType *_p;
    int        _s;
};



/*! Pixel Pointer with Clipper
--------------------------------------------------------------*/
template< class TPixel >
class ClippedPixelPtr
{
public:
    typedef TPixel PixelType;
    ClippedPixelPtr( void *ptr, int pitch, const Rect &clipper )
        : _p((PixelType*)ptr), _s(pitch)
    {
        _l = -clipper.GetX1();    _r = clipper.GetX2();
        _u = -clipper.GetY1();    _d = clipper.GetY2();
        _dummy = MakeRGB(0,0,0);
        _read_cnt = 0;
    }

    PixelType& operator *() { return (OK())? *_p : _dummy; }
    operator PixelType *() { return (OK())? _p : &_dummy; }

         void Write( PixelType pixel ) { if (OK()) *_p=pixel; }
    PixelType Read () const
    {
        return (OK())? *_p : _dummy;
        //if ( _read_cnt%2 == 0 )
        //if (OK()) { _dummy=*_p; ++_read_cnt; }
        //return _dummy; 
    }

    ClippedPixelPtr& operator +=( Pos d )
    {
        ((char*&)_p) += _s*d.y; _p += d.x; _l+=d.x; _r-=d.x; _u+=d.y; _d-=d.y;
        return *this;
    }

    ClippedPixelPtr& operator +=( int pixels )
    {
        _p += pixels; _l += pixels; _r -= pixels;
        return *this;
    }

    ClippedPixelPtr& operator ++() { ++_p; ++_l; --_r; return *this; }
    ClippedPixelPtr& operator --() { --_p; --_l; ++_r; return *this; }

    ClippedPixelPtr& Next() 
    {
        ((char*&)_p) += _s; ++_u; --_d; return *this; 
    }

    ClippedPixelPtr& Prev() 
    {
        ((char*&)_p) -= _s; --_u; ++_d; return *this; 
    }

    bool OK() const
    {
        return _l>=0 && _r>0 && _u>=0 && _d>0;
    }

    operator GenericPixelPtr<PixelType>() 
    {
        return GenericPixelPtr<PixelType>(_p, _s); 
    }

	ClippedPixelPtr operator + ( const Pos &p )
	{
		ClippedPixelPtr newPtr(*this);
		return newPtr += p;
	}

    operator unsigned char*() { return (unsigned char*)_p; }
    int GetPitch() { return _s; }
    Rect GetClipper() const
    {
        return Rect( -_l,-_u,_r+_l,_d+_u);
    }

private:
    PixelType *_p;
    int        _s;

    int        _l;
    int        _r;
    int        _u;
    int        _d;
    mutable PixelType  _dummy;
    mutable int        _read_cnt;
};



};//namespace Useless

#endif//__INCLUDED__USELESS__PIXEL_PTR_H__
