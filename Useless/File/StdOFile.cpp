/*
**  $Id: StdOFile.cpp,v 1.11 2005/01/05 19:18:46 koolas Exp $
**
**  NAME
**      StdOFile
**
**  PURPOSE
**      Output file using standarc C file io.
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "UselessPch.h"

#include "Useless/File/StdOFile.h"
#include "Useless/Error/IOError.h"
#include "Useless/System/Assert.h"


namespace Useless {


StdOFile::StdOFile( const std::string &name, bool removeonfail )
    : _name(name), _removeonfail(removeonfail)
{
    if ( name == "-" )
    {
        _file = stdout;
    }
    _file = fopen(_name.c_str(),"wb");
    if(! _file) { throw IOError("Failed to open/create file '%s'.",_name.c_str()); }
}


StdOFile::~StdOFile() throw()
{
    if(_file)
    {
        fclose(_file);
        if(_removeonfail) { remove(_name.c_str()); }
    }
}


int StdOFile::WriteUpto( const void *buffer, int size )
{
    assert(_file);
    assert(buffer);
    assert(size>=0);
    size_t writelen = fwrite( buffer, 1, size, _file );
    if(ferror(_file)) { throw IOError("Write error on file '%s'.",_name.c_str()); }
    return writelen;
}


void StdOFile::Seek( Types::QUAD offset, SeekMode mode )
{
    assert(_file);
    int origin;
    if     (mode==SEEKSET) { origin=SEEK_SET; }
    else if(mode==SEEKCUR) { origin=SEEK_CUR; }
    else if(mode==SEEKEND) { origin=SEEK_END; }
    else { assert(0); }

    if(fseek(_file,offset,origin)!=0) { throw IOError("Failed to seek in file '%s'.",_name.c_str()); }
}


Types::QUAD StdOFile::GetLen()
{
    assert(_file);
    long curp = ftell(_file);
    if(curp==-1) { throw IOError("Failed to get current position in file '%s'.",_name.c_str()); }
    if(fseek(_file,0,SEEK_END)!=0) { throw IOError("Failed to seek in file '%s'.",_name.c_str()); }
    long len = ftell(_file);
    if(len==-1) { throw IOError("Failed to get current position in file '%s'.",_name.c_str()); }
    if(fseek(_file,curp,SEEK_SET)!=0) { throw IOError("Failed to seek in file '%s'.",_name.c_str()); }
    return len;
}

Types::QUAD StdOFile::GetPos()
{
    /*
    fpos_t file_position;
    fgetpos( _file, &file_position ); // fgetpos uses int64 - so it counts position in bytes
    */
    return ftell( _file);
}


const std::string &StdOFile::GetName()
{
    return _name;
}


void StdOFile::Close()
{
    bool failed = fclose(_file);
    _file = 0;
    if(failed) { throw IOError("Failed to properly close file '%s'.",_name.c_str()); }
}



} // namespace Useless

