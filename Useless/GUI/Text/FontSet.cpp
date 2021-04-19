#include "UselessPch.h"

#include "Useless/GUI/Text/FontSet.h"

namespace Useless {

FontSet::FontSet()
{}

FontSet::FontSet( const Font &font, int id )
{
    Insert( id, font );
}

FontSet::~FontSet()
{}


int FontSet::Insert( const Font &font )
{
    return _fonts.Insert(font);
}

void FontSet::Insert( int id, const Font &font )
{
    assert( !Exists(id) );
    _fonts.GetMap().Insert( id, font);
}

void FontSet::Remove( int id )
{
    assert( Exists(id) );
    _fonts.Remove( id);
}

int FontSet::Insert( const char *gfx_fname, const char *table_fname, int spacing )
{
    return Insert( Font(gfx_fname, table_fname, spacing ) );
}

int FontSet::Insert( const FontSkin &font_skin, int spacing )
{
    return Insert( Font(font_skin, spacing ) );
}

};//namespace Useless
