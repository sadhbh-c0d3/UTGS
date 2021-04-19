/*
**  $Id: IFile.cpp,v 1.8 2003/01/20 21:42:21 koolas Exp $
**
**  NAME
**      IFile
**
**  PURPOSE
**      Input file abstract class.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "UselessPch.h"

#include "Useless/File/IFile.h"
#include "Useless/Error/IOError.h"


namespace Useless {


void IFile::Read( void *buffer, int size )
{
    if( ReadUpto(buffer,size) != size ) { throw IOError("Unexpected end of file '%s'.",GetName().c_str()); }
}


Types::BYTE IFile::ReadByte()
{
    Types::UBYTE a[1];
    Read( a, 1 );
    return Types::BYTE(a[0]);
}


Types::WORD IFile::ReadWordHE()
{
    Types::UBYTE a[2];
    Read( a, 2 );
    return Types::WORD( (a[0]<<8) | (a[1]) );
}


Types::LONG IFile::ReadLongHE()
{
    Types::UBYTE a[4];
    Read( a, 4 );
    return Types::LONG( (a[0]<<24) | (a[1]<<16) | (a[2]<<8) | (a[3]) );
}


Types::QUAD IFile::ReadQuadHE()
{
    Types::UBYTE a[8];
    Read( a, 8 );
    return Types::QUAD( (Types::UQUAD(a[0])<<56) | (Types::UQUAD(a[1])<<48) | (Types::UQUAD(a[2])<<40) | (Types::UQUAD(a[3])<<32)
                      | (Types::UQUAD(a[4])<<24) | (Types::UQUAD(a[5])<<16) | (Types::UQUAD(a[6])<< 8) |  Types::UQUAD(a[7]) );
}


Types::WORD IFile::ReadWordLE()
{
    Types::UBYTE a[2];
    Read( a, 2 );
    return Types::WORD( (a[1]<<8) | (a[0]) );
}


Types::LONG IFile::ReadLongLE()
{
    Types::UBYTE a[4];
    Read( a, 4 );
    return Types::LONG( (a[3]<<24) | (a[2]<<16) | (a[1]<<8) | (a[0]) );
}


Types::QUAD IFile::ReadQuadLE()
{
    Types::UBYTE a[8];
    Read( a, 8 );
    return Types::QUAD( (Types::UQUAD(a[7])<<56) | (Types::UQUAD(a[6])<<48) | (Types::UQUAD(a[5])<<40) | (Types::UQUAD(a[4])<<32)
                      | (Types::UQUAD(a[3])<<24) | (Types::UQUAD(a[2])<<16) | (Types::UQUAD(a[1])<< 8) |  Types::UQUAD(a[0]) );
}


} // namespace Useless

