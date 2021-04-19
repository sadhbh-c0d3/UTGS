/*
**  $Id: MemBlock.cpp,v 1.9 2003/01/20 21:45:51 koolas Exp $
**
**  NAME
**      MemBlock
**
**  PURPOSE
**      Memory block wrapper.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "UselessPch.h"

#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/System/Assert.h"
#include "Useless/Error/Error.h"
#include <memory.h>


namespace Useless {


MemBlock::MemBlock( int size )
    : _mem(0), _size(0)
{
    Alloc(size);
}


MemBlock::~MemBlock() throw()
{
    delete[] (char*)_mem;
}


void MemBlock::Alloc( int size )
{
    void *newmem = AllocBlock(size);
    delete[] (char*)_mem;
    _mem = newmem;
    _size = size;
}


void MemBlock::ReAlloc( int size )
{
    void *newmem = AllocBlock(size);
    int minsize = (size<_size) ? size : _size;
    if(minsize>0) { memcpy( newmem, _mem, minsize); }
    _mem = newmem;
    _size = size;
}


void MemBlock::Free()
{
    Alloc(0);
}


void *MemBlock::AllocBlock( int size )
{
    assert(size>=0);
    void *newmem = 0;
    if(size>0)
    {
        newmem = new char[size];
        if(! newmem) { throw Useless::Error("Failed to allocate memory."); }
    }
    return newmem;
}

void MemBlock::Clear( int value )
{
    memset( _mem, value, _size );
}


} // namespace Useless
