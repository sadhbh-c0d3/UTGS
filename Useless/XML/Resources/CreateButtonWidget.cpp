#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/XML/Resources/CreateFonts.h"
#include "Useless/XML/Resources/CreateText.h"
#include "Useless/XML/Resources/CreateHints.h"

#include "Useless/GUI/Widgets/CaptionButton.h"
#include "Useless/GUI/Widgets/CaptionHilite.h"
#include "Useless/GUI/Widgets/CaptionMark.h"
#include "Useless/GUI/Widgets/CaptionRadio.h"
#include "Useless/GUI/Widgets/CaptionSwitch.h"
#include "Useless/GUI/Widgets/CaptionTab.h"
#include "Useless/GUI/Widgets/CaptionTrigger.h"
#include "Useless/GUI/Widgets/IconButton.h"
#include "Useless/GUI/Widgets/ButtonWidget.h"
#include "Useless/GUI/Widgets/HiliteWidget.h"
#include "Useless/GUI/Widgets/MarkWidget.h"
#include "Useless/GUI/Widgets/RadioWidget.h"
#include "Useless/GUI/Widgets/SwitchWidget.h"
#include "Useless/GUI/Widgets/TabWidget.h"
#include "Useless/GUI/Widgets/TriggerWidget.h"
#include "Useless/GUI/Misc/DataStream.h"
#include "Useless/GUI/Misc/DataTypes.h"
#include "Useless/GUI/Skins/NullSkin.h"

