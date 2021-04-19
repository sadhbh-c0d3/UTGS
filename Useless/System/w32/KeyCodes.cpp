#include "UselessPch.h"

#include "KeyCodes.h"
#include "Useless/System/Assert.h"

const Useless::Keys::CodeMap &
Useless::Keys::GetCodeMap()
{
    static std::map< std::string, int > codes;

    if ( codes.empty() )
    {
        codes[ "ESC" ] = ESC;
        codes[ "F1" ] = F1;

        codes[ "F2" ] = F2;
        codes[ "F3" ] = F3;
        codes[ "F4" ] = F4;
        codes[ "F5" ] = F5;
        codes[ "F6" ] = F6;
        codes[ "F7" ] = F7;
        codes[ "F8" ] = F8;
        codes[ "F9" ] = F9;
        codes[ "F10" ] = F10;
        codes[ "F11" ] = F11;
        codes[ "F12" ] = F12;

        codes[ "TILDE" ] = TILDE;
        codes[ "ONE" ] = ONE;
        codes[ "TWO" ] = TWO;
        codes[ "THREE" ] = THREE;
        codes[ "FOUR" ] = FOUR;
        codes[ "FIVE" ] = FIVE;
        codes[ "SIX" ] = SIX;
        codes[ "SEVEN" ] = SEVEN;
        codes[ "EIGHT" ] = EIGHT;
        codes[ "NINE" ] = NINE;
        codes[ "ZERO" ] = ZERO;
        codes[ "MINUS" ] = MINUS;
        codes[ "EQUAL" ] = EQUAL;

        //-- Backspace
        codes[ "BS" ] = BS;

        //-- QWERTY (second row)
        codes[ "TAB" ] = TAB;
        codes[ "Q" ] = Q;
        codes[ "W" ] = W;
        codes[ "E" ] = E;
        codes[ "R" ] = R;
        codes[ "T" ] = T;
        codes[ "Y" ] = Y;
        codes[ "U" ] = U;
        codes[ "I" ] = I;
        codes[ "O" ] = O;
        codes[ "P" ] = P;
        codes[ "LBRACE" ] = LBRACE;
        codes[ "RBRACE" ] = RBRACE;

        //-- Return
        codes[ "RET" ] = RET;

        //-- ASDFG (third row)
        codes[ "CAPS" ] = CAPS;
        codes[ "A" ] = A;
        codes[ "S" ] = S;
        codes[ "D" ] = D;
        codes[ "F" ] = F;
        codes[ "G" ] = G;
        codes[ "H" ] = H;
        codes[ "J" ] = J;
        codes[ "K" ] = K;
        codes[ "L" ] = L;
        codes[ "COLON" ] = COLON;
        codes[ "QUOTE" ] = QUOTE;
        codes[ "BSLASH" ] = BSLASH;
        codes[ "LSHIFT" ] = LSHIFT;

        //-- ZXCVB (fourth row)
        codes[ "    LBSLASH" ] = //<- " ; idiotic left backslash
            codes[ "Z" ] = Z;
        codes[ "X" ] = X;
        codes[ "C" ] = C;
        codes[ "V" ] = V;
        codes[ "B" ] = B;
        codes[ "N" ] = N;
        codes[ "M" ] = M;
        codes[ "COMA" ] = COMA;
        codes[ "DOT" ] = DOT;
        codes[ "SLASH" ] = SLASH;
        codes[ "RSHIFT" ] = RSHIFT;

        //-- Control keys
        codes[ "LCTRL" ] = LCTRL;
        codes[ "LWINDOWS" ] = LWINDOWS;
        codes[ "SPACE" ] = SPACE;
        codes[ "RALT" ] = RALT;
        codes[ "RWINDOWS" ] = RWINDOWS;
        codes[ "MENU" ] = MENU;
        codes[ "RCTRL" ] = RCTRL;

        //-- Arrow keys
        codes[ "LEFT" ] = LEFT;
        codes[ "DOWN" ] = DOWN;
        codes[ "RIGHT" ] = RIGHT;
        codes[ "UP" ] = UP;

        //-- Editor keys
        codes[ "INS" ] = INS;
        codes[ "DEL" ] = DEL;
        codes[ "HOME" ] = HOME;
        codes[ "END" ] = END;
        codes[ "PGUP" ] = PGUP;
        codes[ "PGDN" ] = PGDN;

        //-- NumPad keys
        codes[ "NUMLOCK" ] = NUMLOCK;
        codes[ "GREY_DIV" ] = GREY_DIV;
        codes[ "GREY_MUL" ] = GREY_MUL;
        codes[ "GREY_MINUS" ] = GREY_MINUS;
        codes[ "GREY_PLUS" ] = GREY_PLUS;
        codes[ "NUM7" ] = NUM7;
        codes[ "NUM8" ] = NUM8;
        codes[ "NUM9" ] = NUM9;
        codes[ "NUM4" ] = NUM4;
        codes[ "NUM5" ] = NUM5;
        codes[ "NUM6" ] = NUM6;
        codes[ "NUM1" ] = NUM1;
        codes[ "NUM2" ] = NUM2;
        codes[ "NUM3" ] = NUM3;
        codes[ "NUM0" ] = NUM0;
        codes[ "GREY_DOT" ] = GREY_DOT;
        codes[ "GREY_ENTER" ] = GREY_ENTER;

        //-- Other keys
        codes[ "PRINT_SCR" ] = PRINT_SCR;
        codes[ "SCROLL_LOCK" ] = SCROLL_LOCK;
        codes[ "PAUSE" ] = PAUSE;

        codes[ "NONE" ] = 0;
    }

    return codes;
}

int
Useless::Keys::GetCode( const std::string &name )
{
    const CodeMap &codes = GetCodeMap();
    C_assert( codes.find( name ) != codes.end () );
    return codes.find( name )->second;
}
