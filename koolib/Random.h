#ifndef __INCLUDED_KOOLIB_RANDOM_H__
#define __INCLUDED_KOOLIB_RANDOM_H__

#include <algorithm>
#include <ctime>

#ifdef KOOLIB_HAS_BOOST_RANDOM

#   include <boost/random/linear_congruential.hpp>
#   include <boost/random/uniform_smallint.hpp>

namespace Random {

    typedef boost::minstd_rand RNG;
    typedef boost::uniform_smallint< RNG > RangeRNG;

    inline unsigned long _getseed() { time_t t; time(&t); return t; }
            
    static RNG s_RNG( _getseed() );

    template< class _RandomAccessIterator > inline
        void Shuffle( _RandomAccessIterator first, _RandomAccessIterator last )
        {
            if (( last - first ) <= 1) { return; }

            RangeRNG rng( s_RNG, 0, last - first - 1 );

            // TODO: Fix this algorithm is probably not the best to use
            // Instead one could iterate: next={first..last} and then
            // swap next with element at random position i={next..last}
            // The problem is though that we'd need new uniform_smallint 
            // for each iteration.
            for ( int count=0; count < (last - first)/2; ++count )
            {
                for ( _RandomAccessIterator next = first; next != last; ++next )
                {
                    std::iter_swap( next, first + rng() );
                }
            }
        }

    inline int SampleRange( int first, int last )
    {
        if ( last <= first ) { return first; }
        RangeRNG rng( s_RNG, first, last );
        return rng();
    }

};// Random

#else

//TODO: Get boost random library

namespace Random {
    
    inline int SampleRange( int first, int last )
    {
        if ( last <= first ) { return first; }
        int r = std::rand() / ( RAND_MAX / ( last - first ));
        return r;
    }

    template< class _RandomAccessIterator > inline
        void Shuffle( _RandomAccessIterator first, _RandomAccessIterator last )
        {
            if (( last - first ) <= 1) { return; }

            // TODO: Fix this algorithm is probably not the best to use
            // Instead one could iterate: next={first..last} and then
            // swap next with element at random position i={next..last}
            for ( int count=0; count < (last - first)/2; ++count )
            {
                for ( _RandomAccessIterator next = first; next != last; ++next )
                {
                    int idx = SampleRange( 0, last - first - 1 );
                    std::iter_swap( next, first + idx );
                }
            }
        }

};// Random

#endif

#endif//__INCLUDED_KOOLIB_RANDOM_H__
