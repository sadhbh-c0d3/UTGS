#ifndef __INCLUDED_USELESS_WORD_SELECTOR_H__
#define __INCLUDED_USELESS_WORD_SELECTOR_H__

#include "Useless/GUI/Text/TextInfo.h"
#include "Useless/GUI/Text/Font.h"

namespace Useless {

/*! WordSelector finds words inside Text using delivered separators.
 */
class USELESS_API WordSelector
{
public:
    WordSelector() {}
    WordSelector( const Font &font, const Text &text, const Text &separators=" \t\n" );

    typedef TextInfo::Word Word;
    const Word& GetWord      (int n) const { assert(n>=0 && n<_words.Size()); return _words[n]; }
           int  GetNumWords  ()      const { return _words.Size();}
    const Text& GetSeparators()      const { return _separators; }

private:
    typedef StupidVector<Word> WordTable;

    WordTable   _words;
         Text   _separators;

public:
    typedef WordTable::ConstIterator ConstIterator;

    ConstIterator  ConstBegin() const { return _words.ConstBegin(); }
    ConstIterator  ConstEnd()   const { return _words.ConstEnd();   }

};


};//namespace Useless
#endif//__INCLUDED_USELESS_WORD_SELECTOR_H__
