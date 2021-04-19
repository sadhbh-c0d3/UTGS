/*
 *  $Id: BitVector.cpp,v 1.9 2003/01/20 21:45:51 koolas Exp $
 *
 *  NAME
 *      BitVector
 *
 *  PURPOSE
 *      Use when dynamic array of booleans is needed.
 *
 *  AUTHOR(S)
 *      Sadhbh Code (https://github.com/sadhbh-c0d3)
 *
 */  

#include "UselessPch.h"

#include "Useless/Util/Storage/BitVector.h"
#include "Useless/System/Assert.h"

Useless::BitVector & Useless::BitVector::SetBlock(int from, int to)
{
    int start_seg = from / 32;
    int end_seg = to / 32;
    int start_off = from % 32;
    int end_off = to % 32 + 1;
    if ( end_seg >= _bits.size() ) { _bits.resize( end_seg+1 ); }
    if ( start_seg==end_seg )
    {
        _bits[ start_seg ] |= ( ~( (1 << start_off) -1 ) ) & ( (1 << end_off) -1 );
    }
    else
    {
        _bits[ start_seg ] |= ~( (1 << start_off) -1 );
        _bits[ end_seg ] |= ( (1 << end_off) -1 );
    }
    for ( int i = start_seg+1; i < end_seg; ++i )
    {
        _bits[i] = (~0);
    }
    
    return (*this);
}
Useless::BitVector & Useless::BitVector::ClearBlock(int from, int to)
{
    int start_seg = from / 32;
    int end_seg = to / 32;
    int start_off = from % 32;
    int end_off = to % 32 + 1;
    if ( end_seg >= _bits.size() ) { _bits.resize( end_seg+1 ); }
    if ( start_seg==end_seg )
    {
        _bits[ start_seg ] &= ( (1 << start_off) -1 ) | (~( (1 << end_off) -1 ) );
    }
    else
    {
        _bits[ start_seg ] &= ( (1 << start_off) -1 );
        _bits[ end_seg ] &= ~( (1 << end_off) -1 );
    }
    for ( int i = start_seg+1; i < end_seg ; ++i )
    {
        _bits[i] = 0;
    }
    
    return (*this);
}

Useless::BitVector Useless::BitVector::GetSegment
    ( unsigned int from, unsigned int to) const
    {
        int seg = from / 32;
        unsigned int N = (!to)? Size() : to - from;
        if (seg >= _bits.size() ) return BitVector();
        BitVector out;
        int i = 0;
        int val;
        while( i + 31 < N )
        {
            out.SetSeg( i, GetSeg( from + i ) );
            i+=32;
        }
        if ( i < N )
        {
            int u = N - i;
            val = GetSeg( from + i );
            if ( u > 0 ) val &= ( (1 << u) - 1 );
            out.SetSeg( i, val );
        }
        return out;
    }

Useless::BitVector& Useless::BitVector::SetSegment
    ( const Useless::BitVector& src, int where, int num )
    {
        if (!num) num = src.Size();
        int start_seg = where / 32;
        int end_seg = (where+num) / 32;
        int pos = where;
        int a = ( ( (where+num)%32 ) >0 )? 1 : 0 ;
        int grow = end_seg + a - _bits.size();
        if ( grow > 0 ) _bits.resize( end_seg + a);
        int offs = 0;
        int n = end_seg - start_seg;
        for (int i=0; i!=n ; ++i )
        {
            int bits = src.GetSeg(offs);
            SetSeg( pos+offs, bits );
            offs+=32;
        }
        if ( pos != where + num )
        {
            int bits1, bits2, mask, shift;
            shift = ( where + num ) - pos;
            bits1 = src.GetSeg(offs);
            bits2 = GetSeg(pos);
            mask = (shift)? (1<<shift) -1 : ~0;
            bits1 &= mask;
            bits2 &= ~mask;
            SetSeg(pos, bits1 | bits2, true);
        }
        return (*this);
    }
    
