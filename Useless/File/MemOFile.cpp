/*
**  $Id: MemOFile.cpp,v 1.8 2003/01/20 21:42:21 koolas Exp $
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

#include "Useless/File/MemOFile.h"
#include "Useless/System/Assert.h"

namespace Useless {

MemOFile::MemOFile(int size) 
{ 
    _data.Alloc(size);
    _p = 0;
}

MemOFile::~MemOFile() throw() {}

int MemOFile::WriteUpto( const void *buffer, int size )
{
    int n = ( size >= _data.GetSize()-_p )? _data.GetSize()-_p : size; assert(n>=0);
    if (n>0)
    { 
        memcpy(_data.GetPtrUB()+_p, buffer, n);
        _p = _p + n;
    }
    return n;
}

void MemOFile::Seek( Types::QUAD offset, SeekMode mode )
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

Types::QUAD MemOFile::GetLen()
{
    return _data.GetSize();
}

const MemBlock& MemOFile::GetData() const
{
    return _data;
}

const std::string MemOFile::_name("");
const std::string &MemOFile::GetName() { return _name; }

}; // namespace Useless

