#include "Dynamo/classes/CInterfaceProvider.h"
#include "Useless/UselessConfig.h"
#include "koolib/DynamicModule.h"

namespace Dynamo {

struct CModule : CInterface, virtual IModule
{
    CModule( const std::string &name, IInterfaceProvider *provider, IXmlProgram *prog, IXmlScope *scope ): _name( name )
    {
        _module = new DynamicModule( _name );
        _module->AssignFuncPtr("CreateInstance", _CreateInstance);
        _instance = _CreateInstance( provider, prog, scope );
    }

    ~CModule()
    {
        try {
            _instance = 0;
            delete _module;
        }
        catch(...)
        {
            delete _module;
            throw;
        }
    }

    IInterface *GetInstance()
    {
        return _instance.get();
    }

    std::string         _name;
    DynamicModule      *_module;
    Hand< IInterface >  _instance;
    CreateInstance_t    _CreateInstance;

};

IModule * CInterfaceProvider::ProvideIModule( std::string moduleName, IXmlProgram *prog, IXmlScope *scope )
{
    return new CModule( moduleName, this, prog, scope );
}

};//Dynamo
