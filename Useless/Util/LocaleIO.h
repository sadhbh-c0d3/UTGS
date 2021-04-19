#ifndef __INCLUDED_USELESS_LOCALE_IO_H__
#define __INCLUDED_USELESS_LOCALE_IO_H__

#include <locale>
#include <map>
#include <sstream>

#include "Useless/LocaleConfig.h"

namespace Useless {

/**
 * Read( ref_output_string &,
 *       input_string,
 *       ref_locale & )
 *
 * Read input_string using ref_locale into ref_output_string.
 * input_string would be the localized one, for example encoded in cp1250,
 * ref_output_string would probably be wstring, an internal UCS-2.
 */
template< class CharType, class Byte > std::basic_string< CharType > &
Read( std::basic_string< CharType > &wstr,
        const std::basic_string< Byte > &strIn,
        std::locale &loc )
{
#ifdef WIN32
    std::mbstate_t state(0);
#else
    std::mbstate_t state;
#endif
    typedef std::codecvt_byname< CharType, Byte, std::mbstate_t > CodeCvt;
    const CodeCvt &codeCvt = std::use_facet< CodeCvt >( loc );

    const char   *buffer_in = &strIn[0];
    size_t  in_size = strIn.size();
    const char *const_in_next = buffer_in;

    std::codecvt_base::result result = std::codecvt_base::partial;

    while ( true /*std::codecvt_base::partial == result*/ )
    {
        wchar_t  buffer_out;
        wchar_t *out_next = &buffer_out;

        result = codeCvt.in( state,
                buffer_in,
                buffer_in + in_size,
                const_in_next,

                &buffer_out,
                &buffer_out + 1,
                out_next
                );

        if ( &buffer_out + 1 == out_next )
        {
            wstr += buffer_out;
        }
        else
        {
            break;
        }

        in_size -= const_in_next - buffer_in;
        buffer_in = const_in_next;
    }
    return wstr;
}


/**
 * Write( ref_output_string &,
 *       input_string,
 *       ref_locale & )
 *
 * Write input_string into ref_output_string using ref_locale.
 * input_string would probably be wstring, an internal UCS-2,
 * ref_output_string would be the localized one, for example encoded in iso-8859-2.
 */
template< class CharType, class Byte > std::basic_string< Byte > &
Write( std::basic_string< Byte > &strOut,
        const std::basic_string< CharType > &wstr,
        std::locale &loc )
{
#ifdef WIN32
    std::mbstate_t state(0);
#else
    std::mbstate_t state;
#endif
    const CharType *const_in_next = 0;
    Byte           *out_next = 0;

    typedef std::codecvt_byname< CharType, Byte, std::mbstate_t > CodeCvt;
    const CodeCvt &codeCvt = std::use_facet< CodeCvt >( loc );

    std::codecvt_base::result result = std::codecvt_base::partial;

    for ( int index = 0, Size = wstr.size(); index < Size; ++index )
    {
        CharType buffer_in = wstr[ index ];
        Byte     buffer_out[ sizeof(CharType) + 1 ] = {0,};

        result = codeCvt.out( state,
                &buffer_in,
                &buffer_in + 1,
                const_in_next,

                buffer_out,
                buffer_out + 2,
                out_next
                );

        if ( buffer_out != out_next )
        {
            strOut += buffer_out;
        }
    }

    return strOut;
}

typedef std::codecvt_byname< char, char, std::mbstate_t > Charslator;
typedef std::codecvt_byname< wchar_t, char, std::mbstate_t > WideCharslator;

}; //Useless

#endif//__INCLUDED_USELESS_LOCALE_IO_H__
