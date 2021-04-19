#include "UselessPch.h"

#include "LangSelector.h"

#ifdef __USE_BOOST_UTF8_CODECVT__
#   include <boost/version.hpp>
#   if BOOST_VERSION < 103300
#       include <boost/utf8_codecvt_facet.hpp>  //< use for boost 1.31
#   else
#	    include <boost/archive/detail/utf8_codecvt_facet.hpp> //< use for boost 1.33
#   endif
#endif

namespace Useless {

INIT_SINGLETON( LangSelector, USELESS_API );

__LangSelector::__LangSelector()
{
    const char *known_country_codes[] = {
        "america_USA",
        "britain_GBR",
        "china_CHN",
        "czech_CZE",
        "danish_DENMARK",
        "finnish_FINLAND",
        "english_UK"
        "england_GBR",
        "norwegian-bokmal_NORWAY"
        "great britain_GBR",
        "holland_NLD",
        "hong-kong_HKG",
        "new-zealand_NZL",
        "nz_NZL",
        "pr china_CHN",
        "pr-china_CHN",
        "puerto-rico_PRI",
        "slovak_SVK",
        "south africa_ZAF",
        "south korea_KOR",
        "south-africa_ZAF",
        "south-korea_KOR",
        "trinidad & tobogo_TTO",
        "uk_GBR",
        "united-kingdom_GBR",
        "united-states_USA",
        "us_USA"
    };

    const char *localeNames[][2] = {
        {"iso-8859-2", "polish_POLAND.28592"},
        {"cp1250", "polish_POLAND.1250"},
        {"koi8-r", "us_US.20866"},
        {"cp1251", "us_US.1251"},
        {"big-5", "us_US.950"},
        {"gbk", "us_US.936"},
        {"danish", "danish_DENMARK"},
        {"finnish","finnish_FINLAND"},
        {"english","english_UK"},
        {"norwegian-bokmal","norwegian-bokmal_NORWAY"}
    };
    int totalCount = sizeof(localeNames)/(2*sizeof(char*));

    for ( int i=0; i < totalCount; ++i )
    {
        std::locale loc( std::locale("C"), new WideCharslator( localeNames[i][1] ));
        _locales.insert( std::make_pair( localeNames[i][0], loc ));
    }
    
#   ifdef __USE_BOOST_UTF8_CODECVT__
    {
#      if BOOST_VERSION < 103300
		std::locale utf8_loc( std::locale("C"), new utf8_codecvt_facet< wchar_t, char > );  //< use for boost 1.31
#      else
		std::locale utf8_loc( std::locale("C"), new boost::archive::detail::utf8_codecvt_facet );  //< use for boost 1.33
#      endif
        _locales.insert( std::make_pair( "utf-8", utf8_loc ));
    }
#   endif
}

std::locale
__LangSelector::operator [] ( const std::string &localeISOName )
{
    std::string lcISOName;
    std::transform( localeISOName.begin(), localeISOName.end(),
            std::back_inserter( lcISOName ), tolower
            );
    
    std::map< std::string, std::locale >::const_iterator
        foundLocale = _locales.find( lcISOName );

    if ( foundLocale != _locales.end() )    
    {
        return (*foundLocale).second;
    }
    else
    {
        try {
            return std::locale( localeISOName.c_str() );
        }
        catch(...)
        {
            throw Error("Unknown locale '%s'\n"
                    "Locale name must be formed: language, language_COUNTRY, or language_COUNTRY.codepage"
                    , localeISOName.c_str() );
        }
    }
}

}; //Useless
