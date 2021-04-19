#ifndef __INCLUDED__USELESS_TEXT_INFO_H__
#define __INCLUDED__USELESS_TEXT_INFO_H__

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'
#include "Useless/Util/Storage/StupidVector.h"

namespace Useless {

namespace TextInfo
{

typedef enum { SPACE=0, TAB=1, NEW_LINE=2 } WhiteChars;

/*! Describes where word begins and where it ends
 *  It also tells how long this word is using font.
 */
struct Word
{
    Word(int w, int h, int b, int e, int sw=0)
        :width(w), height(h), begin(b), end(e), separator_width(sw) {}

    int     begin;
    int     end; 
    int     width;
    int     height;
    int     separator_width;
};


/*! Describes row width, height and indices of words in this line
 */
struct Row
{
    Row(): height(0) {}

    int               width;
    int               height;
    StupidVector<int> words;
};



};//namespace TextInfo

};//namespace Useless
#endif//__INCLUDED__USELESS_TEXT_INFO_H__
