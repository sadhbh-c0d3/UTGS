#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateHints.h"

namespace Useless {

Resource* CreateScrolledWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    const char *p_name = "create popup";
    BeginParentWidget( env, p_name, ch_env );

    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    int speed = ScrolledWidget::DEFFAULT_SPEED;
    try_attribute_i( attributes, "speed", &speed );

    int dragSliding = false;
    try_attribute_i( attributes, "drag_slide", &dragSliding );

    std::vector<int> adv = to_integer_vector( "0,0" );
    std::string advance;
  
    if ( try_attribute( attributes, "advance", &advance ) )
    {
        adv = to_integer_vector( advance );
        if ( adv.size() != 2 ) { throw Error("Advance attribute must have x and y coordinates"); }
    }

    //temporary hub
    SPointer<HubWidget> ptr = new HubWidget( 100, 100 );
    ch_env["parent_hub"] = ptr.get();

    // create children elements
    ChildsFromXML( i, &ch_env );

    if( ptr->ChildrenCount( ) > 1 )
        throw Error("Scrolled widget %s has more than 1 content.", (*i)._name.c_str());     
    
    ScrolledWidget *scrollee = new ScrolledWidget();

    if( ptr->ChildrenCount( ) > 0 )
    {
        Widget* content = ptr->FirstChild();
        ptr->Retrieve( content );
        scrollee->AttachWidget( content );
    }
  
    scrollee->SetSpeed(speed);
    scrollee->SetNoDragSliding( !dragSliding );
    scrollee->HScrollTo(adv[0]);
    scrollee->VScrollTo(adv[1]);

    InsertChildWidget( scrollee, attributes, env);
    SetHint(scrollee, attributes);
    return new AnyResource<ScrolledWidget*>( scrollee );
}

};//namespace Useless



