#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateRollerWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    std::string sImage = get_attribute( (*i)._attributes, "img" );
    std::string sRange = get_attribute( (*i)._attributes, "range" );

    ImageResource::Type image = *ImageResource("images", sImage);
    int range = to_integer( sRange );

    RollerWidget *ptr = new RollerWidget( *image, range );
    
    double speed = 1.0;
    if ( try_attribute_r( (*i)._attributes, "speed", &speed ))
    {
        ptr->SetRollSpeed( speed );
    }

    InsertChildWidget( ptr, attributes, env);    
    return new AnyResource< Widget *>( ptr );
}

Resource* CreateScoreRoller( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    std::string sImage = get_attribute( (*i)._attributes, "img" );
    std::string sNum = get_attribute( (*i)._attributes, "numRollers" );

    ImageResource::Type image = *ImageResource("images", sImage);
    int numRollers = to_integer( sNum );

    ScoreRoller *ptr = new ScoreRoller( *image, numRollers );
    
    double speed = 1.0;
    if ( try_attribute_r( (*i)._attributes, "speed", &speed ))
    {
        ptr->SetRollSpeed( speed );
    }

    InsertChildWidget( ptr, attributes, env);    
    return new AnyResource< HubWidget *>( ptr );
}

}; // namespace Useless
