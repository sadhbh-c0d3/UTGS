//Debug Out windows version (by koolas)
#include "UselessPch.h"

#include <windows.h>
#include <vector>
#include <algorithm>

namespace Useless {

void print_dbg( const char *pchar, ...)
{
    va_list args;
    va_start( args, pchar );
    int size = strlen( pchar ) * 2 + 1;
    int n = -1;
    std::vector<char> mem;
    
    while ( n == -1 )
    {
        mem.resize( size );
        memset( (char*)&*mem.begin(), 0L, size); 
        n = _vsnprintf( (char*)&*mem.begin(), size, pchar, args );
        size *= 2;
    } 

    std::basic_string<TCHAR> out;
    std::copy( mem.begin(), mem.end(), std::back_inserter( out));
            
    OutputDebugString( out.c_str());
}

};//namespace Useless