Useless::BitVector Useless::BitVector::Expand(const Useless::BitVector& mutator, int num) const
    {
        BitVector out;
        if (!num) num = mutator.SeekLeft();
        int n = mutator._bits.size();
        out._bits.resize(n);
        
        int ssg = 0;                        // source segment, offset and mask = (1<<offset)
        int sof = 0;
        unsigned int smsk = 1;
        int bits = 0;
        
        for ( int sg=0; sg!= n; ++sg )
        {
            unsigned int muts = mutator._bits[sg];    // destination segment, offset and mask
            unsigned int mask = 1;
            unsigned int result = 0;
            for ( int off=0; off!=32 ;++off, mask<<=1, --num )
            {
                if ( !sof )    { bits = _bits[ssg]; }
                if ( muts & mask )
                {
                    if (off > sof) { result |= ( bits & smsk ) << ( off - sof ); }
                    else { result |= ( bits & smsk ) >> ( sof - off ); }
                    smsk <<= 1;
                    ++sof;
                }

                if (!num || off==31) { out._bits[sg] = result; result = 0; }
                if ( sof == 32 ) { ++ssg; sof = 0; smsk = 1; }
                if (!num) return out;
            }
        }
        return out;
    }
        
Useless::BitVector Useless::BitVector::Collapse(const Useless::BitVector& mutator, int num) const
    {
        BitVector out;
        if (!num) num = mutator.SeekLeft();
        int n = mutator._bits.size();
        out._bits.resize(n);
        
        int dsg = 0;                // destination segment, offset and mask = (1<<offset)
        int dof = 0;
        unsigned int dmsk = 1;
        int result = 0;
        
        for ( int sg=0; sg!= n; ++sg )
        {
            unsigned int muts = mutator._bits[sg];    // source segment, offset and mask
            unsigned int mask = 1;
            unsigned int bits = _bits[sg];
            for ( int off=0; off!=32 ;++off, mask<<=1, --num )
            {
                if ( muts & mask )
                {
                    if (off > dof) { result |= ( bits & mask ) >> ( off - dof ); }
                    else { result |= ( bits & mask ) << ( dof - off ); }
                    ++dof;
                    dmsk <<= 1;
                }

                if (!num || dof==32) { out._bits[dsg] = result; result = 0; }
                if ( dof == 32 ) { ++dsg; dof = 0; dmsk = 1; }
                if (!num) return out;
            }
        }
        return out;
    }

Useless::BitVector Useless::BitVector::operator >> (int n) const
    {
        assert( n > 0 && "BitVector::operator >>");
        BitVector out;
        out = GetSegment(n);
        return out;
    }


Useless::BitVector& Useless::BitVector::operator >>= (int n)
    {
        assert( n > 0 && "BitVector::operator >>=");
        int osh = n / 32;                           // Outer Shift - beetwen segments
        int ish = n % 32;                           // Inner Shift - inside segment
        unsigned int mask = (1 << ish) - 1;
        int N = _bits.size();
        int i; 
        int sg;
        for ( i=0, sg=osh; sg!=N-1; ++i, ++sg)
        {
            _bits[i]  = _bits[sg] >> ish;
            _bits[i] |= ( (_bits[sg+1] & mask) << (32 - ish) );
        }
        _bits[i] = _bits[sg] >> ish;
        if ( i <= _bits.size() ) { _bits.resize(i+1); }
        return (*this);
    }

Useless::BitVector Useless::BitVector::operator << (int n) const
    {
        assert( n > 0 && "BitVector::operator <<");
        BitVector out;
        out.SetSegment( (*this), n);
        return out;
    }


Useless::BitVector& Useless::BitVector::operator <<= (int n)
    {
        assert( n > 0 && "BitVector::operator <<=");
        int osh = n / 32;
        int ish = n % 32;
        unsigned int mask = ~( (1 << ish) - 1 );
        int N = _bits.size();
        _bits.resize(N+1);
        _bits[N] = 0;
        int sg;
        for ( sg=N; sg!=0; --sg)
        {
            unsigned int b;
            b  = _bits[sg] << ish;
            b |= ( _bits[sg-1] & mask ) >> (32 - ish);
            _bits[sg+osh] = b;
        }
        _bits[sg+osh] = _bits[0] << ish;
        
        return (*this);
    }

Useless::BitVector::BitVector( const std::string &bits)
{
    for ( int i=0, l=bits.length(); i<l; ++i )
    {
             if ( bits[i] == '1' ) { SetBit(i);  }
        else if ( bits[i] == '0' ) { ClearBit(i);}
        else    { assert(0); }
    }
}

std::string Useless::BitVector::GetString() const
{
    std::string ret;
    for ( int i=0, n=Size(); i<n; ++i )
    {
        ret += (GetBit(i))? '1' : '0';
    }
    return ret;
}
