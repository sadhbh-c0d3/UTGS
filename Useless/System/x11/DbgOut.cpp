//Debug out X11 version (by koolas)

#include <vector>
#include <stdio.h>
#include <stdarg.h>

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
        memset( &mem[0], 0L, size); 
        n = vsnprintf( (char*)(&mem[0]), size, pchar, args );
        size *= 2;
    } 
    fwrite( &mem[0], 1, mem.size(), stdout);
}

};//namespace Useless
