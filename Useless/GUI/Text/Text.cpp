#include "UselessPch.h"

#include "Useless/GUI/Text/Text.h"
#include "Useless/Util/Storage/MemBlock.h"

#ifndef WIN32
#   include <stdio.h>
#   include <stdarg.h>
#   define _vsnprintf vsnprintf
#   define _vsnwprintf vswprintf
#endif

namespace Useless {

Text::Text( const char  *pchar,  ... )
{
    va_list args;
    va_start( args, pchar );
    int size = strlen( pchar ) * 2;
    int n = -1;
    MemBlock mem;
    
    while ( n == -1 )
    {
        mem.Alloc( (size + 1) * sizeof(char) );
        mem.Clear();
        n = _vsnprintf( (char*)mem.GetPtr(), size, pchar, args );
        size <<= 1;
    } 

    MakeText( mem.GetPtrUB() );
}

Text::Text( const wchar_t *pshort, ... )
{
#ifdef WIN32
    va_list args;
    va_start( args, pshort );
    int size = wcslen( pshort ) * 2;
    int n = -1;
    MemBlock mem;

    while ( n == -1 )
    {
        mem.Alloc( (size + 1) * sizeof(wchar_t) );
        mem.Clear();
        n = _vsnwprintf( (wchar_t*)mem.GetPtr(), size, pshort, args );
        size <<= 1;
    }

    MakeText( mem.GetPtrUW() );
#endif
}

int Text::Match(const Text& pattern, StupidVector<int> &matches) const
{
    int p=0;
    while ( ( p=find( pattern, p) ) > -1 )
    {
        matches.Insert(p++);
    }
    return matches.Size();
}

Text& Text::Replace(int start, int end, const Text &replacement)
{
    replace( start, end>-1 ? end-start: -1, replacement, 0, -1 );
    return *this;
}

Text& Text::Cut(int start, int end)
{
    erase( start, end);
    return *this;
}

Text& Text::Paste(int pos, const Text &text)
{
    insert( pos, text);
    return *this;
}

char toChar( UniCode c )
{
    return (char)c;
}

//Conversion to default string types
std::string Text::GetString() const
{
    std::string str;
    str.resize( size() );
    std::transform( begin(), end(), str.begin(), toChar );
    return str;
}

char toWchar( UniCode c )
{
    return (wchar_t)c;
}

//Conversion to default string types
std::basic_string<short> Text::GetWString() const
{
    std::basic_string<short> str;
    str.resize( size() );
    std::transform( begin(), end(), str.begin(), toWchar );
    return str;
}



};//namespace Useless
