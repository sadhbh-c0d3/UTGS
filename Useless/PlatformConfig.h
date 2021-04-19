#ifndef __INCLUDED_USELESS_PLATFORM_CONFIG_H__
#define __INCLUDED_USELESS_PLATFORM_CONFIG_H__

#ifndef USELESS_STATICLIB
#   ifdef USELESS_EXPORTS
#       define USELESS_API __declspec(dllexport)
#   else
#       define USELESS_API __declspec(dllimport)
#   endif
#   ifdef USELESS_RESOURCES_EXPORTS
#       define USELESS_RESOURCES_API __declspec(dllexport)
#   else
#       define USELESS_RESOURCES_API __declspec(dllimport)
#   endif
#else
#   define USELESS_API
#   define USELESS_RESOURCES_API
#endif



#define __USE_DIRECT_SOUND__
#define __USE_DIRECT_SHOW__


#endif//__INCLUDED_USELESS_PLATFORM_CONFIG_H__
