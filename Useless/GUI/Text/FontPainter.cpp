#include "UselessPch.h"

#include "Useless/GUI/Text/FontPainter.h"

namespace Useless {

FontPainter::FontPainter(const Font &font, const Text &text, int width )
    :_font(font), _text(text)
{
    _words = WordSelector( _font, _text );
    _lines = LineWrapper<WordSelector>( _words, width, _text );
    _status = Status( _words, _text );
}

void FontPainter::Paint( const Painter &painter, Pos p, int line_space, int alignment  )
{
    line_space += GetFont().GetSkin().GetSpacing();
    for ( int i=0, n=_lines.GetNumRows(); i<n; ++i )
    {
        const TextInfo::Row &row=_lines.GetRow(i);
        Pos pj;

        switch (alignment)
        {
            case LEFT:
                pj = p; break;

            case RIGHT:
                pj = p - Pos(row.width,0); break;

            case CENTER:
                pj = p - Pos(row.width/2,0); break;
        };

        for ( int j=0, m=row.words.Size(); j<m; ++j )
        {
            const TextInfo::Word &word = _words.GetWord( row.words[j] );
            _font.Paint( painter, _text(word.begin, word.end), pj );
            pj.x += word.width + ((word.separator_width>0)? word.separator_width : 0);
        }
        p += Pos(0, row.height + line_space);
    }
}

void FontPainter::SetText ( const Text &text )
{
    _text = text;
    _words = WordSelector( _font, _text );
    _lines = LineWrapper<WordSelector>( _words, GetWidth(), _text );
    _status = Status( _words, _text );
}

void FontPainter::SetFont ( const Font &font )
{
    _font = font;
    _words = WordSelector( _font, _text );
    _lines = LineWrapper<WordSelector>( _words, GetWidth(), _text );
    _status = Status( _words, _text );
}


void FontPainter::SetWidth( int width )
{
    _lines = LineWrapper<WordSelector>( _words, width, _text );
}

int FontPainter::GetHeight(int line_space) const
{
    line_space += GetFont().GetSkin().GetSpacing();
    int spaces = _lines.GetNumRows()-1;
    return _lines.GetHeight() + (spaces>0 ? spaces*line_space : 0);
}

FontPainter::Carrige FontPainter::QueryPosition( const Pos &pos, int line_space, int alignment )
{
    Carrige badCarrige;
    badCarrige._letterInText = -1;
    badCarrige._letterInWord = -1;
    badCarrige._wordInText = -1;
    badCarrige._wordInRow = -1;
    badCarrige._row = -1;

    Carrige carrige;
    carrige._letterInText=0;
    carrige._letterInWord=0;
    carrige._wordInText=0;
    carrige._wordInRow=0;
    carrige._row=0;

    Pos p(0,0);

    line_space += GetFont().GetSkin().GetSpacing();

    for ( int i=0, n=_lines.GetNumRows(); i<n; ++i )
    {
        const TextInfo::Row &row=_lines.GetRow(i);
        Pos pj;

        switch (alignment)
        {
            case LEFT:
                pj = p; break;

            case RIGHT:
                pj = p - Pos(row.width,0); break;

            case CENTER:
                pj = p - Pos(row.width/2,0); break;
        };

        if (( pj.y < pos.y ) && !( pj.y + row.height < pos.y ))
        {
            carrige._row = i;
            
            for ( int j=0, m=row.words.Size(); j<m; ++j )
            {
                const TextInfo::Word &word = _words.GetWord( row.words[j] );

                if (( pj.x < pos.x ) && !( pj.x + word.width < pos.x ))
                {
                    int offset = pos.x - pj.x;
                    int cumOffset = 0;
                    int letter=0;

                    FontSkin::Letters letters=GetFont().GetStats(_text(word.begin, word.end));

                    for ( FontSkin::Letters::ConstIterator l = letters.ConstBegin(); !!l; ++l )
                    {
                        if (( cumOffset < offset ) && !( cumOffset + *l < offset ))
                        {
                            carrige._letterInWord = letter;
                        }
                        cumOffset += *l;
                        letter++;
                    }


                    carrige._wordInText += j;
                    carrige._wordInRow = j;
                    carrige._letterInText = word.begin + carrige._letterInWord;
            
                    return carrige;
                }


                pj.x += word.width + ((word.separator_width>0)? word.separator_width : 0);
            }

            return badCarrige;
        }


        p += Pos(0, row.height + line_space);
        carrige._wordInText += row.words.Size();
    }

    return badCarrige;
}

};//namespace Useless
