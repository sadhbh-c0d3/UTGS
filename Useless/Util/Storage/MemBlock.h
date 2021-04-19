#ifndef __INCLUDED_USELESS_MEMBLOCK_H__
#define __INCLUDED_USELESS_MEMBLOCK_H__

/*
**  $Id: MemBlock.h,v 1.7 2003/01/20 21:45:51 koolas Exp $
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

#include "Useless/System/Types.h"


namespace Useless {


class USELESS_API MemBlock
{
public:
    MemBlock( int size=0 );
    ~MemBlock() throw();

    void *GetPtr( int offset=0 ) throw() { return (char*)(_mem)+offset; }
    int   GetSize() const throw() { return _size; }

    void Alloc( int size );
    void ReAlloc( int size );
    void Free();
    void Clear( int value = 0 );

    // I don't use static_cast here since it won't compile in GCC - too much trouble
    const void *GetConstPtr( int offset=0 ) const throw() { return (const char*)(_mem)+offset; }
    
    // helper functions (non const)
    char         *GetPtrC ( int offset=0 ) throw() { return ( char        * )( GetPtr(offset) ); }
    Types::UBYTE *GetPtrUB( int offset=0 ) throw() { return ( Types::UBYTE* )( GetPtr(offset) ); }
    Types::BYTE  *GetPtrB ( int offset=0 ) throw() { return ( Types::BYTE * )( GetPtr(offset) ); }
    Types::UWORD *GetPtrUW( int offset=0 ) throw() { return ( Types::UWORD* )( GetPtr(offset) ); }
    Types::WORD  *GetPtrW ( int offset=0 ) throw() { return ( Types::WORD * )( GetPtr(offset) ); }
    Types::ULONG *GetPtrUL( int offset=0 ) throw() { return ( Types::ULONG* )( GetPtr(offset) ); }
    Types::LONG  *GetPtrL ( int offset=0 ) throw() { return ( Types::LONG * )( GetPtr(offset) ); }

    // helper functions (const)
    const char         *GetConstPtrC ( int offset=0 ) const { return ( const char        * )( GetConstPtr(offset) ); }
    const Types::UBYTE *GetConstPtrUB( int offset=0 ) const { return ( const Types::UBYTE* )( GetConstPtr(offset) ); }
    const Types::BYTE  *GetConstPtrB ( int offset=0 ) const { return ( const Types::BYTE * )( GetConstPtr(offset) ); }
    const Types::UWORD *GetConstPtrUW( int offset=0 ) const { return ( const Types::UWORD* )( GetConstPtr(offset) ); }
    const Types::WORD  *GetConstPtrW ( int offset=0 ) const { return ( const Types::WORD * )( GetConstPtr(offset) ); }
    const Types::ULONG *GetConstPtrUL( int offset=0 ) const { return ( const Types::ULONG* )( GetConstPtr(offset) ); }
    const Types::LONG  *GetConstPtrL ( int offset=0 ) const { return ( const Types::LONG * )( GetConstPtr(offset) ); }


private:
    void *_mem;
    int   _size;

    static void *AllocBlock( int size );

    MemBlock( const MemBlock & );
    MemBlock& operator=( const MemBlock & );
};


} // namespace Useless

#endif __INCLUDED_USELESS_MEMBLOCK_H__
