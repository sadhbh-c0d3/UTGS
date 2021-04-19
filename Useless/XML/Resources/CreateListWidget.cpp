#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateFonts.h"
#include "Useless/XML/Resources/CreateHints.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateText.h"

namespace Useless {

void SetupListAttributes( IWList &list, const XMLAttributes &attributes);

Resource* CreateListWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;

    ScrollbarStyle* sbStyle = NULL;
    AdvScrollbarStyle* asbStyle = NULL;
    FrameStyle* fStyle = NULL;
    XMLIterator ch_i = i;
    
    CascadeStyles( i, env, attributes, branch_style, ch_env);
    
    std::vector< Text > lines;
    for ( ch_i.StepInto(); !!ch_i; ++ch_i )
    {
        // a little bit tricky, but C++ will creates new scope for those variables inside this 'for' block.
        XMLCreatorEnv *env = &ch_env;
        XMLCreatorEnv ch_env;
        XMLAttributes attributes;
        XMLStyle      branch_style;
        CascadeStyles( ch_i, env, attributes, branch_style, ch_env);

        if( (*ch_i)._name == "text" )
        {
            int bodyAsLink = 0;
            try_attribute_i( attributes, "body_as_link", &bodyAsLink );
            Text text;
            if ( bodyAsLink )
                text = QueryText( make_string((*ch_i)._value));
            else
                text = MakeText( to_chars( (*ch_i)._value ));    
            lines.push_back( text );
        }
        else
        if( (*ch_i)._name == "scrollbar" )
        {
            XMLIterator it = ch_i;
            if( !!it.StepInto() )
                asbStyle = MakeAdvScrollbarStyle( ch_i, env );
            else
                sbStyle = MakeScrollbarStyle( ch_i, env );
        }
        else
        if( (*ch_i)._name == "frame" )
        {
            if( fStyle == NULL )
            {
                std::string skin = get_attribute( attributes, "skin" );
                int left=-1, right=-1, top=-1, bottom=-1, w=100, h=100;
                try_attribute_i( attributes, "left", &left);
                try_attribute_i( attributes, "right", &right);
                try_attribute_i( attributes, "top", &top);
                try_attribute_i( attributes, "bottom", &bottom);        
                try_attribute_i( attributes, "width", &w);
                try_attribute_i( attributes, "height", &h);
                fStyle = new FrameStyle( w, h, QuerySkin(skin), left, top, right, bottom );;
            }
            else
                throw Error("There are more than 1 frame in the list element.");
        }
        else
            throw Error("Unsupported tag %s in list.", (*ch_i)._name.c_str() );

        attributes.clear();
        branch_style.clear();
        ch_env.clear();
    }

    std::string fonts = get_attribute( attributes, "font" );
    std::string skins = get_attribute( attributes, "skin" );

    int shift = 2;
    try_attribute_i( attributes, "shift", &shift );    

    ListStyle2 listStyle( QueryFontSet( fonts ), 
                          QuerySkinMan( skins ),
                          shift );

    if( fStyle )
    {
        AdvListWidget* ptr;

        if( sbStyle )
        {
            ptr = new AdvListWidget( listStyle, *fStyle, *sbStyle ); 
            delete sbStyle;
        }
        else
        if( asbStyle )
        {
            ptr = new AdvListWidget( listStyle, *fStyle, *asbStyle ); 
            delete asbStyle;
        }
        else
            throw Error("No scrollbar in advanced list.");

        delete fStyle;

        for( std::vector<Text>::iterator i=lines.begin();i!=lines.end();i++)
            ptr->Items() << *i;
        
        SetupListAttributes( *ptr, attributes);
        InsertChildWidget( ptr, attributes, env);
        SetHint(ptr, attributes);

        return new AnyResource<AdvListWidget*>( ptr );
    }
    else
    {
        ListWidget* ptr = new ListWidget( listStyle );
        
        for( std::vector<Text>::iterator i=lines.begin();i!=lines.end();i++)
            *ptr << *i;
            
        SetupListAttributes( *ptr, attributes);
        InsertChildWidget( ptr, attributes, env);
        SetHint(ptr, attributes);
        return new AnyResource<ListWidget*>( ptr );
    }
}

void SetupListAttributes( IWList &list, const XMLAttributes &attributes)
{
    int selector_selects;
    if ( try_attribute_i( attributes, "selector_selects", &selector_selects))
    {
        list.Items().SelectorSelects(selector_selects);
    }
    
    int selector_size;
    if ( try_attribute_i( attributes, "selector_size", &selector_size))
    {
        list.Items().SetSelectorSize(selector_size);
    }
}

}; // namespace Useless
