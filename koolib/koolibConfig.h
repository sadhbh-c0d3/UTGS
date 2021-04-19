#ifndef __INCLUDED_KOOLIB_CONFIG_H__
#define __INCLUDED_KOOLIB_CONFIG_H__

#include "utgs-config.h"

#ifndef KOOLIB_STATICLIB
#   ifdef KOOLIB_EXPORTS
#       define KOOLIB_API __declspec(dllexport)
#   else
#       define KOOLIB_API __declspec(dllimport)
#   endif
#   ifdef KOOLIB_BIND_EXPORTS
#       define KOOLIB_BIND_API __declspec(dllexport)
#   else
#       define KOOLIB_BIND_API __declspec(dllimport)
#   endif
#else
#   define KOOLIB_API
#   define KOOLIB_BIND_API
#endif

#define KOOLIB_NOVTABLE __declspec(novtable)

#include "koolib/TemplateConfig.h"

#endif//__INCLUDED_KOOLIB_CONFIG_H__
