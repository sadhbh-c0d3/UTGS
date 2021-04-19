#ifndef __INCLUDED_USELESS_FONTSET_H__
#define __INCLUDED_USELESS_FONTSET_H__

#include "Useless/GUI/Text/Font.h"
#include "Useless/Util/Storage/AutoVector.h"

namespace Useless {

class USELESS_API FontSet
{
public:
              FontSet();
              FontSet( const Font &font, int id=0 );
     virtual ~FontSet();

         int  Insert( const Font &font );
        void  Insert( int id, const Font &font );

         int  Insert( const char *gfx_fname, const char *table_fname, int spacing=0 );
         int  Insert( const FontSkin &font_skin, int spacing=0 );

        void  Remove( int id );

         int  Size()           const        { return   _fonts.Size();  }
        bool  Empty()          const        { return   _fonts.Empty(); }
        bool  Exists( int id ) const        { return !!_fonts.Find(id);}

        Font &GetFont( int id )             { return *_fonts.Find(id); }
  const Font &GetFont( int id ) const       { return *_fonts.Find(id); }

        void  Paint(       int            id, 
                     const Painter       &painter,
                     const Text          &text,
                     const Pos           &pos     =Pos()
                   )                        { _fonts[id].Paint(painter,text,pos); }

         int  GetWidth ( int id, const Text& text) const { return _fonts[id].GetWidth(text); }
         int  GetWidth ( int id, UniCode code)     const { return _fonts[id].GetWidth(code); }
         int  GetHeight( int id)                   const { return _fonts[id].GetHeight();    }


        Font &GetFont()                     { return *_fonts.Begin(); }
  const Font &GetFont() const               { return *_fonts.ConstBegin(); }

private:
    typedef AssocVector<int, Font > FontMap;
    AutoVector< Font>               _fonts;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_FONTSET_H__
