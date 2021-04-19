#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateFonts.h"
#include "Useless/XML/Resources/CreateHints.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateText.h"
#include "Useless/GUI/Widgets/NumericField.h"

namespace Useless {

Resource* CreateTextField( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env);

    int width = to_integer( get_attribute( attributes, "width" ) );
    int height = to_integer( get_attribute( attributes, "height" ) );
    std::string fonts = get_attribute( attributes, "font" );
    std::string skins = get_attribute( attributes, "skin" );

    int bodyAsLink = 0;
    try_attribute_i( attributes, "body_as_link", &bodyAsLink );
    Text text;
    if ( bodyAsLink )
        text = QueryText( make_string( (*i)._value ));
    else
        text = MakeText( to_chars( (*i)._value ));

    int numeric = 0; 
    TextField* ptr;
    if( try_attribute_i( attributes, "numeric", &numeric ) ) 
    {
        ptr = new NumericField( width, height,
                                QueryFontSet(fonts), 
                                QuerySkinMan(skins),
                                text );
    }
    else
    {
        ptr = new TextField( width, height,
                             QueryFontSet(fonts), 
                             QuerySkinMan(skins),
                             text );
    }
    
    int d;
    if( try_attribute_i( attributes, "left", &d))
    {
        ptr->SetLeftMargin( d);
    }
    if( try_attribute_i( attributes, "right", &d))
    {
        ptr->SetRightMargin( d);
    }

    std::string align;    
    if( try_attribute( attributes, "alignment", &align ) )
    {
        if( align == "LEFT" || align == "0" )
            ptr->SetAlignment( TextField::LEFT );
        else
            ptr->SetAlignment( TextField::RIGHT );
    }

    int char_limit = -1;
    if ( try_attribute_i( attributes, "char_limit", &char_limit ) )
    {
        ptr->SetCharLimit( char_limit );
    }

    for ( XMLIterator chi = i.GetChildrenIterator(); !!chi; ++chi )
    {
        if ( (*chi)._name == "allowed_chars" )
        {
            Text chars = MakeText( (*chi)._value );
            std::set< UniCode > allowed_chars;
            for ( Text::iterator iC = chars.begin(); iC != chars.end(); ++iC )
            {
                allowed_chars.insert(*iC);
            }
            ptr->SetAllowedChars( allowed_chars );
        }
    }
    

    ptr->SetText( (*i)._value );
    InsertChildWidget( ptr, attributes, env);
    SetHint(ptr, attributes);
    return new AnyResource<TextField*>( ptr );
}

};//namespace Useless
