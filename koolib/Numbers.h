/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE KooLAS C++ Template & Class Library     *
 * CALLED KooLib SOURCE CODE.                                       *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE KooLib SOURCE CODE IS (C) COPYRIGHT 2004                     *
 * by the Sadhbh Code (https://github.com/sadhbh-c0d3)              *
 *                                                                  *
 ********************************************************************

function:
last mod: $Id: Numbers.h,v 1.4 2004/09/14 18:25:53 koolas Exp $

 ********************************************************************/


#ifndef __INCLUDED_NUMBER_H__
#define __INCLUDED_NUMBER_H__

#include <cmath>
namespace Numbers {

    template< const int _Bits = 8, typename _Int = int > struct Fixed {

        enum {
            BITS = _Bits,
            MULT = 1 << _Bits,
            MASK = 0xFFFFFFFF >> (32 - _Bits)
        };

        typedef _Int IntType;

        IntType _value;

        // --== convertion from int/float ==--

        Fixed()
        {
            _value = 0;
        }

        template< const int B_, typename I_ > Fixed( const Fixed< B_, I_ > &other )        
        {            
            _value = Fixed( other.GetInteger(), other.GetFraction() )._value;
        }

        Fixed( const Fixed< BITS, IntType > &other )
        {
            _value = other._value;
        }

        Fixed( IntType I )
        {
            _value = I * MULT;
        }

        Fixed( IntType I, IntType F )
        {
            // I assume ( (I >= 0 && F >=0) || ( I < 0 && F < 0 ));
            _value = ( I * MULT ) + F;
        }

        Fixed( float f )
        {
            IntType I = (IntType)f;
            IntType F = (IntType)((f - (float)I) * 256.f);
            _value = Fixed( I, F )._value;
        }

        Fixed( double f )
        {
            IntType I = (IntType)f;
            IntType F = (IntType)((f - (double)I) * 256.0);
            _value = Fixed( I, F )._value;
        }

        template< class _Y > Fixed New( _Y a ) const
        {
            return Fixed( a );
        }

        // --== convertion to int/float ==--

        IntType GetInteger() const
        {
            return (_value / MULT);
        }

        IntType GetFraction() const
        {
            return (_value - (GetInteger() * MULT));
        }

        IntType Ceil() const
        {
            IntType I = GetInteger();
            IntType F = GetFraction();
            return ( F > 0 ? I + 1 : I );
        }

        IntType Floor() const
        {
            IntType I = GetInteger();
            IntType F = GetFraction();
            return ( F < 0 ? I - 1 : I );
        }

        float GetFloat() const
        {
            return ((float)_value / (float)MULT);
        }

        double GetDouble() const
        {
            return ((double)_value / (double)MULT);
        }


        // --== arithmetic operators ==--

        template< class _Y > Fixed & operator += ( _Y c )
        {
            _value += New( c )._value;
            return (*this);
        }

        template< class _Y > Fixed & operator -= ( _Y c )
        {
            _value -= New( c )._value;
            return (*this);
        }

        template< class _Y > Fixed & operator *= ( _Y c )
        {
            _value = (_value * New( c )._value) / MULT;
            return (*this);
        }

        template< class _Y > Fixed & operator /= ( _Y c )
        {
            _value = (_value * MULT) / New( c )._value;
            return (*this);
        }

        bool operator ! () const
        {
            return !_value;
        }

        Fixed operator -() const
        {
            Fixed f;
            f._value = -_value;
            return f;
        }

        // --== comparison ==--   

        bool operator == ( Fixed c ) const
        {
            return _value == c._value;
        }

        bool operator !=  ( Fixed c ) const
        {
            return _value != c._value;
        }

        bool operator <  ( Fixed c ) const
        {
            return _value < c._value;
        }

        bool operator <= ( Fixed c ) const
        {
            return _value <= c._value;
        }

        bool operator >  ( Fixed c ) const
        {
            return _value > c._value;
        }

        bool operator >=  ( Fixed c ) const
        {
            return _value >= c._value;
        }

        operator bool () const
        {
            return (bool)_value;
        }

        operator int () const
        {
            return GetInteger();
        }

        operator float () const
        {
            return GetFloat();
        }

        operator double () const
        {
            return GetDouble();
        }
    };

    // -- comparison operators ==--

    template< const int _Bits, typename _Int, typename _Y > bool operator == ( _Y a, Fixed< _Bits, _Int > b )
    {
        return b.New( a ) == b;
    }

    template< const int _Bits, typename _Int, typename _Y > bool operator == ( Fixed< _Bits, _Int > b, _Y a )
    {
        return  b == b.New( a );
    }

    template< const int _Bits, typename _Int, typename _Y > bool operator != ( _Y a, Fixed< _Bits, _Int > b )
    {
        return b.New( a ) != b;
    }

    template< const int _Bits, typename _Int, typename _Y > bool operator != ( Fixed< _Bits, _Int > b, _Y a )
    {
        return  b != b.New( a );
    }

    template< const int _Bits, typename _Int, typename _Y > bool operator < ( _Y a, Fixed< _Bits, _Int > b )
    {
        return b.New( a ) < b;
    }

    template< const int _Bits, typename _Int, typename _Y > bool operator < ( Fixed< _Bits, _Int > b, _Y a )
    {
        return  b < b.New( a );
    }

    template< const int _Bits, typename _Int, typename _Y > bool operator <= ( _Y a, Fixed< _Bits, _Int > b )
    {
        return b.New( a ) <= b;
    }

    template< const int _Bits, typename _Int, typename _Y > bool operator <= ( Fixed< _Bits, _Int > b, _Y a )
    {
        return  b <= b.New( a );
    }

    template< const int _Bits, typename _Int, typename _Y > bool operator > ( _Y a, Fixed< _Bits, _Int > b )
    {
        return b.New( a ) > b;
    }

    template< const int _Bits, typename _Int, typename _Y > bool operator > ( Fixed< _Bits, _Int > b, _Y a )
    {
        return  b > b.New( a );
    }

    template< const int _Bits, typename _Int, typename _Y > bool operator >= ( _Y a, Fixed< _Bits, _Int > b )
    {
        return b.New( a ) >= b;
    }

    template< const int _Bits, typename _Int, typename _Y > bool operator >= ( Fixed< _Bits, _Int > b, _Y a )
    {
        return  b >= b.New( a );
    }

    // --== arithmetic operators ==--

    template< const int _Bits, typename _Int, typename _Y > Fixed< _Bits, _Int > operator + ( Fixed< _Bits, _Int > a, _Y b )
    {
        return (a += a.New( b ));
    }

    template< const int _Bits, typename _Int, typename _Y > Fixed< _Bits, _Int > operator + ( _Y a, Fixed< _Bits, _Int > b )
    {
        return (b.New( a ) += b);
    }

    template< const int _Bits, typename _Int, typename _Y > Fixed< _Bits, _Int > operator - ( Fixed< _Bits, _Int > a, _Y b )
    {
        return (a -= a.New( b ));
    }

    template< const int _Bits, typename _Int, typename _Y > Fixed< _Bits, _Int > operator - ( _Y a, Fixed< _Bits, _Int > b )
    {
        return (b.New( a ) -= b);
    }

    template< const int _Bits, typename _Int, typename _Y > Fixed< _Bits, _Int > operator * ( Fixed< _Bits, _Int > a, _Y b )
    {
        return (a *= a.New( b ));
    }

    template< const int _Bits, typename _Int, typename _Y > Fixed< _Bits, _Int > operator * ( _Y a, Fixed< _Bits, _Int > b )
    {
        return (b.New( a ) *= b);
    }

    template< const int _Bits, typename _Int, typename _Y > Fixed< _Bits, _Int > operator / ( Fixed< _Bits, _Int > a, _Y b )
    {
        return (a /= a.New( b ));
    }

    template< const int _Bits, typename _Int, typename _Y > Fixed< _Bits, _Int > operator / ( _Y a, Fixed< _Bits, _Int > b )
    {
        return (b.New( a ) /= b);
    }


    // Default fixed point number
    typedef Fixed< 8, int > FPNum;

#ifdef WIN32
    typedef Fixed< 8, __int32 > FPNum32;
    typedef Fixed< 16, __int64 > FPNum64;
#else
    typedef Fixed< 8, int32_t > FPNum32;
    typedef Fixed< 16, int64_t > FPNum64;
#endif

    template< class _F > struct PrecissionTraits
    {
        typedef FPNum64 Better;
    };

};//Numbers


#endif//__INCLUDED_NUMBER_H__

