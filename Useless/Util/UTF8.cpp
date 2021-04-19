#include "UselessPch.h"

#include "Useless/Util/UTF8.h"

#include <locale>
#include <sstream>
#include <algorithm>


#ifdef USELESS_HAS_BOOST_SERIALIZATION

#   include <boost/version.hpp>
#   if BOOST_VERSION < 103300
#       include <boost/utf8_codecvt_facet.hpp>  //< use for boost 1.31
	    std::locale utf8_loc( std::locale("C"), new utf8_codecvt_facet< wchar_t, char > );  //< use for boost 1.31
#   else
#	    include <boost/archive/detail/utf8_codecvt_facet.hpp> //< use for boost 1.33
        std::locale utf8_loc( std::locale("C"), new boost::archive::detail::utf8_codecvt_facet );  //< use for boost 1.33
#   endif
        
namespace Useless {

	typedef std::codecvt_byname< wchar_t, char, std::mbstate_t > CodeCvt;

    void ReadUtf8( std::wstring &wstr, const std::string &strIn )
    {
#   ifdef WIN32
        std::mbstate_t state(0);
#   else
        std::mbstate_t state;
#   endif
        const CodeCvt &codeCvt = std::use_facet< CodeCvt >( utf8_loc );

        const char   *buffer_in = &strIn[0];
        size_t  in_size = strIn.size();
        const char *const_in_next = buffer_in;

        std::codecvt_base::result result = std::codecvt_base::partial;

        while ( std::codecvt_base::partial == result )
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

            in_size -= const_in_next - buffer_in;
            buffer_in = const_in_next;
        }
    }


    void WriteUtf8( std::string &strOut, const std::wstring &wstr )
    {
#   ifdef WIN32
        std::mbstate_t state(0);
#   else
        std::mbstate_t state;
#   endif
        const wchar_t *const_in_next = 0;
        char *out_next = 0;

        const CodeCvt &codeCvt = std::use_facet< CodeCvt >( utf8_loc );

        std::codecvt_base::result result = std::codecvt_base::partial;

        for ( int index = 0, Size = wstr.size(); index < Size; ++index )
        {
            wchar_t buffer_in = wstr[ index ];
            char    buffer_out[3] = {0,};

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
    }
};//Useless

#else

namespace Useless {

    //TODO: Get Boost.Serialization.Utf8 library and set USELESS_HAS_BOOST_SERIALIZATION flag.

    void ReadUtf8( std::wstring &wstr, const std::string &strIn )
    {
        std::copy( strIn.begin(), strIn.end(), std::inserter( wstr, wstr.end()));
    }


    void WriteUtf8( std::string &strOut, const std::wstring &wstr )
    {
        std::copy( wstr.begin(), wstr.end(), std::inserter( strOut, strOut.end()));
    }

};//Useless
#endif

    