namespace Useless {

Resource* CreateButtonWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env);

    bool caption = false;
    std::string fontSet;
    FontSet fonts;
    
    std::vector<int> pos = to_integer_vector( "0,0" );
    
    if( try_attribute( attributes, "font", &fontSet ) )
    {
        caption = true;
        fonts = QueryFontSet(fontSet);
    
        std::string position;   
        if ( try_attribute( attributes, "distance", &position ) )
        {
            pos = to_integer_vector( position );
            if ( pos.size() != 2 ) { throw Error("pos attribute must have x and y coordinates"); }
        }
    }

    Widget *ptr;

    std::string iconSet;
    if( try_attribute( attributes, "icon", &iconSet ) )
    {
        std::string skinSet = get_attribute( attributes, "skin" );
        SkinMan &skins = QuerySkinMan(skinSet);
        SkinMan &icons = QuerySkinMan(iconSet);

        int distance = 0;
        try_attribute_i( attributes, "distance", &distance );

        if( (*i)._name == "button")
            ptr = new IconButton( icons, skins, distance );
        else 
        if( (*i)._name == "hilite" ){
            typedef Useless::TemplateWidget< Useless::IconWidget, Useless::HiliteLogic > IconHilite;
            ptr = new IconHilite( icons, skins, distance );
        }
        else 
        if( (*i)._name == "mark" ){
            typedef Useless::TemplateWidget< Useless::IconWidget, Useless::MarkLogic > IconMark;
            ptr = new IconMark( icons, skins, distance );
        }
        else 
        if( (*i)._name == "radio" ){
            typedef Useless::TemplateWidget< Useless::IconWidget, Useless::RadioLogic > IconRadio;
            ptr = new IconRadio( icons, skins, distance );
        }
        else 
        if( (*i)._name == "switch" ){
            typedef Useless::TemplateWidget< Useless::IconWidget, Useless::SwitchLogic > IconSwitch;
            ptr = new IconSwitch( icons, skins, distance );
        }
        else 
        if( (*i)._name == "tab" ){
            typedef Useless::TemplateWidget< Useless::IconWidget, Useless::TabLogic > IconTab;
            ptr = new IconTab( icons, skins, distance );
        }
        else 
        if( (*i)._name == "trigger" ){
            typedef Useless::TemplateWidget< Useless::IconWidget, Useless::TriggerLogic > IconTrigger;
            ptr = new IconTrigger( icons, skins, distance );
        }
        else
             throw Error("Undefined icon widget: %s", (*i)._name.c_str());
    }
    else if( caption )
    {
        std::string skinSet;
        SkinMan *skins;
        bool temp_skinset = false;
        if( try_attribute( attributes, "skin", &skinSet ) )
            skins = &QuerySkinMan( skinSet );
        else
        {
            skins = new SkinSet();
            ((SkinSet*)skins)->Insert( NullSkin() );
            temp_skinset = true;
        }

        XMLIterator it = i;
        if( !!it.StepInto() )
        {
            attributes.clear();
            branch_style.clear();
            ch_env.clear();
            CascadeStyles( it, env, attributes, branch_style, ch_env);

            // create child DataObject
            DataObject* p_content;
            int distance;
            if( (*it)._name == "text" )
            {
                int link = 0;
                try_attribute_i( attributes, "body_as_link", &link );
                if (link==1)
                {
                    p_content = new DT_Text( QueryDTText( make_string((*it)._value )));
                }
                else
                {
                    distance = 0;
                    try_attribute_i( attributes, "distance", &distance );
                    p_content = new DT_Text( MakeText( to_chars( (*it)._value )), distance);
                }
            }
            else if( (*it)._name == "image" )
            {
                distance = 50;
                try_attribute_i( attributes, "distance", &distance );                        
                std::string src = get_attribute(attributes, "src");
                SPointer< ImageBase > sp_image = MakeImage( src );
                p_content = new DT_Image( dynamic_cast< Image &>( *sp_image ), distance);
            }
            else 
            { 
                throw Error("Unsupported DataObject: %s.", (*i)._name.c_str()); 
            }

            attributes.clear();
            branch_style.clear();
            ch_env.clear();
            CascadeStyles( i, env, attributes, branch_style, ch_env);
                       
            if( (*i)._name == "button"){        
                typedef Useless::TemplateWidget< Useless::ContentWidget, Useless::ButtonLogic > ContentButton;
                ptr = new ContentButton(  p_content, *skins, fonts, Pos(pos[0],pos[1]) );
            }
            else
            if( (*i)._name == "hilite"){        
                typedef Useless::TemplateWidget< Useless::ContentWidget, Useless::HiliteLogic > ContentHilite;
                ptr = new ContentHilite(  p_content, *skins, fonts, Pos(pos[0],pos[1]) );
            }
            else
            if( (*i)._name == "mark"){        
                typedef Useless::TemplateWidget< Useless::ContentWidget, Useless::MarkLogic > ContentMark;
                ptr = new ContentMark(  p_content, *skins, fonts, Pos(pos[0],pos[1]) );
            }
            else
            if( (*i)._name == "radio"){        
                typedef Useless::TemplateWidget< Useless::ContentWidget, Useless::RadioLogic > ContentRadio;
                ptr = new ContentRadio(  p_content, *skins, fonts, Pos(pos[0],pos[1]) );
            }
            else
            if( (*i)._name == "switch"){        
                typedef Useless::TemplateWidget< Useless::ContentWidget, Useless::SwitchLogic > ContentSwitch;
                ptr = new ContentSwitch(  p_content, *skins, fonts, Pos(pos[0],pos[1]) );
            }
            else
            if( (*i)._name == "tab"){        
                typedef Useless::TemplateWidget< Useless::ContentWidget, Useless::TabLogic > ContentTab;
                ptr = new ContentTab(  p_content, *skins, fonts, Pos(pos[0],pos[1]) );
            }
            else
            if( (*i)._name == "trigger"){        
                typedef Useless::TemplateWidget< Useless::ContentWidget, Useless::TriggerLogic > ContentTrigger;
                ptr = new ContentTrigger(  p_content, *skins, fonts, Pos(pos[0],pos[1]) );
            }
            else
                 throw Error("Undefined content widget: %s", (*i)._name.c_str());
        }
        else
        {
            CaptionWidget::Alignment alignment = CaptionWidget::INNER;   
            std::string align("INNER");    
            if( try_attribute( attributes, "alignment", &align) )
            {
                if( align == "inner" || align == "INNER" || align == "0" )
                    alignment = CaptionWidget::INNER;
                else
                if( align == "inner_left" || align == "INNER_LEFT" || align == "1" )
                    alignment = CaptionWidget::INNER_LEFT;
                else
                if( align == "inner_right" || align == "INNER_RIGHT" || align == "2" )
                    alignment = CaptionWidget::INNER_RIGHT;
                else
                if( align == "outer_left" || align == "OUTER_LEFT" || align == "3" )
                    alignment = CaptionWidget::OUTER_LEFT;
                else
                if( align == "outer_right" || align == "OUTER_RIGHT" || align == "4" )
                    alignment = CaptionWidget::OUTER_RIGHT;
            }

            int link = 0;
            try_attribute_i( attributes, "body_as_link", &link );
            Text text;
            if ( link )
                text = QueryText( make_string((*i)._value));
            else
                text = MakeText( to_chars( (*i)._value ));


            if( (*i)._name == "button")
                ptr = new CaptionButton( text, *skins, fonts, alignment, 
                                                        Pos(pos[0],pos[1]) );
            else 
            if( (*i)._name == "hilite" )
                ptr = new CaptionHilite( text, *skins, fonts, alignment, 
                                                        Pos(pos[0],pos[1]) );
            else 
            if( (*i)._name == "mark" )
                ptr = new CaptionMark( text, *skins, fonts, alignment, 
                                                        Pos(pos[0],pos[1]) );
            else 
            if( (*i)._name == "radio" )
                ptr = new CaptionRadio( text, *skins, fonts, alignment, 
                                                        Pos(pos[0],pos[1]) );
            else 
            if( (*i)._name == "switch" )
                ptr = new CaptionSwitch( text, *skins, fonts, alignment, 
                                                        Pos(pos[0],pos[1]) );
            else 
            if( (*i)._name == "tab" )
                ptr = new CaptionTab( text, *skins, fonts, alignment, 
                                                        Pos(pos[0],pos[1]) );
            else 
            if( (*i)._name == "trigger" )
                ptr = new CaptionTrigger( text, *skins, fonts, alignment, 
                                                        Pos(pos[0],pos[1]) );
            else
                 throw Error("Undefined caption widget: %s", (*i)._name.c_str());
        }

        if( temp_skinset )
            delete skins;
    }
    else 
    {
        std::string skinSet = get_attribute( attributes, "skin" );
        SkinMan &skins = QuerySkinMan(skinSet);

        if( (*i)._name == "button") 
            ptr = new ButtonWidget( skins );
        else 
        if( (*i)._name == "hilite" )
            ptr = new HiliteWidget( skins );
        else 
        if( (*i)._name == "mark" )
            ptr = new MarkWidget( skins );
        else 
        if( (*i)._name == "radio" )
           ptr = new RadioWidget( skins );
        else 
        if( (*i)._name == "switch" )
            ptr = new SwitchWidget( skins );
        else 
        if( (*i)._name == "tab" )
            ptr = new TabWidget( skins );
        else 
        if( (*i)._name == "dragger" )
            ptr = new DraggerWidget( skins );
        else 
        if( (*i)._name == "trigger" )
            ptr = new TriggerWidget( skins );
        else
            throw Error("Undefined button widget: %s", (*i)._name.c_str());
    }

    int focusable=false;
    if ( try_attribute_i( attributes, "focusable", &focusable))
    {
        if (!focusable)
            dynamic_cast<KeyButtonLogic*>( ptr)->DisableFocussing();
    }
    
    InsertChildWidget( ptr, attributes, env);
    SetHint(ptr, attributes);
    return new AnyResource<Widget*>( ptr );
}

};//namespace Useless
