#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSystem.h"
#include "Useless/System/factory/CreateScreen.h"

namespace Useless {

Resource* CreateScreen( XMLIterator i, XMLCreatorEnv *env )
{
    int width = 640;
    int height = 480;
    int bpp = 16;
    double refresh = 0.0;
    int windowed = 0;
    int sys_cursor = 0;
    int decorations = 1;
    int x = 0;
    int y = 0;
    bool reposition = false;
    std::string title="--== Untitled ==--";
    std::string parent;
    try_attribute_i( (*i)._attributes, "width", &width );
    try_attribute_i( (*i)._attributes, "height", &height );
    try_attribute_i( (*i)._attributes, "bpp", &bpp );
    try_attribute_r( (*i)._attributes, "refresh", &refresh );
    try_attribute_i( (*i)._attributes, "windowed", &windowed );
    try_attribute_i( (*i)._attributes, "sys_cursor", &sys_cursor );
    try_attribute( (*i)._attributes, "title", &title );
    if ( !try_attribute_i( (*i)._attributes, "decorations", &decorations ))
    {
        // fullscreen apps have no window-decorations.
        decorations = windowed;
    }
    reposition = try_attribute_i( (*i)._attributes, "x", &x );
    reposition |= try_attribute_i( (*i)._attributes, "y", &y );

    SPointer<Screen> p_screen = Useless::Creator<Screen>::Create( title.c_str());

    if ( try_attribute( (*i)._attributes, "parent", &parent ))
    {
        SPointer< Screen > p_parent = *ScreenResource("system", parent);
        p_screen->SetParent( p_parent.get() );
    }

    if ( !windowed )
    {
        try {
            p_screen->Open( width, height, bpp, refresh );
        }
        catch( ... ) {
            // nie uda³o siê otworzyæ fullscreen'a, spróbuj otworzyæ w oknie
            p_screen->OpenWindowed( width, height );
            p_screen->AllowSizing( false );
        }
    }
    else
    {
        p_screen->OpenWindowed( width, height );
    }

    if (!sys_cursor)
        p_screen->HideCursor();
    else
        p_screen->ShowCursor(); // needed to set Application::_cursor_status

    if (!decorations)
        p_screen->HideDecorations();
    else
        p_screen->ShowDecorations();

    if ( reposition )
        p_screen->Reposition( x, y );

    p_screen->Clear( 0L, Rect( width, height ));
    p_screen->PrintText( 20, 20, "Setting up display. Please, wait...");
    p_screen->Swap();
    p_screen->Clear( 0L, Rect( width, height ));
    p_screen->PrintText( 20, 20, "Setting up display. Please, wait...");
    p_screen->Swap();
    

    return new AnyResource< SPointer<Screen> >( p_screen );
}

};//namespace Useless
