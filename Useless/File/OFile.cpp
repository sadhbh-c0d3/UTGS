/*
**  $Id: OFile.cpp,v 1.9 2003/01/20 21:42:21 koolas Exp $
**
**  NAME
**      OFile
**
**  PURPOSE
**      Output file abstract class.
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "UselessPch.h"

#include "Useless/File/OFile.h"
#include "Useless/Error/IOError.h"


namespace Useless {


void OFile::Write( const void *buffer, int size )
{
    if( WriteUpto(buffer,size) != size ) { throw IOError("Unexpected end of file '%s'.",GetName().c_str()); }
}


void OFile::WriteByte( Types::UBYTE a )
{
    Write( &a, 1 );
}


void OFile::WriteWordHE( Types::UWORD w )
{
    Types::UBYTE a[2];
    a[0] = (Types::UBYTE)(w>>8);
    a[1] = (Types::UBYTE)(w);
    Write( a, 2 );
}


void OFile::WriteLongHE( Types::ULONG w )
{
    Types::UBYTE a[4];
    a[0] = (Types::UBYTE)(w>>24);
    a[1] = (Types::UBYTE)(w>>16);
    a[2] = (Types::UBYTE)(w>> 8);
    a[3] = (Types::UBYTE)(w);
    Write( a, 4 );
}


void OFile::WriteQuadHE( Types::UQUAD w )
{
    Types::UBYTE a[8];
    a[0] = (Types::UBYTE)(w>>56);
    a[1] = (Types::UBYTE)(w>>48);
    a[2] = (Types::UBYTE)(w>>40);
    a[3] = (Types::UBYTE)(w>>32);
    a[4] = (Types::UBYTE)(w>>24);
    a[5] = (Types::UBYTE)(w>>16);
    a[6] = (Types::UBYTE)(w>> 8);
    a[7] = (Types::UBYTE)(w);
    Write( a, 8 );
}


void OFile::WriteWordLE( Types::UWORD w )
{
    Types::UBYTE a[2];
    a[0] = (Types::UBYTE)(w);
    a[1] = (Types::UBYTE)(w>>8);
    Write( a, 2 );
}


void OFile::WriteLongLE( Types::ULONG w )
{
    Types::UBYTE a[4];
    a[0] = (Types::UBYTE)(w);
    a[1] = (Types::UBYTE)(w>> 8);
    a[2] = (Types::UBYTE)(w>>16);
    a[3] = (Types::UBYTE)(w>>24);
    Write( a, 4 );
}


void OFile::WriteQuadLE( Types::UQUAD w )
{
    Types::UBYTE a[8];
    a[0] = (Types::UBYTE)(w);
    a[1] = (Types::UBYTE)(w>> 8);
    a[2] = (Types::UBYTE)(w>>16);
    a[3] = (Types::UBYTE)(w>>24);
    a[4] = (Types::UBYTE)(w>>32);
    a[5] = (Types::UBYTE)(w>>40);
    a[6] = (Types::UBYTE)(w>>48);
    a[7] = (Types::UBYTE)(w>>56);
    Write( a, 8 );
}



} // namespace Useless

