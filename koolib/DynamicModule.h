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

#ifndef __INCLUDED_KOOLIB_DYNAMIC_MODULE_H__
#define __INCLUDED_KOOLIB_DYNAMIC_MODULE_H__

#include <string>
#include <iterator>
#include <Useless/Error/ErrorMessage.h>

template< class _Module >
struct DynamicModuleBase : _Module
{
    DynamicModuleBase( const std::string &name )
        : _Module( name )
        {}

    template< class _PF >
    void AssignFuncPtr( const std::string &func, _PF & pf )
    {
        pf = (_PF)GetFuncPtr( func );
    }
};

#ifdef WIN32

#include <windows.h>


struct DynamicModuleWIN32
{
    typedef HMODULE ModuleHandle;
    typedef FARPROC FuncPtr;

    DynamicModuleWIN32( const std::string &fileName )
    {
        assert( fileName.length() < MAX_PATH );
		std::basic_string< TCHAR > moduleName;
		std::copy( fileName.begin(), fileName.end()
			, std::inserter( moduleName, moduleName.end()));        
        _module = ::LoadLibrary( moduleName.c_str() );
        C_verifyf( DynamicModuleWIN32, 0, _module,
                ("Failed to load module :", fileName.c_str() ));
        C_xlogf( DynamicModuleWIN32, 0, true,
                ("Loaded module :", fileName.c_str(), "at :", (void*)_module ));
    }

    ~DynamicModuleWIN32()
    {
        ::FreeLibrary( _module );
    }

    FuncPtr GetFuncPtr( const std::string &func )
    {
		std::basic_string< CHAR > funcName;
		std::copy( func.begin(), func.end(), std::inserter( funcName, funcName.end()));
		FuncPtr pf = ::GetProcAddress( _module, funcName.c_str() );
        C_verifyf( DynamicModuleWIN32, 2, 0 != pf,
                ("Failed to obtain pointer to function :", func.c_str() ));
        return pf;
    }

    ModuleHandle get() { return _module; }

    ModuleHandle _module;
};

typedef DynamicModuleBase< DynamicModuleWIN32 > DynamicModule;

#else
#include <dlfcn.h>

struct DynamicModuleUNIX
{
    typedef void *ModuleHandle;
    typedef void *FuncPtr;

    DynamicModuleUNIX( const std::string &fileName )
    {
        _module = dlopen( fileName.c_str(), RTD_LAZY );        
        C_verify( DynamicModuleUNIX, 0, _module,
                ("Failed to load module :", fileName.c_Str(), "error :", dlerror() ));
        C_xlogf( DynamicModuleUNIX, 0, true,
                ("Loaded module :", fileName.c_str(), "at :", (void*)_module ));
    }

    ~DynamicModuleUNIX()
    {
        dlclose( _module );
    }

    FuncPtr GetFuncPtr( const std::string &func )
    {
        FuncPtr pf = dlsym( _module, func.c_str() );
        char *error = dlerror();
        C_verifyf( DynamicModuleUNIX, 2, !error,
                ("Failed to obtain pointer to function :", func.c_str(), "error :", error ));
        return pf;
    }
    
    ModuleHandle get() { return _module; }

    ModuleHandle _module;
};

typedef DynamicModuleBase< DynamicModuleUNIX > DynamicModule;

#endif

#endif//__INCLUDED_KOOLIB_DYNAMIC_MODULE_H__

