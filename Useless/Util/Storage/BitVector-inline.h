/*
 *     $Id: BitVector-inline.h,v 1.3 2002/09/12 14:27:37 koolas Exp $
 *
 *     NAME
 *         BitVector
 *
 *     PURPOSE
 *         Use when dynamic array of booleans is needed.
 *         All standard operators ( &,|,^,~,=,==, &=, ^=, |= ) works with it
 *         You have direct access to single bit via SetBit(), ClearBit() &
 *         GetBit(). You can obtain unsigned int, which will contain
 *         range from n-th bit wide on 32 bits via GetSeg(n). You
 *         may also set value in this range via SetSeg(n, value)
 *         You can also find first bit starting from fixed position
 *        going left or right using SeekRight(pos, zero-or-one) it goes right,
 *        using SeekLeft(pos, zero-or-one) it goes left.
 *        Also its possible to count how far from position is first bit
 *        difference, using SeekBlockLeft(pos) or SeekBlockRight(pos)
 *        You may obtain BitVector from range in other BitVector using
 *        GetSegment(from, to), or set range using other BitVector
 *        SetSegment(BitVector, where-to-put, count-of-bits).
 *        Count() tells you how many ones there are in BitVector
 *         
 *
 *     AUTHOR(S)
 *         Sadhbh Code (https://github.com/sadhbh-c0d3)    
 *
 */  

// DO NOT INCLUDE THIS FILE
// !!! allways included by "BitVector.h" !!!
#ifndef __USES_BITVECTOR_INLINES__
#define __USES_BITVECTOR_INLINES__


#pragma warning(disable:4800)   // disable 'forcing value to bool...' warning
#pragma warning(disable:4018)   // disable '>=: signed/unsigned mismatch' warning
#pragma warning(disable:4267)   // disable 'convertion size_t to int' warning


inline Useless::BitVector::BitVector (const Useless::BitVector &f)
    {
        int n = f._bits.size();
        _bits.resize(n);
        for(int i=0; i!=n; ++i)
        {
            _bits[i] = f._bits[i];
        }
    }

inline Useless::BitVector::BitVector (const int v)
    {
        _bits[0] = v;
    }
        
        
inline Useless::BitVector & Useless::BitVector::SetBit(int n) 
    { 
        int seg = n / 32;
        int off = n % 32;
        if (seg >= _bits.size() ) { _bits.resize( seg + 1); _bits[seg] = 0; }
        _bits[seg] |= (1 << off);
        return (*this);
    }
        
inline Useless::BitVector & Useless::BitVector::ClearBit(int n) 
    { 
        int seg = n / 32;
        int off = n % 32;
        if (seg >= _bits.size() ) { _bits.resize( seg + 1); _bits[seg] = 0; }
        _bits[seg] &= (~(1 << off));
        return (*this);
    }
        
inline bool Useless::BitVector::GetBit(int n) const 
    { 
        int seg = n / 32;
        int off = n % 32;
        if (seg >= _bits.size() ) return 0;
        return  ( _bits[seg] & (1 << off) );
    }
        
inline Useless::BitVector& Useless::BitVector::SetSeg(int n, int val, bool crop)
    {
        int seg = n / 32;
        int off = n % 32;
        int grow1 = ( seg >= _bits.size() )? 1 : 0;
        int grow2 = (off)? 1 : 0;
        if ( seg+1 + grow1 + grow2 > _bits.size() && !crop) 
        { 
            _bits.resize( _bits.size() + grow1 + grow2 );
            if (grow1) 
            {
                _bits[seg]=0; 
                if (grow2) _bits[seg+1]=0; 
            }
        }
        if (!off) _bits[seg] = val;
        else
        {
            _bits[seg] &= ((1<<off)-1);
            _bits[seg] |= (val << off);
            if (!crop)
            {
                _bits[seg+1] &= ~( (1<<off)-1 )  ;
                _bits[seg+1] |= ( val >> (32-off) ) ;
            }
        }
        
        return (*this) ;
    }
    

