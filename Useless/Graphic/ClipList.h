#ifndef __INCLUDED_USELESS_GFX_CLIPLIST_H__
#define __INCLUDED_USELESS_GFX_CLIPLIST_H__

/*
**  $Id: ClipList.h,v 1.9 2003/01/20 21:53:40 koolas Exp $
**
**  NAME
**      ClipList
**
**  PURPOSE
**      Clip list of rectangles
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "Useless/Graphic/Point.h"
#include "Useless/Graphic/Rect.h"
#include <list>
#include "Useless/System/Assert.h"


namespace Useless {


class USELESS_API ClipList
{
public:
    ClipList() {}
    ~ClipList() {}

    ClipList( const Rect     &r  ) { (*this)+=r;  }
    ClipList( const ClipList &cl ) { (*this)+=cl; }

    ClipList& operator= ( const Rect     &r  ) { Clear(); (*this)+=r;  return *this; }
    ClipList& operator= ( const ClipList &cl ) { Clear(); (*this)+=cl; return *this; }

    ClipList& operator-= ( const Rect &r );     //!< and not
    ClipList& operator&= ( const Rect &r );
    ClipList& operator|= ( const Rect &r );
    ClipList& operator^= ( const Rect &r );

    ClipList& operator-= ( const ClipList &cl );    //!< and not
    ClipList& operator&= ( const ClipList &cl );
    ClipList& operator|= ( const ClipList &cl );
    ClipList& operator^= ( const ClipList &cl );

    friend ClipList operator- ( const ClipList &cl1, const Rect &r ) { ClipList cl(cl1); return cl-=r; }      //!< and not
    friend ClipList operator& ( const ClipList &cl1, const Rect &r ) { ClipList cl(cl1); return cl&=r; }
    friend ClipList operator| ( const ClipList &cl1, const Rect &r ) { ClipList cl(cl1); return cl|=r; }
    friend ClipList operator^ ( const ClipList &cl1, const Rect &r ) { ClipList cl(cl1); return cl^=r; }

    friend ClipList operator- ( const ClipList &cl1, const ClipList &cl2 ) { ClipList cl(cl1); return cl-=cl2; }    //!< and not
    friend ClipList operator& ( const ClipList &cl1, const ClipList &cl2 ) { ClipList cl(cl1); return cl&=cl2; }
    friend ClipList operator| ( const ClipList &cl1, const ClipList &cl2 ) { ClipList cl(cl1); return cl|=cl2; }
    friend ClipList operator^ ( const ClipList &cl1, const ClipList &cl2 ) { ClipList cl(cl1); return cl^=cl2; }


private:
    typedef std::list<Rect>          RectList;
    typedef RectList::iterator       RectListIter;
    typedef RectList::const_iterator RectListConstIter;

    RectList _rectlist;


protected:
    ClipList& operator+= ( const Rect &r ) { _rectlist.push_back(r); return *this; }
    ClipList& operator+= ( const ClipList &cl ) { _rectlist.insert( _rectlist.begin(), cl._rectlist.begin(), cl._rectlist.end() ); return *this; }
    friend ClipList& operator<< ( ClipList &cl1, ClipList &cl2 ) { assert( &cl1 != &cl2 ); cl1._rectlist.splice( cl1._rectlist.end(), cl2._rectlist ); return cl1; }

    void Clear() { _rectlist.clear(); }
    void ClearNoReduce( const Rect &r );
    void OrNoReduce   ( const Rect &r );
    void XorNoReduce  ( const Rect &r );
    void Reduce();

    void InsertAfter( RectListIter i, const Rect &r );
    void Sub( RectListIter i, const Rect &r );


public:
    typedef std::list<Rect>::const_iterator ConstIter;

    ConstIter begin() const { return _rectlist.begin(); }
    ConstIter end()   const { return _rectlist.end(); }
    int       size()  const { return _rectlist.size(); }
    bool      empty() const { return _rectlist.empty(); }
    void      clear()       { _rectlist.clear(); }


    template< class Storage >
    void copy( Storage &rects) const { rects.resize( size() ); std::copy( begin(), end(), rects.begin() ); }

};


} // namespace Useless


#endif //__INCLUDED_USELESS_GFX_CLIPLIST_H__
