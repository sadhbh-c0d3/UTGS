#ifndef __INCLUDED__USELESS_CELL_DIVISION_VECTOR_H__
#define __INCLUDED__USELESS_CELL_DIVISION_VECTOR_H__
#include <vector>
#include <algorithm>

namespace Useless {

/*! cell_t is element type of cell_division_vector, and it describes
 *  size of one grid mesh.
 */
struct cell_t
{
    cell_t( int s ): size(s) {}
    cell_t( int s, int p ): size(s), position(p) {}

    int size;
    int position;
};
/*! cell_division_vector is used to calculate positions of each divider
 *  line in non-uniform grid, with mixed weightened and fixed mesh sizes.
 *  It also has cross_range method which finds which cells are crossed
 *  by given range (left,right).
 */
struct cell_division_vector : std::vector< cell_t >
{
public:
    void push_back( cell_t c )
    {
        insert(end(), c); _fit_size.x += (c.size>0)? c.size : -c.size;
    }

    void calc_positions( int size_to_fit )
    {
        _fit_size.x = size_to_fit;
        sum_sizes s = std::for_each( begin(), end(), sum_sizes() );
        int normal_size = s.positive;
        int fit_size = size_to_fit-s.negative;
        if ( !normal_size || fit_size<0 ) { normal_size=1; fit_size=0; }
        _fit_size.x = std::for_each( begin(), end(), update_positions(normal_size,fit_size) );
    }

    typedef std::pair< const_iterator, const_iterator > range_t;
    range_t cross_range( int left, int right ) const
    {
        const_iterator p =
         std::lower_bound( begin(), end(), cell_pos(left), less_equal_position() );

        if (p == end())
        {
            return std::make_pair(p,p);
        }
        else if (p != begin())
        {
            --p;
        }

        const_iterator q =
         std::lower_bound( begin(), end(), cell_pos(right), less_position() );

        return std::make_pair(p, q);
    }
    
    //! does given cell exist ?
    bool exists( size_t n ) const { return ( n>=0 && n<size() ); }
    //! what size has given cell after calc_positioins
    size_t get_size( size_t n ) const
    {
        if ( n>=0 ) {
            if ( n+1<size() ) { return at(n+1).position - at(n).position; }
            else if ( n+1==size() ) { return _fit_size.x - at(n).position; }
        } return 0;
    }

    const cell_t& at( size_t n) const { return operator[](n);}
    cell_t& at( size_t n) { return operator[](n);}

    int get_fit_size() const { return _fit_size.x; }

    struct cell_pos
    {
        cell_pos( int p ): position(p) {}
        operator cell_t() const { return cell_t(0,position); }

        int position;
    };

    struct less_equal_position
    {
        bool operator()( const cell_t &c1, const cell_t &c2 )
        {
            return c1.position <= c2.position;
        }
    };

    struct less_position
    {
        bool operator()( const cell_t &c1, const cell_t &c2 )
        {
            return c1.position < c2.position;
        }
    };

    struct update_positions
    {
        update_positions(): last_position(0) {}
        update_positions( int n, int f): last_position(0), normal_size(n), fit_size(f) {}

        void operator()( cell_t &c ) const
        {
            c.position = (int)last_position;
            if (c.size>0)
                last_position += (fit_size*c.size)/normal_size;
            else
                last_position -= c.size;
        }

        operator int () const { return (int)last_position; }

        mutable float last_position;
        float normal_size;
        float fit_size;
    };

    struct sum_sizes
    {
        sum_sizes(): positive(0), negative(0) {}
        void operator()( const cell_t &c ) const 
        {
            if (c.size>0)
                positive += c.size; 
            else
                negative -= c.size;
        }
        operator int() const { return positive-negative; }

        mutable int positive;
        mutable int negative;
    };

    struct zeroed_int{ int x; zeroed_int(): x(0) {} } _fit_size;
};


};//namespace Useless
#endif//__INCLUDED__USELESS_CELL_DIVISION_VECTOR_H__
