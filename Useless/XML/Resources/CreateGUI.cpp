#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSystem.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/GUI/Misc/Layout.h"
#include "Useless/GUI/Display/ScreenMan.h"

namespace Useless {

// Do this if we protect application with red-green cross
void USELESS_API SetCopyrightProtectionString( const std::string &screenOption1, std::string screenOption2 );
std::string USELESS_API GetCopyrightProtectionString();

Resource* CreateGUI( XMLIterator i, XMLCreatorEnv *env )
{
    SPointer<GUIMaster> sp_gui;

    int active = 1;
    try_attribute_i( (*i)._attributes, "active", &active );

    std::string screenId = "screen";
    try_attribute( (*i)._attributes, "screen", &screenId );

    std::string cursor;
    Screen &screen = QueryScreen( screenId );
    sp_gui = new GUIMaster( screen, !active);

    if ( try_attribute( (*i)._attributes, "cursor", &cursor )  )
    {
        sp_gui->GetScreenMan().CreateDefaultCursor( *MakeMouseCursor( cursor ));
        sp_gui->GetScreenMan().SetDefaultCursor();
    }
    
    const char *p_name;
    if ( (*env).Query("name", p_name ) )
    {
        // We make sure all GUI resources are bound to screen are removed when screen is destroyed.
		// @TODO: (*) won't work right now, since OnDestroy is invoked too late...
		/*
		screen.OnDestroy.TieTernary( &Resources::Instance(), &__Resources::Remove, std::string("widgets"), std::string(p_name), true );
        screen.OnDestroy.TieTernary( &Resources::Instance(), &__Resources::Remove, std::string("layouts"), std::string(p_name), true );
        screen.OnDestroy.TieTernary( &Resources::Instance(), &__Resources::Remove, std::string("system"),  std::string(p_name), true );
		*/

        HubWidget *p_desktop = sp_gui->GetWorkspace().GetDesktop();
        std::string id = p_name; id += ".desktop";
        Resources::Instance().Insert("widgets", id, new AnyResource< HubWidget*>( p_desktop ));
        
        HubWidget *p_menus = sp_gui->GetWorkspace().GetMenus();
        id = p_name; id += ".menus";
        Resources::Instance().Insert("widgets", id, new AnyResource< HubWidget*>( p_menus ));
        
        HubWidget *p_hints = sp_gui->GetWorkspace().GetHints();
        id = p_name; id += ".hints";
        Resources::Instance().Insert("widgets", id, new AnyResource< HubWidget*>( p_hints ));
        
        std::string layout;
        if ( try_attribute( (*i)._attributes, "layout", &layout) )
        {
            LayoutBase *p_layout;
            if ( layout == "horizontal" )
            {
                p_layout = new HLayout( p_desktop );
            }
            else // vertical
            {
                p_layout = new VLayout( p_desktop );
            }
            p_desktop->Posses( p_layout );
            Resources::Instance().Insert("layouts", id, new AnyResource<LayoutBase*>(p_layout) );
        }

        std::string hints;
        if (try_attribute((*i)._attributes, "hints", &hints ))
        {  
            SPointer<Hints> *sp_hints;
            Resources::Instance().Query("system", hints, sp_hints);            
            sp_gui->SetHintMan(*sp_hints);
        }

        // Do this if we protect application with red-green cross
        std::string protection;
        if ( try_attribute((*i)._attributes, "protection", &protection ))
        {
            SetCopyrightProtectionString( protection, protection );
        }
        else
        {
            SetCopyrightProtectionString( GetCopyrightProtectionString(), GetCopyrightProtectionString() );
        }
    }

    return new AnyResource< SPointer<GUIMaster> >( sp_gui );
}

};//namespace Useless
