/*
**  $Id: MemIFile.cpp,v 1.8 2003/01/20 21:42:21 koolas Exp $
**
**  NAME
**      MemOFile
**
**  PURPOSE
**      Output data to memory using file-routine-interface
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "UselessPch.h"

#include "Useless/File/MemIFile.h"
#include "Useless/System/Assert.h"

namespace Useless {

MemIFile::MemIFile(const MemBlock & data) : _data(data), _p(0) {}

MemIFile::~MemIFile() throw() {}

int MemIFile::ReadUpto( void *buffer, int size )
{
    int n = ( size >= _data.GetSize()-_p )? _data.GetSize()-_p : size; assert(n>=0);
    if (n>0)
    { 
        memcpy(buffer,_data.GetConstPtrUB()+_p, n);
        _p = _p + n;
    }
    return n;
}

void MemIFile::Seek( Types::QUAD offset, SeekMode mode )
{
    switch(mode)
    {
    case SEEKSET:
        assert(offset>=0);
        _p = ( offset>=_data.GetSize() )? _data.GetSize() : offset;
        break;
    case SEEKCUR:
        _p = ( _p+offset>=_data.GetSize() )? _data.GetSize() : _p+offset;
        break;
    case SEEKEND:
        assert(offset>=0);
        _p = _data.GetSize() - offset - 1;
        break;
    };
}

Types::QUAD MemIFile::GetLen()
{
    return _data.GetSize();
}

const std::string MemIFile::_name("");
const std::string &MemIFile::GetName() { return _name; }

}; // namespace Useless

