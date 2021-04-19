#include "UselessPch.h"

#include "Useless/GUI/Text/WordSelector.h"
#include <algorithm>

namespace Useless {

WordSelector::WordSelector( const Font &font, const Text &text, const Text &separators ): _separators(separators)
{
    StupidVector<int> word_separators;
    int i,n,jb,je;

    for ( i=0, n=separators.size(); i<n; ++i )
    {
        if ( separators[i]>0 )  { text.Match( separators[i], word_separators); }
    }

    std::sort( word_separators.begin(), word_separators.end() );
    int pixelsSinceNL = 0;
    int spWidth = font.GetWidth(separators[TextInfo::SPACE]);
    int tabWidth = spWidth * 4;

    for ( i=0, jb=0, je=0, n=word_separators.size(); i<n; ++i )
    {
        je = word_separators[i];

        if ( jb<je )
        {
            Word word( font.GetWidth(text(jb,je)), font.GetHeight(), jb, je, 0);
            
            UniCode sep_code = text[je];

            if ( sep_code==separators[TextInfo::NEW_LINE] )
            {
                word.separator_width = -1;
                pixelsSinceNL = 0;
            }

            else if ( sep_code==separators[TextInfo::TAB] )
            {
                pixelsSinceNL += word.width;
                int numTabStops = pixelsSinceNL / tabWidth;                
                word.separator_width = (numTabStops + 1) * tabWidth - pixelsSinceNL;            
                pixelsSinceNL += word.separator_width;
            }

            else
            {
                word.separator_width = font.GetWidth(sep_code);
                pixelsSinceNL += word.width + word.separator_width;
            }

            _words.Insert(word);
            jb = je+1;
        }
    }

    if ( jb < (je=text.length()) )
    {
        Word word( font.GetWidth(text(jb,je)), font.GetHeight(), jb, je, 0);
        _words.Insert(word);
    }

}

};//namespace Useless
