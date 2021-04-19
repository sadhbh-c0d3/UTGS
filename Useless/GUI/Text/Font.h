#ifndef __INCLUDED_USELESS_FONT_H__
#define __INCLUDED_USELESS_FONT_H__

#include "Useless/Graphic/Planes/Painter.h"
#include "Useless/GUI/Text/Text.h"

#include "Useless/GUI/Skins/FontSkin.h"
#include "Useless/Util/Storage/AssocVector.h"
#include "Useless/Util/Storage/StupidVector.h"
#include "Useless/Util/VirtualValue.h"

namespace Useless {

class USELESS_API Font
{
public:
    Font(const FontSkin& skin, int spacing=0 );
    ~Font();

    void Paint( const Painter &painter, const Text& text, const Pos &pos=Pos() )
    {
        if (!!_skin) _skin->Paint( painter, text, pos);
    }

    void Preformat( const Text &text )
    {
        if (!!_skin) _skin->Preformat( text);
    }

    int GetHeight() const                   { return (!_skin)? 0 : _skin->GetHeight(); }
    int GetWidth ( UniCode code )     const { return (!_skin)? 0 : _skin->GetWidth(code); }
    int GetWidth ( const Text& text ) const { return (!_skin)? 0 : _skin->GetWidth(text); }

    typedef FontSkin::Letters Letters;
    Letters GetStats( const Text& text ) const { return (!_skin)? Letters() : _skin->GetStats(text); }

    /*! Set/get kerning between two charactes 'lhs' and 'rhs'
     *  by default (if !lhs or !rhs) it returns average kerning */
    void  SetKerning( int s, UniCode lhs=0, UniCode rhs=0 )  { _skin->SetKerning(s); }
    int   GetKerning( UniCode lhs=0, UniCode rhs=0 ) const { return _skin->GetKerning(); }

    virtual bool operator !() { return !_skin; }

//---------------------- compatibility part
public:
    Font() {}
    Font(const char* fn_font,                        const char* fn_table, int spacing=0);
    Font(const char* fn_font, const char* fn_alpha,  const char* fn_table, int spacing=0);

    const FontSkin& GetSkin() const { return *_skin; }
          FontSkin& GetSkin()       { return *_skin; }

private:
    VirtualValue<FontSkin> _skin;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_FONT__H__
