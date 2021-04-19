#ifndef __INCLUDED__USELESS_KEY_CODES_H__
#define __INCLUDED__USELESS_KEY_CODES_H__

// This file is NOT correct for X11

namespace Useless {
namespace Keys {

/* Keyboard scan codes in natural order
------------------------------------------*/
enum KeyCodes
{
//-- Escape
    ESC=1,

//-- Function keys on the upper side
    F1=59,
    F2=60,
    F3=61,
    F4=62,
    F5=63,
    F6=64,
    F7=65,
    F8=66,
    F9=67,
    F10=68, //<- this key doesn't seem to work on my computer
    F11=87,
    F12=88, //<- while debuging this key causes user-breakpoint in msvc

//-- Numeric (first row)
    TILDE=41,
    ONE=2,
    TWO=3,
    THREE=4,
    FOUR=5,
    FIVE=6,
    SIX=7,
    SEVEN=8,
    EIGHT=9,
    NINE=10,
    ZERO=11,
    MINUS=12,
    EQUAL=13,

//-- Backspace
    BS=14,

//-- QWERTY (second row)
    TAB=15,
    Q=16,
    W=17,
    E=18,
    R=19,
    T=20,
    Y=21,
    U=22,
    I=23,
    O=24,
    P=25,
    LBRACE=26,
    RBRACE=27,

//-- Return
    RET=28,

//-- ASDFG (third row)
    CAPS=58,
    A=30,
    S=31,
    D=32,
    F=33,
    G=34,
    H=35,
    J=36,
    K=37,
    L=38,
    COLON=39,
    QUOTE=40,
    BSLASH=43,
    LSHIFT=42,

//-- ZXCVB (fourth row)
    LBSLASH=86, //<- idiotic left backslash
    Z=44,
    X=45,
    C=46,
    V=47,
    B=48,
    N=49,
    M=50,
    COMA=51,
    DOT=52,
    SLASH=53,
    RSHIFT=54,

//-- Control keys
    LCTRL=29,
    LWINDOWS=91,
    SPACE=57,
    RALT=56,
    RWINDOWS=92,
    MENU=93,
    RCTRL=29 + 0x100,

//-- Arrow keys
    LEFT=75 + 0x100,
    DOWN=80 + 0x100,
    RIGHT=77 + 0x100,
    UP=72 + 0x100,

//-- Editor keys
    INS=82 + 0x100,
    DEL=83 + 0x100,
    HOME=71 + 0x100,
    END=79 + 0x100,
    PGUP=73 + 0x100,
    PGDN=81 + 0x100,

//-- NumPad keys
    NUMLOCK=69,
    GREY_DIV=53,
    GREY_MUL=55,
    GREY_MINUS=74,
    GREY_PLUS=78,
    NUM7=71,
    NUM8=72,
    NUM9=73,
    NUM4=75,
    NUM5=76,
    NUM6=77,
    NUM1=79,
    NUM2=80,
    NUM3=81,
    NUM0=82,
    GREY_DOT=83,
    GREY_ENTER=28,

//-- Other keys
    PRINT_SCR=55 + 0x100,
    SCROLL_LOCK=70,
    PAUSE=69 + 0x100 
};

inline bool IsChar( int key_code )
{
    return
    ( key_code>=ONE && key_code<=BS ) ||
    ( key_code>=Q && key_code<RET ) ||
    ( key_code>=A && key_code<LSHIFT ) ||
    ( key_code>=Z && key_code<RSHIFT ) ||
    ( key_code>=BSLASH && key_code<=RSHIFT ) ||
    ( key_code>=BSLASH && key_code<=RSHIFT ) ||
    ( key_code==SPACE );
}
    
};//namespace Keys
};//namespace Useless
#endif//__INCLUDED__USELESS_KEY_CODES_H__

