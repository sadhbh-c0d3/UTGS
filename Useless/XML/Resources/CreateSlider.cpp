#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateHints.h"
#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateSlider( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env);

    int size = to_integer( get_attribute( attributes, "size" ) );
    std::string skins = get_attribute( attributes, "skin" );
    
    SliderWidget* ptr = new SliderWidget( size, QuerySkinSet( skins ) );
 
    int value = 2;    
    if( try_attribute_i( attributes, "states", &value) && value < 2 )
    {
        throw Error("Slider %s must have almost 2 states.", (*i)._name.c_str());
    }
    
    ptr->SetNumStates( value );
    
    if( try_attribute_i( attributes, "select", &value ) )
    {
        ptr->SetSelected( value );
    }
    
    if( try_attribute_i( attributes, "grip_size", &value ) )
    {
        ptr->SetGripSize( value ); 
    }

    InsertChildWidget( ptr, attributes, env);
    SetHint(ptr, attributes);
    
    return new AnyResource<SliderWidget*>( ptr );
}

};//namespace Useless
