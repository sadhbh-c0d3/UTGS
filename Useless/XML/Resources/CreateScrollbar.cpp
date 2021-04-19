#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateHints.h"

namespace Useless {

Resource* CreateScrollbar( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    
    XMLIterator it = i;
    if( !!it.StepInto() )
    {
        AdvScrollbarStyle* asbStyle = MakeAdvScrollbarStyle( i, env );

        int advance = -1, range = -1, pagerSize = -1;
        do
        {
            if( (*it)._name == "scroller" )
            {
                CascadeStyles( it, env, attributes, branch_style, ch_env);
                try_attribute_i( attributes, "advance", &advance );
                try_attribute_i( attributes, "range", &range );
                try_attribute_i( attributes, "pager_size", &pagerSize );
                attributes.clear();
                branch_style.clear();
                ch_env.clear();
            }
            ++it;
        }
        while( !!it );
               
        AdvScrollbarWidget* ptr = new AdvScrollbarWidget( *asbStyle );
        
        delete asbStyle;
        
        CascadeStyles( i, env, attributes, branch_style, ch_env);
        
        // set repeat rate params
        std::string repeatRate;   
        if ( try_attribute( attributes, "repeat_rate", &repeatRate ) )
        {
            std::vector<int> repeatParams;
            repeatParams = to_integer_vector( repeatRate );
            if ( repeatParams.size() != 2 ) 
            { 
                throw Error("Repeat rate given in scrollbar has to have 2 params: period and delay."); 
            }
            ptr->SetRepeatRate( repeatParams[0], repeatParams[1] );
        }

        // set scroller params
        if( pagerSize > 0 )
            ptr->Scroller().SetSize( pagerSize ); 
        if( range > 0 )
            ptr->Scroller().SetRange( range );   
        if( advance > 0 )
            ptr->Scroller().SetAdvance( advance );

        //resize scrollbar
        int size;
        if( try_attribute_i( attributes, "size", &size ) )            
            ptr->Resize( size, size );

        InsertChildWidget( ptr, attributes, env);
        SetHint(ptr, attributes);
        return new AnyResource<AdvScrollbarWidget*>( ptr );
    }
    else
    {
        ScrollbarStyle* sbStyle = MakeScrollbarStyle( i, env );
        ScrollbarWidget* ptr = new ScrollbarWidget( *sbStyle );
        delete sbStyle;
        
        CascadeStyles( i, env, attributes, branch_style, ch_env);
        int value;
        if( try_attribute_i( attributes, "pager_size", &value ) )
            ptr->SetSize( value );
        if( try_attribute_i( attributes, "range", &value ) )
            ptr->SetRange( value );
        if( try_attribute_i( attributes, "advance", &value ) )
            ptr->SetAdvance( value );

        InsertChildWidget( ptr, attributes, env);
        SetHint(ptr, attributes);
        return new AnyResource<ScrollbarWidget*>( ptr );
    }
}


ScrollbarStyle* MakeScrollbarStyle( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env);

    std::string skins = get_attribute( attributes, "skin" );
    int size = to_integer( get_attribute( attributes, "size" ) );
    int sizeLimit = 16;
    try_attribute_i( attributes, "size_limit", &sizeLimit );
    
    // !!!
    return new ScrollbarStyle( size, QuerySkinSet( skins ), sizeLimit ); 
}

AdvScrollbarStyle* MakeAdvScrollbarStyle( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env);

    int buttonScrollSpeed = 1;
    try_attribute_i( attributes, "scroll_speed", &buttonScrollSpeed );

    std::string upButton,downButton;
    int currentButton = 0;
    ScrollbarStyle* sbStyle = NULL;
    for ( i.StepInto(); !!i; ++i )
    {
        CascadeStyles( i, env, attributes, branch_style, ch_env);
              
        // get scrollbar buttons skins names
        if( (*i)._name == "button" )
        {
            switch( currentButton )
            {
            case 0:
                upButton = get_attribute( attributes, "skin" );
                currentButton++;
                break;
            case 1:
                downButton = get_attribute( attributes, "skin" );
                currentButton++;
                break;
            };                 
        }
        else // get scroller params
        if( (*i)._name == "scroller" )
        {
            if( sbStyle == NULL )
                sbStyle = MakeScrollbarStyle( i, env );
            else
                throw Error("There are more than 1 scrollers in the scrollbar.");
        }

        attributes.clear();
        branch_style.clear();
        ch_env.clear();
    }

    
    if( downButton == "" ||
        upButton == "" ||
        !sbStyle)
    {
        throw Error("Scrollbar %s needs 2 butons and scroller.");
    }

    // !!!
    AdvScrollbarStyle* asbStyle = new AdvScrollbarStyle( *sbStyle, 
                                                         QuerySkinMan( upButton ), 
                                                         QuerySkinMan( upButton ),
                                                         buttonScrollSpeed );
    delete sbStyle;
    return asbStyle;
}

};//namespace Useless
