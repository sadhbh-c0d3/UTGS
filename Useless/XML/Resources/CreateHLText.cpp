#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateFonts.h"
#include "Useless/XML/Resources/CreateHints.h"
#include "Useless/XML/Resources/CreateStyle.h"

namespace Useless {

Resource* CreateHLText( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    std::string font = get_attribute( attributes, "font" );
    FontSet &font_set = QueryFontSet(font);    

    bool hilited = ( (*i)._name == "hltext" );

    FontChanger *ptr;
    int line_spacing = 0;
    std::string alignment;
    XMLIterator ch_i = i;

    if( !!ch_i.StepInto() )
    {
        // DualText
        std::wstring left, right;
        for( ; !!ch_i; ++ch_i )
        {
            if( (*ch_i)._name == "left" )
                left = (*ch_i)._value;
            else
                if( (*ch_i)._name == "right" )
                    right = (*ch_i)._value;
        }

        int w = to_integer( get_attribute( attributes, "width" ) );
        
        int shift = 2;
        try_attribute_i( attributes, "shift", &shift );
        
        BaseDualText *p_dual;
        if( hilited )
        {
            HLDualText *p_hl_dual = new HLDualText( w, font_set.GetFont( FontChanger::DEFAULT), left, right, shift );
            ptr = p_hl_dual;
            p_dual = p_hl_dual;
        }
        else
        {
            DualTextAndFont *p_dual_font = new DualTextAndFont( w, font_set.GetFont( FontChanger::DEFAULT), left, right, shift );
            ptr = p_dual_font;
            p_dual = p_dual_font;
        }
               
        std::string dotcode;
        if( try_attribute( attributes, "dot", &dotcode ) )
            p_dual->SetDotCode( ( dotcode.size()>0 ? dotcode[0] : ' ' ) );
    }
    else
    if( try_attribute_i( attributes, "spacing", &line_spacing ) ||
        try_attribute( attributes, "align", &alignment ) )
    {
        // AdvText
        int align = 0;
        if( alignment == "left"   ) { align = 0; } else
        if( alignment == "right"  ) { align = 1; } else
        if( alignment == "center" ) { align = 2; }

        if( hilited )
            ptr = new HLAdvText( font_set.GetFont( FontChanger::DEFAULT), to_chars( (*i)._value ), line_spacing, align );
        else
            ptr = new AdvTextAndFont( font_set.GetFont( FontChanger::DEFAULT), to_chars( (*i)._value ), line_spacing, align );
    }   
    else
    {
        // Text
        if( hilited )
            ptr = new HLText( font_set.GetFont( FontChanger::DEFAULT), to_chars( (*i)._value ) );
        else
            ptr = new TextAndFont( font_set.GetFont( FontChanger::DEFAULT), to_chars( (*i)._value ) );
    }
    
    ptr->UseFontSet( font_set );
    InsertChildWidget( ptr, attributes, env);
    SetHint(ptr, attributes);
    return new AnyResource<Widget*>( ptr );
}

};//namespace Useless
