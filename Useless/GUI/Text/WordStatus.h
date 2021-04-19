#ifndef __INCLUDED__USELESS__WORD_STATUS_H__
#define __INCLUDED__USELESS__WORD_STATUS_H__

#include "Useless/GUI/Text/TextInfo.h"
#include "Useless/GUI/Text/Text.h"

namespace Useless {

/*! WordStatus calculates statistics for text
 */
template< class WordStorage >
class WordStatus
{
public:
    WordStatus(): _word_count(0), _lines_count(0), _width(0), _height(0) {}
    WordStatus( const WordStorage &word_storage, const Text &text );

    int GetWordCount()  const { return _word_count; }
    int GetLinesCount() const { return _lines_count; }
    int GetWidth()      const { return _width; }
    int GetHeight()     const { return _height; }

private:
    int _word_count;
    int _lines_count;
    int _width;
    int _height;
};
//-------------------------------------------------------------------------

template< class WordStorage >
WordStatus<WordStorage>::WordStatus( const WordStorage &word_storage, const Text &text )
    : _word_count(0), _lines_count(0), _width(0), _height(0)
{
    using namespace TextInfo;
    typedef typename WordStorage::ConstIterator ConstIterator;
    ConstIterator word_iterator = word_storage.ConstBegin();
    int w=0, h=0;
    Text separators = word_storage.GetSeparators();

    while( !!word_iterator )
    {
        Word word = *word_iterator++;
        ++_word_count;
        w += word.width;
        h = ( h < word.height )? word.height : h;


        UniCode sepa = text[word.end];
        if ( sepa==separators[TextInfo::NEW_LINE] || !word_iterator )
        {
            ++_lines_count;
            _width = (_width < w)? w : _width;
            _height += h;
            w=0; h=0;
        }
        else
        {
            w += word.separator_width;
        }
    }
}


};//namespace Useless
#endif//__INCLUDED__USELESS__WORD_STATUS_H__
