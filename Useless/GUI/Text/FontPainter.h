#ifndef __INCLUDED_USELESS_FONT_PAINTER__H__
#define __INCLUDED_USELESS_FONT_PAINTER__H__

#include "Useless/GUI/Text/Font.h"
#include "Useless/GUI/Text/WordSelector.h"
#include "Useless/GUI/Text/LineWrapper.h"
#include "Useless/GUI/Text/WordStatus.h"

namespace Useless {

class USELESS_API FontPainter
{
public:
    FontPainter() {}
    FontPainter( const Font &font, const Text &text="", int width=0 );
    ~FontPainter() {}

    enum { LEFT=0, RIGHT=1, CENTER=2 };
    void Paint( const Painter &painter, Pos p=Pos(), int line_space=0, int alignment=0 );

    Text GetText  ()          const { return _text; }
     int GetWidth ()          const { return _lines.GetWidth(); }
     int GetHeight(int line_space=0) const;

   const Font& GetFont  ()    const { return _font; }

    void SetText  ( const Text &text );
    void SetFont  ( const Font &font );
    void SetWidth ( int width );

private:
    typedef WordSelector _Words;
    typedef LineWrapper< WordSelector > _Lines;
    typedef WordStatus< _Words > _Status;

    Font   _font;
    Text   _text;
    _Words  _words;
    _Lines  _lines;
    _Status _status;

public:
    typedef _Status Status;
    typedef _Words  Words;
    typedef _Lines  Lines;

    const Status & GetStatus() const { return _status; }
    const Lines  & GetLines() const { return _lines; }
    const Words  & GetWords() const { return _words; }

    struct Carrige
    {
        int _letterInText;
        int _letterInWord;
        int _wordInText;
        int _wordInRow;
        int _row;
    };

    Carrige QueryPosition( const Pos &pos, int line_space=0, int alignment=0 );
};

    
};//namespace Useless
#endif//__INCLUDED_USELESS_FONT_PAINTER__H__
