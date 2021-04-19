#ifndef __INCLUDED_CREATE_FONTS_H__
#define __INCLUDED_CREATE_FONTS_H__

#include "Useless/XML/Resources/CreateFromXML.h"
#include "Useless/GUI/Text/Font.h"
#include "Useless/GUI/Skins/AlphaFont.h"
#include "Useless/GUI/Text/FontSet.h"

namespace Useless {

USELESS_RESOURCES_API Resource* CreateFacedFont( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_faced_font( "fonts::faced", CreateFacedFont );


USELESS_RESOURCES_API Resource* CreateAlphaFont( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_alpha_font( "fonts::alpha", CreateAlphaFont );


USELESS_RESOURCES_API Resource* CreateFontSet( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_font_set( "fonts::fontset", CreateFontSet );


USELESS_RESOURCES_API Resource* ResolveFontLink( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_resolve_font_link( "fonts::link", ResolveFontLink );


typedef ResourceQuery< Font > FontResource;
typedef ResourceQuery< FontSet > FontSetResource;


inline Font& QueryFont( const std::string &id )
{
    Font *p_font;
    Resources::Instance().Query("fonts", id, p_font);
    return *p_font;
}

inline FontSet& QueryFontSet( const std::string &id )
{
    FontSet *p_fontset;
    Resources::Instance().Query("fonts", id, p_fontset);
    return *p_fontset;
}

USELESS_RESOURCES_API bool InsertChildFont( const Font &font, XMLCreatorEnv *env );

};//namespace Useless
#endif//__INCLUDED_CREATE_FONTS_H__