inline unsigned int Useless::BitVector::GetSeg(int n) const
    {
        int seg = n / 32;
        int off = n % 32;
        if (seg >= _bits.size() ) return 0;
        if (!off) return _bits[seg];
        unsigned int out = 0;
        out |= ( _bits[seg] >> off );
        if (seg+1 >= _bits.size() ) return out;
        out |= ( _bits[seg+1] & ( (1<<off) -1) ) << ( 32-off );
        return out ;
    }
    
inline Useless::BitVector Useless::BitVector::operator &(const Useless::BitVector& f) const
    {
        Useless::BitVector out;
        int n;
        const BitV * vref;
        if( f._bits.size() > _bits.size() )
        {    
            n = _bits.size(); vref = &(f._bits);
        }
        else
        {
            n = f._bits.size(); vref =  &(_bits);
        }
        int N = vref->size();
        out._bits.resize(N);
        int i;
        for(i=0; i!=n; ++i)
        {
            out._bits[i] = _bits[i] & f._bits[i];
        }
        for(i=n; i!=N; ++i)
        {
            out._bits[i] = 0;
        }
        return out;
    }
    
inline Useless::BitVector Useless::BitVector::operator |(const Useless::BitVector& f) const
    {
        BitVector out;
        int n;
        const BitV * vref;
        if( f._bits.size() > _bits.size() )
        {    
            n = _bits.size(); vref = &(f._bits);
        }
        else
        {
            n = f._bits.size(); vref =  &(_bits);
        }
        int N = vref->size();
        out._bits.resize(N);
        int i;
        for(i=0; i!=n; ++i)
        {
            out._bits[i] = _bits[i] | f._bits[i];
        }
        for(i=n; i!=N; ++i)
        {
            out._bits[i] = (*vref)[i];
        }
        return out;
    }

inline Useless::BitVector Useless::BitVector::operator ^(const Useless::BitVector& f) const
    {
        BitVector out;
        int n;
        const BitV * vref;
        if( f._bits.size() > _bits.size() )
        {    
            n = _bits.size(); vref = &(f._bits);
        }
        else
        {
            n = f._bits.size(); vref =  &(_bits);
        }
        int N = vref->size();
        out._bits.resize(N);
        int i;
        for(i=0; i!=n; ++i)
        {
            out._bits[i] = _bits[i] ^ f._bits[i];
        }
        for(i=n; i!=N; ++i)
        {
            out._bits[i] = (*vref)[i];
        }
        return out;
    }

inline Useless::BitVector& Useless::BitVector::operator =(const Useless::BitVector f)
    {
        int n = f._bits.size();
        _bits.resize(n);
        for(int i=0; i!=n; ++i)
        {
            _bits[i] = f._bits[i];
        }
        return (*this);
    }
    
inline Useless::BitVector Useless::BitVector::operator ~()
    {
        int n = _bits.size();
        BitVector out;
        out._bits.resize(n);
        for(int i=0; i!=n; ++i)
        {
            out._bits[i] = (~_bits[i]);
        }
        return out;
    }

inline Useless::BitVector& Useless::BitVector::operator &=(const Useless::BitVector& f)
    {
        int n;
        const BitV * vref;
        if( f._bits.size() > _bits.size() )
        {    
            n = _bits.size(); vref = &(f._bits);
        }
        else
        {
            n = f._bits.size(); vref =  &(_bits);
        }
        int N = vref->size();
        int i;
        for(i=0; i!=n; ++i)
        {
            _bits[i] &= f._bits[i];
        }
        for(i=n; i!=N; ++i)
        {
            _bits[i] = 0;
        }
        return (*this);
    }
    
inline Useless::BitVector& Useless::BitVector::operator |=(const Useless::BitVector& f)
    {
        int n;
        const BitV * vref;
        if( f._bits.size() > _bits.size() )
        {    
            n = _bits.size(); vref = &(f._bits);
        }
        else
        {
            n = f._bits.size(); vref =  &(_bits);
        }
        int N = vref->size();
        int i;
        for(i=0; i!=n; ++i)
        {
            _bits[i] |= f._bits[i];
        }
        for(i=n; i!=N; ++i)
        {
            _bits[i] = (*vref)[i];
        }
        return (*this);
    }
    
