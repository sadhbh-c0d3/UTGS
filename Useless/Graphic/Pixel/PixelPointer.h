#ifndef __INCLUDED_USELESS_PIXEL_POINTER_H__
#define __INCLUDED_USELESS_PIXEL_POINTER_H__

#include "Useless/Graphic/Pixel/Pixel.h"

namespace Useless {


/*! Flexible pixel pointer - compatibility file
 * !!! Please, look at PixelPtr.h and use structures defined there !!!
-----------------------------------------------------------------------------------------*/
template< class TPixel > 
class PixelPointer
{
public:
    typedef TPixel PixelType;

    PixelPointer(): _pointer(0), _pixel_ok(0) {}
    
    PixelPointer(PixelType *ptr, int pitch, const Rect &clp ): _pointer(ptr), _pitch(pitch), 
    _lt( -clp.GetX1() ), _rt( clp.GetX2() ), _up( -clp.GetY1() ), _dn( clp.GetY2() )
    {
        _pixel_ok = _pointer && OK();
    }

    PixelPointer(const PixelPointer &pp, int dx, int dy): 
        _pitch(pp._pitch), _lt(pp._lt+dx), _rt(pp._rt-dx), _up(pp._up+dy), _dn(pp._dn-dy)
    {
        _pixel_ok = OK();
        _pointer = (PixelType*)( (BytePixel*)pp._pointer + dy*_pitch ) + dx;
    }
    
    operator PixelType* ()  { return  _pixel_ok ? _pointer : &_dummy_pixel;}
    PixelType* operator->() { return  _pixel_ok ? _pointer : &_dummy_pixel;}

    typedef unsigned char BytePixel;

    PixelPointer Offset( int dx, int dy)
    {
        return PixelPointer(*this, dx, dy);
    }
    PixelPointer operator() ( int dx, int dy)
    {
        return PixelPointer(*this, dx, dy);
    }

    PixelPointer& PrevPixel()
    {
        Dx(-1); --_pointer; return *this;
    }
    PixelPointer& NextPixel()
    {
        Dx(1); ++_pointer; return *this;
    }
    PixelPointer& PrevLine()
    {
        Dy(-1); (BytePixel *&)_pointer -= _pitch; return *this;
    }
    PixelPointer& NextLine()
    {
        Dy(1); (BytePixel *&)_pointer += _pitch; return *this;
    }

    PixelPointer& MoveNPixels( int dx)
    {
        Dx(dx); _pointer+=dx; return *this;
    }
    
    PixelPointer& MoveNLines( int dy)
    {
        Dy(dy); (BytePixel *&)_pointer += dy*_pitch; return *this;
    }

    bool OK    ()           const { return _lt>=0&&_rt>0&&_up>=0&&_dn>0; }
    void DxDy  ( int dx, int dy ) {   _lt+=dx; _rt-=dx;  _up+=dy; _dn-=dy; _pixel_ok = OK(); }
    void Dx    ( int dx )         {   _lt+=dx; _rt-=dx; _pixel_ok = OK(); }
    void Dy    ( int dy )         {   _up+=dy; _dn-=dy; _pixel_ok = OK(); }

private:
    PixelType *_pointer;
          int  _pitch;

          int  _lt;
          int  _rt;
          int  _up;
          int  _dn;
         bool  _pixel_ok;

    PixelType  _dummy_pixel;
};


};//namespace Useless
#endif//__INCLUDED_USELESS_PIXEL_POINTER_H__
