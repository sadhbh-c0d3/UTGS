#ifndef __INCLUDED__USELESS__PREFORMATED_TEXT_H__
#define __INCLUDED__USELESS__PREFORMATED_TEXT_H__

#include "Useless/GUI/Text/Text.h"
#include "Useless/GUI/Skins/FontSkin.h" //-- font flags


namespace Useless {

/*! PreformatedText is a text with marked-up font style
 *
 * Markup syntax:
 *
 * "\020[tags and attributes]text data"
 *
 * Tags: b-bold, i-italic, u-underline, n-invert colors. Use /b,/i,/u,/n to reset tags.
 *
 * Attributes: c:0x000000-color hex, k:0-kerning decimal
 *
 * Exapmle strings:
 *
 * "\020[b]some text\020[/b]" - set bold
 *
 * "\020[i]some text\020[/i]" - set italic
 *
 * "\020[bu]some text\020[/b/u]" - set bold underline
 *
 * "\020[c:0x00FF00]some text" - set color to 0x00FF00
 *
 * "\020[s:10]some text" - set spacing
 *
 * "\020[d:10]some text" - add 10 pixel gap
 */
class USELESS_API PreformatedText : public Text
{
public:
    PreformatedText();
    PreformatedText( const Text &text );
    ~PreformatedText();

    enum { SET_FG=16, SET_BG=32 }; //additional flags

    struct Markup
    {
        Markup() { memset( this, 0, sizeof(Markup)); }

        /* range of text section */
        int          start;
        int          end;

        /* valid fields */
        unsigned f_style : 1,
                 f_color : 2,
                 f_backdrop : 1,
                 f_kerning : 1;

        /* fields */
        unsigned int style; // NORMAL, BOLD, ITALIC, UNDERLINE, INVERT
        NormalPixel  fg_color;
        NormalPixel  bg_color;
        int          kerning;
    };

private:
    typedef StupidVector<Markup> Markups;
    Markups _markups;

public:
    typedef Markups::ConstIterator ConstIterator;

    ConstIterator ConstBegin() const { return _markups.ConstBegin(); }
    ConstIterator ConstEnd()   const { return _markups.ConstEnd(); }
};

};//namespace Useless
#endif//__INCLUDED__USELESS__PREFORMATED_TEXT_H__
