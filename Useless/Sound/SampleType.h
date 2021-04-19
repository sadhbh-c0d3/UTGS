#ifndef __INCLUDED__USELESS_SAMPLE_TYPE_H__
#define __INCLUDED__USELESS_SAMPLE_TYPE_H__

#pragma warning(disable:4786)

namespace Useless {

namespace Snd {

//Unsigned 8-bit signal
struct Unsigned8
{
    Unsigned8(): _value(0) {}
    Unsigned8( unsigned char value ): _value(value) {}
    Unsigned8( unsigned int value ): _value( value/256 ) {}
    Unsigned8( char value ) { int v = value; v+=128; _value=v; }
    Unsigned8( int value ): _value( ( (value/256)+128 ) ) {}
    Unsigned8( float f ) { f*=127.5; f+=128; _value = (unsigned char)f; }

    operator unsigned char() const { return _value; }
    unsigned char Get() { return _value; }

    unsigned char _value;
};

//Signed 16-bit signal
struct Signed16
{
    Signed16(): _value(0) {}
    Signed16( int value ): _value(value) {}
    Signed16( unsigned int value ) { int v=value; _value = (short)(int)(v-(1<<15)); }
    Signed16( char value ) { int v = value; _value = v*256; }
    Signed16( unsigned char value ) { int v = value; _value = (v-128)*256; }
    Signed16( float f ) { f*=((float)(1<<15)); _value = (short)f; }

    operator short() const { return _value; }
    short Get() { return _value; }

    short _value;
};

//Float 32-bit signal
struct Float32
{
    Float32(): _value(0) {}
    Float32( float x ): _value(x) {}
    Float32( unsigned char x) { float v=x; v-=127.5f; _value = v/127.5f; }
    Float32( char x)          { float v=x; _value = v/127.5f; }
    Float32( unsigned int x ) { _value = ((float)x)/((float)(1<<15))-1.0f; }
    Float32( int x )          { _value = ((float)x)/((float)(1<<15)); }

    operator float() { return _value; }
    float Get() { return _value; }

    float _value;
};

void SampleTypeTest();

};//namespace Snd
};//namespace Useless
#endif//__INCLUDED__USELESS_SAMPLE_TYPE_H__
