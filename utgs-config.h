//
// Use these 3rd party libraries
//

#define USELESS_HAS_DSHOW
#define USELESS_HAS_VORBIS
//#define USELESS_HAS_CRYPTOPP
#define USELESS_HAS_BOOST_SERIALIZATION
#define USELESS_HAS_LIBPNG

//#define USELESS_DEFAULT_SCREEN_GDI
#define USELESS_DEFAULT_SCREEN_WGL

#ifdef WIN32

#if !defined(UTGS_STATIC_MSVC10) && \
    !defined(UTGS_DLL_MSVC10) && \
    !defined(USELESS_DLL_MSVC10) && \
    !defined(USELESS_RESOURCES_DLL_MSVC10) && \
    !defined(KOOLIB_DLL_MSVC10)

#error Must define exactly one of the following: \
    UTGS_STATIC_MSVC10, UTGS_DLL_MSVC10, \
    USELESS_DLL_MSVC10, USELESS_RESOURCES_DLL_MSVC10, KOOLIB_DLL_MSVC10

#endif


//
// Configurations
//

#ifdef UTGS_STATIC_MSVC10
#   define KOOLIB_STATICLIB
#   define USELESS_STATICLIB
#   define TEMPLATES_MSVC100
#endif

#ifdef UTGS_DLL_MSVC10
#   define TEMPLATES_MSVC100
#endif

#ifdef USELESS_DLL_MSVC10
#   define USELESS_EXPORTS
#   define TEMPLATES_MSVC100
#endif

#ifdef USELESS_RESOURCES_DLL_MSVC10
#   define USELESS_RESOURCES_EXPORTS
#   define TEMPLATES_MSVC100
#endif

#ifdef KOOLIB_DLL_MSVC10
#   define KOOLIB_EXPORTS
#   define TEMPLATES_MSVC100
#endif

#else
#pragma message("WIN32 not defined assuming GCC 2.9x - static")
#   define KOOLIB_STATICLIB
#   define USELESS_STATICLIB
#   define TEMPLATES_GCC29
#endif
