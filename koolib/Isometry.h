/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE KooLAS C++ Template & Class Library     *
 * CALLED KooLib SOURCE CODE.                                       *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE KooLib SOURCE CODE IS (C) COPYRIGHT 2004                     *
 * by the Sadhbh Code (https://github.com/sadhbh-c0d3)              *
 *                                                                  *
 ********************************************************************

  function:
  last mod: $Id: Isometry.h,v 1.7 2004/10/20 18:12:54 koolas Exp $

 ********************************************************************/


#ifndef __INCLUDED_ISOVIEW_H__
#define __INCLUDED_ISOVIEW_H__

#include "Useless/Util/lexy_less.h"
#include "Useless/Graphics/Rect.h"

#include "koolib/Numbers.h"

#include <algorithm>
#include <math.h>
#include <vector>

namespace Isometry {

    using Numbers::FPNum;

    struct Location
    {
        FPNum _u;
        FPNum _v;
        FPNum _w;
        FPNum _order;
        FPNum _extentU;
        FPNum _extentV;
        FPNum _extentW;

        Location( FPNum u=0, FPNum v=0, FPNum w=0, FPNum order=0,
               FPNum extentU=1, FPNum extentV=1, FPNum extentW=1 )
            : _u( u )
            , _v( v )
            , _w( w )
            , _order( order )
            , _extentU( extentU )
            , _extentV( extentV )
            , _extentW( extentW )
            {}

        Location & SetLocation( const Location &l ) { return ( (*this) = l ); }
        
        bool operator < ( const Location &other ) const
        {
            return CTC::lexy_less( _u, other._u,
                    CTC::lexy_less( _v, other._v,
                        CTC::lexy_less( _w, other._w,
                            CTC::lexy_less( _order, other._order )) ))();
        }

        bool Intersects( const Location &querLoc ) const
        {
            const Location &itemLoc = (*this);
            return
                ( querLoc._u <  itemLoc._u + itemLoc._extentU ) &&
                ( itemLoc._u <= querLoc._u + querLoc._extentU ) &&
                ( querLoc._v <  itemLoc._v + itemLoc._extentV ) &&
                ( itemLoc._v <= querLoc._v + querLoc._extentV ) &&
                ( querLoc._w <  itemLoc._w + itemLoc._extentW ) &&
                ( itemLoc._w <= querLoc._w + querLoc._extentW );
        }

    };

    struct Unit
    {
        FPNum _x;
        FPNum _y;
        FPNum _z;
        FPNum _length;

        Unit()
            : _x(0)
            , _y(0)
            , _z(0)
            , _length(0)
            {}

        Unit( FPNum x, FPNum y, FPNum z=0 )
            : _x(x)
            , _y(y)
            , _z(z)
            {
                _length = sqrt( _x*_x + _y*_y );
            }
    };

    inline const Location & get_location( const Location &item )
    {
        return item;
    }

    inline Location & get_location( Location &item )
    {
        return item;
    }

    template< class _Item >
        struct ItemView
        {
            typedef _Item Item;

            ItemView( const _Item &item ): _item( &item ), _xOffset( 0 ), _yOffset( 0 ) {}

            const _Item *_item;
            FPNum _x;
            FPNum _y;
            FPNum _layer;
            FPNum _order;
            FPNum _xOffset;
            FPNum _yOffset;
        };

    struct View
    {
        // u,v,w
        Unit _u;
        Unit _v;
        Unit _w;

        /* phi : left/right angle
         * theta : up/down angle
         * su : size of u grid unit in original 3d space
         * sv : size of v grid unit in original 3d space
         * sw : size of w grid unit in original 3d space
         */
        View( FPNum phi, FPNum theta, FPNum su, FPNum sv, FPNum sw )
        {
            float radian = 180.0 / 3.1415926;
            
            FPNum cosPhi = cos( ((float)phi) / radian );
            FPNum sinPhi = sin( ((float)phi) / radian );
            
            FPNum cosTheta = cos( ((float)theta) / radian );
            FPNum sinTheta = sin( ((float)theta) / radian );

            _u = Unit( -cosPhi * su, sinPhi * cosTheta * su );
            _v = Unit( sinPhi * sv, cosPhi * cosTheta * sv );
            _w = Unit( 0, -sinTheta * sw, 1 );
        }

        /* Rule to obey:
         *
         *  u = usize * [ -cos(p), sin(p)*cos(t) ]
         *  v = vsize * [ sin(p), cos(p)*cos(t) ]
         *  w = wsize * [ 0, sin(t) ]
         */
        View
            ( const Unit &u,
              const Unit &v,
              const Unit &w )
            : _u(u)
            , _v(v)
            , _w(w)
            {
            }

        template< class _Item >
            void GetPosItemView( const ItemView< _Item > &itemView, FPNum &x, FPNum &y, FPNum &order ) const
            {
                x = itemView._x;
                y = itemView._y;
                order = itemView._order;
            }
        
        template< class _Item >
            void GetPosItemViewWithOffset( const ItemView< _Item > &itemView, FPNum &x, FPNum &y, FPNum &order ) const
            {
                x = itemView._x + itemView._xOffset;
                y = itemView._y + itemView._yOffset;
                order = itemView._order;
            }

        template< class _Item >
            void GetPosItem( const _Item &item, FPNum &x, FPNum &y, FPNum &order ) const
            {
                const Location &i = get_location( item );
                x = (i._u * _u._x) + (i._v * _v._x) + (i._w * _w._x);
                y = (i._u * _u._y) + (i._v * _v._y) + (i._w * _w._y);
                order = i._order;
            }

        template< class _Iterator >
            std::vector< ItemView< typename std::iterator_traits< _Iterator >::value_type > >
            Sort( _Iterator first, _Iterator last ) const
            {
                typedef typename std::iterator_traits< _Iterator >::value_type _Item;
                typedef std::vector< ItemView< _Item > > ItemViews;
                ItemViews t;
                ViewTransform fVT(*this);
                Less fLess(*this);
                for ( ; first != last; ++first )
                {
                    t.push_back( fVT( *first ));
                    const Location l = get_location( *first );
                    ItemView< Location > itemView = fVT( Location( 0, 0, 0));

                    for ( FPNum i=1; i < l._extentU; i+=1 )
                    for ( FPNum j=0; j < l._extentV; j+=1 )
                    for ( FPNum k=0; k < l._extentW; k+=1 )
                    {
                        ItemView< Location > altView = fVT( Location( i, j, k ));
                        if ( fLess( itemView, altView ))
                        {
                            itemView = altView;
                        }
                    }

                    t.back()._x;
                    t.back()._y;
                    t.back()._xOffset = itemView._x;
                    t.back()._yOffset = itemView._y;
                }
                //std::transform( first, last, std::back_inserter( t ), ViewTransform(*this) );
                std::stable_sort( t.begin(), t.end(), Less(*this) );
                return t;
            }

        template< class _Iterator >
            _Iterator Clip( const Useless::Rect &rc, _Iterator first, _Iterator last )
            {
                return std::remove_if( first, last, ClippingTest(*this, rc));
            }

        template< class _Painter, class _Iterator >
            Paint
            ( _Painter &painter,
              _Iterator first,
              _Iterator last
            ) const
            {
                //assert( std::is_sorted( first, last, Less(*this) ));
                Useless::Rect rc = ItemRect(*this)();

                while ( first != last )
                {
                    FPNum x, y, o;
                    GetPosItemView( *first, x, y, o );                    
                    painter.Paint( *this, *(*first)._item, x + rc.GetX1(), y + rc.GetY1() );
                    ++first;
                }
            }

        template< class _Iterator >
            _Iterator Select( const Useless::Pos &pos, _Iterator first, _Iterator last ) const
            {
                return std::find_if( first, last, IntersectionTest(*this, pos));
            }


        Location GetLocation( const Useless::Pos &pos, FPNum w ) const
        {
            typedef Numbers::PrecissionTraits< FPNum >::Better BFPNum;

            Location loc;

            BFPNum x = BFPNum::IntType( pos.x ) - BFPNum( _w._x ) * BFPNum( w );
            BFPNum y = BFPNum::IntType( pos.y ) - BFPNum( _w._y ) * BFPNum( w );

            BFPNum det =
                ( BFPNum(_u._x) * BFPNum(_v._y) ) -
                ( BFPNum(_v._x) * BFPNum(_u._y) )
                ;
            loc._u = ( (x * BFPNum(_v._y)) - (y * BFPNum(_v._x)) ) / det;
            loc._v = ( (y * BFPNum(_u._x)) - (x * BFPNum(_u._y)) ) / det;
            loc._w = w;

            return loc;
        }
      

        // --== function objects ==--

        struct Less
        {
            Less( const View &view ): _view( &view ) {}

            const View *_view;

            template< class _Comparable1, class _Comparable2 >
                bool operator () ( const _Comparable1 &a, const _Comparable2 &b ) const
                {
                    const Location & loc1 = get_location( *a._item );
                    const Location & loc2 = get_location( *b._item );
                    bool equalSector = ( loc1._u.GetInteger() == loc2._u.GetInteger() &&
                            loc1._v.GetInteger() == loc2._v.GetInteger() &&
                            loc1._w.GetInteger() == loc2._w.GetInteger() );

                    FPNum x1,y1,x2,y2,o1,o2;
                    _view->GetPosItemViewWithOffset( a, x1, y1, o1 );
                    _view->GetPosItemViewWithOffset( b, x2, y2, o2 );

                    if ( o1 != o2 && loc1.Intersects( loc2 ))
                    {
                        return o1 < o2;
                    }
                    else
                    {
                        return CTC::lexy_less( a._layer, b._layer,
                                CTC::lexy_less( y1, y2,
                                    CTC::lexy_less ( x1, x2,
                                        CTC::lexy_less( o1, o2 ))))();
                    }
                }
        };

        struct ViewTransform
        {
            ViewTransform( const View &view ): _view( &view ) {}

            const View *_view;

            template< class _Item >
                ItemView< _Item > operator () ( const _Item &item ) const
                {
                    ItemView< _Item > iv( item );
                    _view->GetPosItem( item, iv._x, iv._y, iv._order );                    
                    Location l = get_location( item );
                    //dot product with vector [u.z,v.z,w.z]
                    iv._layer = _view->_u._z * (l._u + l._extentU) +
                        _view->_v._z * (l._v + l._extentV) +
                        _view->_w._z * (l._w + l._extentW);
                    return iv;
                }
        };

        struct ClippingTest
        {
            ClippingTest( const View &view, const Useless::Rect &area )
                : _view( &view )
                , _area( area )
                {}

            const View *_view;
            Useless::Rect _area;

            template< class _Comparable >
                bool operator () ( const _Comparable &a ) const
                {
                    FPNum x,y,o;
                    _view->GetPosItemView( a, x, y, o );
                    return !_area.IsInside( x, y );
                }
        };

        struct IntersectionTest
        {
            IntersectionTest( const View &view, const Useless::Pos &pos )
                : _view( &view )
                , _pos( pos )
                {}

            const View *_view;
            Useless::Pos _pos;

            Useless::Pos D( int DU, int DV, int DW ) const
            {
                return Useless::Pos(
                        DU*_view->_u._x + DV*_view->_v._x + DW*_view->_w._x,
                        DU*_view->_u._y + DV*_view->_v._y + DW*_view->_w._y );
            }
            
            template< class _Comparable >
                bool operator () ( const _Comparable &a ) const
                {
                    Useless::Pos p[8];
                    FPNum x,y,o;
                    FPNum eU = a._item->_extentU, eV = a._item->_extentV, eW = a._item->_extentW;
                    _view->GetPosItemView( a, x, y, o );
                    p[0] = Useless::Pos( x, y );
                    p[1] = p[0] + D( 0, 0,eW);
                    p[2] = p[0] + D( 0,eV,eW);
                    p[3] = p[0] + D( 0,eV, 0);
                    p[4] = p[0] + D(eU,eV, 0);
                    p[5] = p[0] + D(eU, 0, 0);
                    p[6] = p[0] + D(eU, 0,eW);
                    p[7] = p[0] + D(0,  0,eW);

                    int intersections = 0;

                    for ( int i=1; i<7; ++i )
                    {
                        Useless::Pos dp( p[i+1] - p[i] );

                        if ( 0 == dp.y )
                        {
                            continue;
                        }

                        int y = _pos.y;

                        if ( y >= std::min( p[i].y, p[i+1].y ) && y < std::max( p[i].y, p[i+1].y ))
                        {
                            int x = p[i].x + ((y - p[i].y)*dp.x)/dp.y;

                            if ( x >= _pos.x )
                            {
                                ++intersections;
                            }
                        }
                    }

                    return (intersections % 2);
                }
        };

        struct ItemRect
        {
            ItemRect( const View &view ): _view( &view ) {}
            const View *_view;
            
            Useless::Pos D( int DU, int DV, int DW ) const
            {
                return Useless::Pos(
                        DU*_view->_u._x + DV*_view->_v._x + DW*_view->_w._x,
                        DU*_view->_u._y + DV*_view->_v._y + DW*_view->_w._y );
            }

            Useless::Rect operator () () const
            {
                Useless::Pos p[6];

                p[0] = D(0,0,1);
                p[1] = D(0,1,1);
                p[2] = D(0,1,0);
                p[3] = D(1,1,0);
                p[4] = D(1,0,0);
                p[5] = D(1,0,1);

                Useless::Rect rc( p[0].x, p[0].y, 0, 0 );

                for ( int i=1; i!=6; ++i )
                {
                    rc.SetX1( std::min( rc.GetX1(), p[i].x ));
                    rc.SetX2( std::max( rc.GetX2(), p[i].x ));
                    rc.SetY1( std::min( rc.GetY1(), p[i].y ));
                    rc.SetY2( std::max( rc.GetY2(), p[i].y ));
                }

                return rc;
            }
        };
    };


};//Isometry


#endif//__INCLUDED_ISOVIEW_H__
