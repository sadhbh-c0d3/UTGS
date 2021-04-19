/*
**  $Id: StdIFile.cpp,v 1.9 2005/01/05 19:18:46 koolas Exp $
**
**  NAME
**      StdIFile
**
**  PURPOSE
**      Input file using standarc C file io.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "UselessPch.h"

#include "Useless/File/StdIFile.h"
#include "Useless/Error/IOError.h"
#include "Useless/System/Assert.h"


namespace Useless {


StdIFile::StdIFile( const std::string &name )
{
    _name = name;
    if ( name == "-" )
    {
        _file = stdin;
    }
    else
    {
        _file = fopen(_name.c_str(),"rb");
    }
    if(! _file) { throw IOError("Failed to open file '%s'.",_name.c_str()); }
}


StdIFile::~StdIFile() throw()
{
    if(_file) { fclose(_file); }
}


int StdIFile::ReadUpto( void *buffer, int size )
{
    assert(buffer);
    assert(size>=0);
    size_t readlen = fread( buffer, 1, size, _file );
    if(ferror(_file)) { throw IOError("Read error on file '%s'.",_name.c_str()); }
    return readlen;
}


void StdIFile::Seek( Types::QUAD offset, SeekMode mode )
{
    int origin;
    if     (mode==SEEKSET) { origin=SEEK_SET; }
    else if(mode==SEEKCUR) { origin=SEEK_CUR; }
    else if(mode==SEEKEND) { origin=SEEK_END; }
    else { assert(0); }

    if(fseek(_file,offset,origin)!=0) { throw IOError("Failed to seek in file '%s'.",_name.c_str()); }
}


Types::QUAD StdIFile::GetLen()
{
    long curp = ftell(_file);
    if(curp==-1) { throw IOError("Failed to get current position in file '%s'.",_name.c_str()); }
    if(fseek(_file,0,SEEK_END)!=0) { throw IOError("Failed to seek in file '%s'.",_name.c_str()); }
    long len = ftell(_file);
    if(len==-1) { throw IOError("Failed to get current position in file '%s'.",_name.c_str()); }
    if(fseek(_file,curp,SEEK_SET)!=0) { throw IOError("Failed to seek in file '%s'.",_name.c_str()); }
    return len;
}

Types::QUAD StdIFile::GetPos()
{
    /*
    fpos_t file_position;
    fgetpos( _file, &file_position ); // fgetpos uses int64 - so it counts position in bytes
    */
    return ftell(_file);
}

const std::string &StdIFile::GetName()
{
    return _name;
}


} // namespace Useless

