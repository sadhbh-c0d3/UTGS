#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateFonts.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateText.h"

namespace Useless {

Resource* CreateTextWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    std::string font = get_attribute( attributes, "font" );

    int bodyAsLink = 0;
    try_attribute_i( attributes, "body_as_link", &bodyAsLink );
    Text text;
    if ( bodyAsLink )
        text = QueryText( make_string( (*i)._value ));
    else
        text = MakeText(to_chars( (*i)._value ));


    Widget *ptr;
    XMLIterator ch_i = i;
    int line_spacing = 0;
    std::string alignment;

    bool adv_text = try_attribute_i( attributes, "spacing", &line_spacing );
    adv_text |= try_attribute( attributes, "align", &alignment );

    // create DualText
    if ( !!ch_i.StepInto() )
    {
        int shift = 2;
        try_attribute_i( attributes, "shift", &shift );

        std::wstring left, right;
        for ( ; !!ch_i; ++ch_i )
        {
            if ( (*ch_i)._name == "left" )
                left = to_chars( (*ch_i)._value );
            else
                if ( (*ch_i)._name == "right" )
                    right = to_chars( (*ch_i)._value );
        }
        
        int bodyAsLink = 0;
        try_attribute_i( attributes, "body_as_link", &bodyAsLink );
        Text leftT, rightT;
        if ( bodyAsLink )
        {
            leftT  = QueryText( make_string( left ));
            rightT = QueryText( make_string( right ));
        }
        else
        {
            leftT  = MakeText(left);
            rightT = MakeText(right);
        }

        DualTextWidget *p_dual;
        ptr = p_dual = new DualTextWidget( 100, QueryFont(font), leftT, rightT, shift );

        std::string dotcode;
        if ( try_attribute( attributes, "dot", &dotcode ) )
            p_dual->SetDotCode( ( dotcode.size()>0 ? dotcode[0] : ' ' ) );    
    }
    else
    if ( adv_text )
    {
        int align = 0;
        if ( alignment == "left"   ) { align = 0; } else
        if ( alignment == "right"  ) { align = 1; } else
        if ( alignment == "center" ) { align = 2; }

        if ( (*i)._name == "activetext" )
        {
            ptr = new ActiveText( QueryFont(font), text, line_spacing, align );
        }
        else
        {
            ptr = new AdvTextWidget( QueryFont(font), text, line_spacing, align );
        }
    }   
    else
    {
         TextWidget *ptrTW = new TextWidget( QueryFont(font), text );
         ptr = ptrTW;
         ptrTW->SetText( text );
    }

    InsertChildWidget( ptr, attributes, env);
    return new AnyResource<Widget*>( ptr );
}

};//namespace Useless
