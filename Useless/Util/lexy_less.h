#ifndef __INCLUDED_LEXY_LESS_H__
#define __INCLUDED_LEXY_LESS_H__

namespace CTC {
    template< class _X > struct do_less
    {
        bool operator () ( _X x, _X y ) const;
    };

    template< class _T, class _F > struct lexy_less_struct
    {
        lexy_less_struct( const _T &x, const _T &y, const _F &f )
            : _x( x ), _y( y ), _f( f )
            {}

        bool operator () () const
        {
            return ( do_less< _T >()( _x, _y ) ||
                    !do_less< _T >()( _y, _x ) && _f());
        }

        _T _x, _y;
        _F _f;
    };

    template< class _X >
        bool do_less< _X >::operator () ( _X x, _X y ) const
        {
            return (x < y);
        }

    struct lexy_less_false
    {
        bool operator () () const { return false; }
    };

    template< class _T, class _F >
        lexy_less_struct< _T, _F >
        lexy_less( const _T &x, const _T &y, const _F &f )
        {
            return lexy_less_struct< _T, _F >( x, y, f );
        }
    
    template< class _T >
        lexy_less_struct< _T, lexy_less_false >
        lexy_less( const _T &x, const _T &y )
        {
            return lexy_less_struct< _T, lexy_less_false >( x, y, lexy_less_false() );
        }
    
};//CTC

#endif//__INCLUDED_LEXY_LESS_H__