inline Useless::BitVector& Useless::BitVector::operator ^=(const Useless::BitVector& f)
    {
        int n;
        const BitV * vref;
        if( f._bits.size() > _bits.size() )
        {    
            n = _bits.size(); vref = &(f._bits);
        }
        else
        {
            n = f._bits.size(); vref =  &(_bits);
        }
        int N = vref->size();
        int i;
        for(i=0; i!=n; ++i)
        {
            _bits[i] ^= f._bits[i];
        }
        for(i=n; i!=N; ++i)
        {
            _bits[i] = (*vref)[i];
        }
        return (*this);
    }

inline bool Useless::BitVector::operator ==(const Useless::BitVector& f) const
    {
        if (f._bits.size() != _bits.size()) return 0;
        int n = _bits.size();
        for(int i=0; i!=n; ++i)
        {
            if( _bits[i] != f._bits[i] ) return false;
        }
        return true;
    }

inline bool Useless::BitVector::Empty() const
    {
        int n = _bits.size();
        int i = 0;
        while( i!=n )
        {
            if ( _bits[i++] ) return false;
        }
        return true;
    }
        
inline int Useless::BitVector::Count( unsigned int from, unsigned int to ) const
    {
        if (from > Size()) return -1;
        int i = from / 32;
        register int j = from % 32;
        int n = _bits.size();
        register int k = 0;
        register int N = to - from;
        for ( ; i!=n; ++i )
        {
            register unsigned int bits = _bits[i];
            for ( ; j!=32; ++j)
            {
                k += ( bits & (1<<j) )? 1 : 0;
                if ( !(--N) ) return k;
            }
            j=0;                
        }
        return k;
    }
        
inline int Useless::BitVector::SeekBlockRight(unsigned int from) const
    {
        if (from > Size()) return -1;
        int i = from / 32;
        register int j = from % 32;
        int n = _bits.size();
        int k = from;
        register bool positive = ( _bits[i] & (1<<j) )? 1 : 0;
        for ( ; i!=n; ++i )
        {
            register unsigned int bits = _bits[i];
            for ( ; j!=32; ++j )
            {
                if (
                      ( positive && !(bits & (1<<j) ) ) ||
                      (!positive &&  (bits & (1<<j) ) )
                   )  return k;
                ++k;
            }
            j=0;    
        }
        return -1;
    }

inline int Useless::BitVector::SeekBlockLeft(unsigned int from) const
    {
        if (from > Size()) return -1;
        int i = from / 32;
        register int j = from % 32;
        int n = _bits.size();
        int k = from;
        register bool positive = ( _bits[i] & (1<<j) )? 1 : 0;
        do
        {
            register unsigned int bits = _bits[i];
            for ( ; j>=0; --j )
            {
                if (
                      ( positive && !(bits & (1<<j) ) ) ||
                      (!positive &&  (bits & (1<<j) ) )
                   )  return k;
                ++k;
            }
            j=0;    
        } while(--i > 0);
        return -1;
    }

inline int Useless::BitVector::SeekRight
        (unsigned int from, bool positive, int num) const
    {
        if (from > Size()) return -1;
        int i = from / 32;
        register int j = from % 32;
        int n = _bits.size();
        int k = from;
        for ( ; i!=n; ++i )
        {
            register unsigned int bits = _bits[i];
            for ( ; j!=32; ++j)
            {
                if (
                    (  positive &&  ( bits & (1<<j) ) )    || 
                    ( !positive && !( bits & (1<<j) ) )
                   )    if (!(--num)) return k;
                ++k;
            }
            j=0;
        }
        return -1;
    }

inline int Useless::BitVector::SeekLeft
        (unsigned int from, bool positive, int num) const
    {
        if (!from || from >= Size()) from = Size()-1;
        int i = from / 32;
        register int j = from % 32;
        int k = from;
        do
        {
            register unsigned int bits = _bits[i];
            for ( ; j>=0; --j)
            {
                if ( 
                     (  positive &&  ( bits & (1<<j) ) ) ||
                     ( !positive && !( bits & (1<<j) ) ) 
                    )    if (!(--num)) return k;
                --k;
            }
            j = 31;
        } while (--i >= 0);
        return -1;
    }

#pragma warning(default:4800)   // default 'forcing value to bool...' warning
#pragma warning(default:4018)   // default '>=: signed/unsigned mismatch' warning
#pragma warning(default:4267)   // default 'convertion size_t to int' warning

#endif

