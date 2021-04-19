#ifndef __INCLUDED_USELESS_LOCALE_CONFIG_H__
#define __INCLUDED_USELESS_LOCALE_CONFIG_H__

// utf8_codecvt<> is defined in boost.serialization library
#ifdef USELESS_HAS_BOOST_SERIALIZATION
#   define __USE_BOOST_UTF8_CODECVT__
#else
//@TODO: Get boost.serialization library and set USELESS_HAS_BOOST_SERIALIZATION
#   pragma message("Get boost.serialization library and set USELESS_HAS_BOOST_SERIALIZATION")
#endif

#endif//__INCLUDED_USELESS_LOCALE_CONFIG_H__
