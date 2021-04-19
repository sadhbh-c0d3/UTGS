#include "UselessPch.h"

#include "Useless/GUI/Skins/CaptionSkin.h"

namespace Useless {

CaptionSkin::CaptionSkin( const Text &caption, const Font &font ):
    _font(font), _caption(caption)
{
}

CaptionSkin::~CaptionSkin()
{
}

void  CaptionSkin::Paint ( const Painter& painter, const Pos& pos )
{
    _font.Paint(painter,_caption,pos);
}
    
};//namespace Useless
