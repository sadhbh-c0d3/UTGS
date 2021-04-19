#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"

namespace Useless {

Resource* CreateLoomWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    const char *p_name = "create loom widget";
    BeginParentWidget( env, p_name, ch_env );

    XMLAttributes attributes;
    XMLStyle branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    double duration = 1.0;
    try_attribute_r( attributes, "duration", &duration);

    std::string type("SLIDE_DOWN");
    try_attribute( attributes, "type", &type);
    
    LoomWidget::Technique technique;

    if( type == "SLIDE_DOWN" || type == "0" )
        technique = LoomWidget::SLIDE_DOWN;
    else
    if( type == "SLIDE_UP" || type == "1" )
        technique = LoomWidget::SLIDE_UP;
    else
    if( type == "SLIDE_LEFT" || type == "2" )
        technique = LoomWidget::SLIDE_LEFT;
    else
    if( type == "SLIDE_RIGHT" || type == "3" )
        technique = LoomWidget::SLIDE_RIGHT;

    LoomWidget* p_loom = new LoomWidget( technique, duration );

    int minVisible;
    if( try_attribute_i( attributes, "min_visible", &minVisible) )
        p_loom->SetMinVisible( minVisible );

    // create temporary parent hub
    HubWidget * ptr = new HubWidget( 100, 100 );
    ch_env["parent_hub"] = ptr;

    ChildsFromXML( i, &ch_env );

    Widget* p_child_widget = ptr->FirstChild();

    if( ptr->ChildrenCount( ) > 1 )
        throw Error("Loom %s has more than 1 widget to attach.", (*i)._name.c_str());
    else 
        if( ptr->Retrieve( p_child_widget ) )
            p_loom->AttachWidget( p_child_widget, p_child_widget->GetPosition().x,
                                  p_child_widget->GetPosition().y );                                   
    
    delete ptr;
    InsertChildWidget( p_loom, attributes, env);
    return new AnyResource<LoomWidget*>( p_loom );
}

};//namespace Useless
