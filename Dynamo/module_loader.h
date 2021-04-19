#ifndef __INCLUDED_DYNAMO_MODULELOADER_H__
#define __INCLUDED_DYNAMO_MODULELOADER_H__

#include <dlfcn.h>
#include <iostream>

namespace Dynamo {

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

    struct DynamicModuleUNIX
    {
        typedef void *ModuleHandle;
        typedef void *FuncPtr;

        DynamicModuleUNIX( const std::string &fileName )
        {
            _module = dlopen( fileName.c_str(), RTLD_LAZY );
            if ( !_module )
            {
                std::cout << "Cannot load module " << fileName << std::endl;
                throw 0L;
            }

            /*C_verify( DynamicModuleUNIX, 0, _module,
              ("Failed to load module :", fileName.c_Str(), "error :", dlerror() ));
              C_xlogf( DynamicModuleUNIX, 0, true,
              ("Loaded module :", fileName.c_str(), "at :", (void*)_module ));*/
        }

        ~DynamicModuleUNIX()
        {
            dlclose( _module );
        }

        FuncPtr GetFuncPtr( const std::string &func )
        {
            FuncPtr pf = dlsym( _module, func.c_str() );
            char *error = dlerror();
            if ( error )
            {
                std::cout << "Failed to obtain pointer to function: " << func << std::endl;
                throw 0L;
            }
            /*C_verifyf( DynamicModuleUNIX, 2, !error,
              ("Failed to obtain pointer to function :", func.c_str(), "error :", error ));*/
            return pf;
        }

        ModuleHandle get() { return _module; }

        ModuleHandle _module;
    };

    typedef DynamicModuleBase< DynamicModuleUNIX > DynamicModule;

};//Dynamo

#endif//__INCLUDED_DYNAMO_MODULELOADER_H__
