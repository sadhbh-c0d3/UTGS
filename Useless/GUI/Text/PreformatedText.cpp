#include "UselessPch.h"
#pragma warning (disable:4786)
#include "Useless/GUI/Text/PreformatedText.h"
#include "Useless/Util/str_conv.h"


namespace Useless {
/*! PreformatedText is a text with marked-up font style
 *
 * Markup syntax:
 *
 * "\020[tags and attributes]text data"
 *
 * Tags: b-bold, i-italic, u-underline, n-invert colors. Use /b,/i,/u,/n to reset tags.
 *
 * Attributes: c:0x000000-color hex, s:0-spacing decimal, d:0-pixel distance decimal
 *
 */

PreformatedText::PreformatedText()
{}

PreformatedText::PreformatedText( const Text &text ): Text(text)
{
    Markup mark;

    for ( int i=0, n=text.length(); ; mark.start = ++i )
    {
        for ( ; text[i]!=020 && i<n; ++i ) {}

        mark.end = i;
        _markups.Insert( mark );

        if (i==n) { break; }

        if ( text[i]==020 )
        {
            if ( ++i >= n ) { throw Error("Bad markup ending \"%s\"", (const char*)&*begin() ); };
  
            if ( text[i]!='[' ) { throw Error("Missing markup '[' \"%s\"", (const char*)&*begin() ); };
            
            enum { READ_TAG, READ_COLOR, READ_BG_COLOR, READ_KERNING } data_mode=READ_TAG;
            std::string data;
            
            for ( ; text[i]!=']' && i<n; ++i )
            {
                if ( data_mode==READ_TAG )
                {
                    switch ( text[i] )
                    {
                    case 'b': mark.style |= FontSkin::BOLD; mark.f_style=1; break;
                    case 'i': mark.style |= FontSkin::ITALIC; mark.f_style=1; break;
                    case 'u': mark.style |= FontSkin::UNDERLINE; mark.f_style=1; break;
                    case 'n': mark.style |= FontSkin::INVERT; mark.f_style=1; break;
                    case '/':
                    {
                        if ( ++i >= n ) { throw Error("Bad markup ending \"%s\"", (const char*)&*begin() ); };
                        switch( text[i] )
                        {
                        case 'b': mark.style &= ~FontSkin::BOLD; mark.f_style=1; break;
                        case 'i': mark.style &= ~FontSkin::ITALIC; mark.f_style=1; break;
                        case 'u': mark.style &= ~FontSkin::UNDERLINE; mark.f_style=1; break;
                        case 'n': mark.style &= ~FontSkin::INVERT; mark.f_style=1; break;
						case 'c': mark.f_color=2; break;
                        }
                        break;
                    }
                    case 'c':
                        if ( ++i >= n || text[i]!=':')
                            { throw Error("Bad markup missing colon \"%s\"", (const char*)&*begin() ); };
                        data_mode=READ_COLOR;
                        break;

                    case 'g':
                        if ( ++i >= n || text[i]!=':')
                            { throw Error("Bad markup missing colon \"%s\"", (const char*)&*begin() ); };
                        data_mode=READ_BG_COLOR;
                        break;

                    case 'k':
                        if ( ++i >= n || text[i]!=':')
                            { throw Error("Bad markup missing colon \"%s\"", (const char*)&*begin() ); };
                        data_mode=READ_KERNING;
                        break;

                    case ' ': break;
                    default:
                        assert("PreformatedText: error bad tag");
                    }
                }
                else
                {
                    while ( text[i] != ' ' && text[i] != ']' ) { data += text[i++]; } --i;
                    switch( data_mode )
                    {
                        case READ_COLOR:
                            mark.fg_color=to_hex(data); mark.f_color=1;
                            break;

                        case READ_BG_COLOR:
                            mark.bg_color=to_hex(data); mark.f_backdrop=1;
                            break;

                        case READ_KERNING:
                            mark.kerning=to_integer(data); mark.f_kerning=1;
                            break;
                    };
                    data_mode = READ_TAG;
                    data = std::string();
                }
            }

            if ( text[i]!=']' ) { throw Error("Missing markup ']' \"%s\"", (const char*)&*begin() ); };
        }
    }
}

PreformatedText::~PreformatedText()
{}

};//namespace Useless
