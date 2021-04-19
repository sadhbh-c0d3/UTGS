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
last mod: $Id: $

 ********************************************************************/

#ifndef __INCLUDED_KOOLIB_TEXT_UTF8_H__
#define __INCLUDED_KOOLIB_TEXT_UTF8_H__

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "koolib/koolibConfig.h"

void ReadUtf8( std::wstring &, const std::string & );
void WriteUtf8( std::string &, const std::wstring & );

struct KOOLIB_API TextUtf8 : std::wstring
{
    typedef wchar_t CharType;
    typedef std::basic_stringstream< CharType > Stream;

    TextUtf8()
	{
	}
    TextUtf8( const wchar_t *value ): std::wstring(value)
	{
	}
    TextUtf8( const std::wstring &value ): std::wstring(value)
	{
	}
    TextUtf8( const std::string &value )
    {
        ReadUtf8( *this, value );
    }
	~TextUtf8()
	{
	}

    operator std::string() const { return as_string(); }

    std::string as_string() const
    {
        std::string value;
        WriteUtf8( value, *this );
        return value;
    }
};

inline TextUtf8 WidenText( const std::string &s )
{
    TextUtf8 r;
    std::copy( s.begin(), s.end(), std::inserter( r, r.end() ));
    return r;
}


inline TextUtf8 WidenText( const char *s )
{
    TextUtf8 r;
    size_t l = strlen( s );
    std::copy( s, s + l, std::inserter( r, r.end() ));
    return r;
}

struct TextAnsi : std::wstring
{
    TextAnsi()
	{
	}
    TextAnsi( const wchar_t *value ): std::wstring( value )
	{
	}
    TextAnsi( const std::wstring &value ): std::wstring( value )
	{
	}
    TextAnsi( const std::string &value ): std::wstring( WidenText( value ))
	{
	}
    TextAnsi( const char *value ): std::wstring( WidenText( value ))
	{
	}
	~TextAnsi()
	{
	}
    
    operator std::string() const { return as_string(); }

    static char op_convert(wchar_t w)
    {
        return (char)w;
    }

    std::string as_string() const
    {
        std::string value;
        std::transform( begin(), end(), std::inserter( value, value.end()), &op_convert);
        return value;
    }

};

#endif//__INCLUDED_KOOLIB_TEXT_UTF8_H__
