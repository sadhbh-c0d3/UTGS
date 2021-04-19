/*
 *  $Id: BitVector.h,v 1.5 2002/09/12 14:27:37 koolas Exp $
 *
 *  NAME
 *      BitVector
 *
 *  PURPOSE
 *      Use when dynamic array of booleans is needed.
 *      All standard operators ( &,|,^,~,=,==, &=, ^=, |= ) works with it
 *      You have direct access to single bit via SetBit(), ClearBit() &
 *      GetBit(). You can obtain unsigned int, which will contain
 *      range from n-th bit wide on 32 bits via GetSeg(n). You
 *      may also set value in this range via SetSeg(n, value)
 *      You can also find first bit starting from fixed position
 *      going left or right using SeekRight(pos, zero-or-one) it goes right,
 *      using SeekLeft(pos, zero-or-one) it goes left.
 *      Also its possible to count how far from position is first bit
 *      difference, using SeekBlockLeft(pos) or SeekBlockRight(pos)
 *      You may obtain BitVector from range in other BitVector using
 *      GetSegment(from, to), or set range using other BitVector
 *      SetSegment(BitVector, where-to-put, count-of-bits).
 *      Count() tells you how many ones there are in BitVector
 *         
 *
 *  AUTHOR(S)
 *      Sadhbh Code (https://github.com/sadhbh-c0d3)
 *
 */  

#ifndef __USES_BIT_VECTOR_H__
#define __USES_BIT_VECTOR_H__
#include <vector>
#include <string>

namespace Useless {

class USELESS_API BitVector
{
    public:            
        BitVector() { _bits.resize(1); _bits[0] = 0; }
        ~BitVector() { _bits.clear(); }
        BitVector (const BitVector &f);
        BitVector (const std::string &bits );
        BitVector (const int v);
        BitVector & SetBit(int n);
        BitVector & ClearBit(int n); 
        bool GetBit(int n) const ;
        BitVector & SetBlock(int from, int to);
        BitVector & ClearBlock(int from, int to);
        BitVector& SetSeg(int n, int val, bool crop = false);
        unsigned int GetSeg(int n) const ;
        BitVector  GetSegment( unsigned int from=0, unsigned int to=0) const ;
        BitVector& SetSegment(const BitVector& src, int where=0, int num=0);
        BitVector  operator &(const BitVector& f) const;
        BitVector  operator |(const BitVector& f) const;
        BitVector  operator ^(const BitVector& f) const;
        BitVector& operator =(const BitVector f);
        BitVector  operator ~();
        BitVector& operator &=(const BitVector& f);
        BitVector& operator |=(const BitVector& f);
        BitVector& operator ^=(const BitVector& f);
        bool operator ==(const BitVector& f) const;
        bool Empty() const;
        int Count( unsigned int from=0, unsigned int to=0 ) const;
        int SeekBlockRight(unsigned int from=0) const;
        int SeekBlockLeft(unsigned int from=0) const;
        int SeekRight(unsigned int from=0, bool positive = true, int num = 1) const;
        int SeekLeft(unsigned int from=0, bool positive = true, int num = 1) const;
        BitVector Expand(const BitVector& mutator, int num=0) const;
        BitVector Collapse(const BitVector& mutator, int num=0) const;
        int Size() const { return ( _bits.size() * 32 );}
        BitVector& Clear() { _bits.clear(); return (*this); }
        BitVector operator << (int n) const;
        BitVector operator >> (int n) const;
        BitVector& operator <<= (int n);
        BitVector& operator >>= (int n);
        std::string GetString() const;
        
    private:
        typedef std::vector< unsigned int > BitV;
        BitV _bits;
};
                
};

// INCLUDE INLINE MEMBER FUNCTIONS
#include "Useless/Util/Storage/BitVector-inline.h"

#endif
