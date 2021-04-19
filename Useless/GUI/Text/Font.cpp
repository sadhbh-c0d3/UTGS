#include "UselessPch.h"

#include "Useless/GUI/Text/Font.h"
#include "Useless/GUI/Skins/FacedFont.h"
#include "Useless/GUI/Skins/AlphaFont.h"

#include "Useless/Graphic/Planes/TransparentImage.h"

namespace Useless {

Font::Font(const char* fn_font, const char* fn_table, int s)
    :_skin( new FacedFont( fn_font, fn_table ) )
{
    if (s) _skin->SetKerning(s);
}

Font::Font(const char* fn_font, const char* fn_alpha,  const char* fn_table, int s)
    :_skin( new FacedFont( TransparentImage( fn_font, fn_alpha ), fn_table ) )
{
    if (s) _skin->SetKerning(s);
}

Font::Font( const FontSkin &skin, int s )
    :_skin( &CopyOf< FontSkin >(skin) )
{
    if (s) _skin->SetKerning(s);
}

Font::~Font()
{
}


};//namespace Useless
