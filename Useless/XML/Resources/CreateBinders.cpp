#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateBinders.h"

namespace Useless {

Resource* CreateGUIBinder( XMLIterator i, XMLCreatorEnv *env )
{
    GUIBinderResource::Type guiBinder( new GUIBinder );

    for ( XMLIterator chIt = i.GetChildrenIterator(); !!chIt ; ++chIt )
    {        
        std::string signalName = get_attribute( (*chIt)._attributes, "signal" );
        GUIBinder::BindDef def;
        def._eventToRaise = get_attribute( (*chIt)._attributes, "event" );
        def._stringToPass = get_attribute( (*chIt)._attributes, "string" );
        guiBinder->_defs.insert( std::make_pair( signalName, def ));
    }

    return (new AnyResource< GUIBinderResource::Type >( guiBinder ));
}


};//Useless
