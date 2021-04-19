#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateText.h"

namespace Useless {

Resource *
CreateCredits( XMLIterator i, XMLCreatorEnv *env )
{
    int pix_per_sec = 50;
    int margins = 5;
    try_attribute_i( (*i)._attributes, "pix_per_sec", &pix_per_sec );
    try_attribute_i( (*i)._attributes, "margins", &margins );

    // Localization w/a
    XMLIterator j( i );
    j.StepInto();
    for ( j.StepInto(); !!j; ++j )
    {
        XMLParser::Attributes &attr =
            const_cast< XMLParser::Attributes &>( (*j)._attributes );
        
        Text name = MakeText( attr["name"] );
        attr["name"] = name.GetString();
    }

    Widget *ptr = 0;

    if ( !ptr )
    {
        ptr = new CreditsWidget( i.StepInto(), pix_per_sec, margins );
    }
    InsertChildWidget( ptr, (*i)._attributes, env);    
    return new AnyResource< Widget *>( ptr );
}

};//Useless
