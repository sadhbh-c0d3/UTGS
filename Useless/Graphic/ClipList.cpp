/*
**  $Id: ClipList.cpp,v 1.9 2003/01/20 21:53:40 koolas Exp $
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


#include "UselessPch.h"


#include "ClipList.h"



namespace Useless {



inline void ClipList::InsertAfter( RectListIter i, const Rect &r )
{
    assert( i!=_rectlist.end() );
    _rectlist.insert( ++i, r );
}



void ClipList::Sub( RectListIter cr, const Rect &r )
{
    Rect common( (*cr) & r );               // ···
    if(!! common)                              // ···
    {                                       // ···
        const int lx = cr->x;               // left x
//        const int cx = common.x;            // common x
        const int rx = common.x+common.w;   // right x
//        const int ty = cr->y;               // top y
        const int cy = common.y;            // common y
        const int by = common.y+common.h;   // bottom y
        const int fw = cr->w;               // full width
        const int lw = common.x-cr->x;      // left width
        const int cw = common.w;            // common width
        const int rw = fw-lw-cw;            // right width
        const int fh = cr->h;               // full height
        const int th = common.y-cr->y;      // top height
        const int ch = common.h;            // common height
        const int bh = fh-th-ch;            // bottom height

        #define CHANGE_TOP                                  cr->h=th
        #define CHANGE_LEFT             cr->y=cy; cr->w=lw; cr->h=ch
        #define CHANGE_RIGHT  cr->x=rx; cr->y=cy; cr->w=rw; cr->h=ch
        #define CHANGE_BOTTOM           cr->y=by;           cr->h=bh
//        #define INSERT_TOP    cr->AfterInsert( new SGT_ClipRect(lx,ty,fw,th) )
        #define INSERT_LEFT   InsertAfter( cr, Rect(lx,cy,lw,ch) )
        #define INSERT_RIGHT  InsertAfter( cr, Rect(rx,cy,rw,ch) )
        #define INSERT_BOTTOM InsertAfter( cr, Rect(lx,by,fw,bh) )

        switch( ((lw==0)<<3) | ((th==0)<<2) | ((rw==0)<<1) | ((bh==0)<<0) )
        {
            case 0:                  // ··· 0000
                INSERT_BOTTOM;       // ·*·
                INSERT_RIGHT;        // ···
                INSERT_LEFT;
                CHANGE_TOP;
                break;

            case 1:                  // ··· 0001
                INSERT_RIGHT;        // ···
                INSERT_LEFT;         // ·*·
                CHANGE_TOP;
                break;

            case 2:                  // ··· 0010
                INSERT_BOTTOM;       // ··*
                INSERT_LEFT;         // ···
                CHANGE_TOP;
                break;

            case 3:                  // ··· 0011
                INSERT_LEFT;         // ···
                CHANGE_TOP;          // ··*
                break;

            case 4:                  // ·*· 0100
                INSERT_BOTTOM;       // ···
                INSERT_RIGHT;        // ···
                CHANGE_LEFT;
                break;

            case 5:                  // ·*· 0101
                INSERT_RIGHT;        // ·*·
                CHANGE_LEFT;         // ·*·
                break;

            case 6:                  // ··* 0110
                INSERT_BOTTOM;       // ···
                CHANGE_LEFT;         // ···
                break;

            case 7:                  // ··* 0111
                CHANGE_LEFT;         // ··*
                break;               // ··*

            case 8:                  // ··· 1000
                INSERT_BOTTOM;       // *··
                INSERT_RIGHT;        // ···
                CHANGE_TOP;
                break;

            case 9:                  // ··· 1001
                INSERT_RIGHT;        // ···
                CHANGE_TOP;          // *··
                break;

            case 10:                 // ··· 1010
                INSERT_BOTTOM;       // ***
                CHANGE_TOP;          // ···
                break;

            case 11:                 // ··· 1011
                CHANGE_TOP;          // ···
                break;               // ***

            case 12:
                INSERT_BOTTOM;       // *·· 1100
                CHANGE_RIGHT;        // ···
                break;               // ···

            case 13:                 // *·· 1101
                CHANGE_RIGHT;        // *··
                break;               // *··

            case 14:                 // *** 1110
                CHANGE_BOTTOM;       // ···
                break;               // ···

            case 15:                 // *** 1111
                _rectlist.erase(cr); // ***
                break;               // ***

            default:
                assert(0);
        }
    }
}



void ClipList::ClearNoReduce( const Rect &r )
{
    RectListIter current, next = _rectlist.begin();
    while( (current=next)!=_rectlist.end() )
    {
        ++next;
        Sub( current, r );
    }
}




void ClipList::OrNoReduce( const Rect &r )
{
    ClearNoReduce( r );
    (*this) += r;
}




void ClipList::XorNoReduce( const Rect &r )
{
    ClipList cl( r );

    RectListIter cr1;
    RectListIter nextcr1 = _rectlist.begin();

    while( (cr1=nextcr1)!=_rectlist.end() )
    {
        ++nextcr1;
        RectListIter cr2;
        RectListIter nextcr2 = cl._rectlist.begin();
        while( (cr2=nextcr2)!=cl._rectlist.end() )
        {
            ++nextcr2;
            Rect common( (*cr1) & (*cr2) );
            if(!! common)
            {
                Sub( cr1, common );
                Sub( cr2, common );
            }
        }
    }
    (*this) += cl;      // maybe: (*this) << cl;
}



#define REDUCE_CHECK(c1,c2,x,y,w,h,cndel,cdel) \
    if(c1->x+c1->w == c2->x) \
    { \
        if(c1->y==c2->y) \
        { \
            if(c1->h<c2->h) \
            { \
                const int nw = c1->w + c2->w; \
                const int nh = c1->h; \
                const int nwh = nw * nh; \
                if(nwh>c1->w*c1->h && nwh>c2->w*c2->h) \
                    { c1->w=nw; c2->y+=nh; c2->h-=nh; joined=true; } \
            } \
            else if(c1->h>c2->h) \
            { \
                const int nw = c1->w + c2->w; \
                const int nh = c2->h; \
                const int nwh = nw * nh; \
                if(nwh>c1->w*c1->h && nwh>c2->w*c2->h) \
                    { c2->x-=c1->w; c2->w=nw; c1->y+=nh; c1->h-=nh; joined=true; } \
            } \
            else    /* if(c1->h==c2->h) */ \
            { \
                cndel->x=c1->x; cndel->w=c1->w+c2->w; _rectlist.erase(cdel); joined=true; \
            } \
        } \
        else if(c1->y+c1->h==c2->y+c2->h) \
        { \
            if(c1->h<c2->h) \
            { \
                const int nw = c1->w + c2->w; \
                const int nh = c1->h; \
                const int nwh = nw * nh; \
                if(nwh>c1->w*c1->h && nwh>c2->w*c2->h) \
                    { c1->w=nw; c2->h-=nh; joined=true; } \
            } \
            else    /* if(c1->h>c2->h) */ \
            { \
                const int nw = c1->w + c2->w; \
                const int nh = c2->h; \
                const int nwh = nw * nh; \
                if(nwh>c1->w*c1->h && nwh>c2->w*c2->h) \
                    { c2->x-=c1->w; c2->w=nw; c1->h-=nh; joined=true; } \
            } \
        } \
    }




void ClipList::Reduce()
{
    bool joined;
    do {
        joined = false;
        for( RectListIter c1=_rectlist.begin(); c1!=_rectlist.end(); ++c1 )
        {
            RectListIter c2;
            RectListIter next2 = c1;
            ++next2;
            while( (c2=next2)!=_rectlist.end() )
            {
                ++next2;
                     REDUCE_CHECK(c1,c2,x,y,w,h,c1,c2)
                else REDUCE_CHECK(c2,c1,x,y,w,h,c1,c2)
                else REDUCE_CHECK(c1,c2,y,x,h,w,c1,c2)
                else REDUCE_CHECK(c2,c1,y,x,h,w,c1,c2)
            }
        }
    } while(joined);
}




ClipList& ClipList::operator-= ( const Rect &r )
{
    ClearNoReduce( r );
    Reduce();
    return *this;
}




ClipList& ClipList::operator&= ( const Rect &r )
{
    RectListIter cr;
    RectListIter nextcr = _rectlist.begin();
    while( (cr=nextcr)!=_rectlist.end() )
    {
        ++nextcr;
        if(!(*cr &= r)) { _rectlist.erase(cr); }
    }
    Reduce();
    return *this;
}




ClipList& ClipList::operator|= ( const Rect &r )
{
    OrNoReduce( r );
    Reduce();
    return *this;
}




ClipList& ClipList::operator^= ( const Rect &r )
{
    XorNoReduce( r );
    Reduce();
    return *this;
}




ClipList& ClipList::operator&= ( const ClipList &cl )
{
    {
        ClipList cl1;
        cl1 << (*this);
        for( RectListConstIter cr1=cl1._rectlist.begin(); cr1!=cl1._rectlist.end(); ++cr1 )
        {
            for( RectListConstIter cr2=cl._rectlist.end(); cr2!=cl._rectlist.end(); ++cr2 )
            {
                Rect common( (*cr1) & (*cr2) );
                if(!! common)
                {
                    (*this) += common;
                }
            }
        }
    }
    Reduce();
    return *this;
}




ClipList& ClipList::operator-= ( const ClipList &cl )
{
    for( RectListConstIter cr=cl._rectlist.begin(); cr!=cl._rectlist.end(); ++cr )
    {
        ClearNoReduce( *cr );
    }
    Reduce();
    return *this;
}




ClipList& ClipList::operator|= ( const ClipList &cl )
{
    for( RectListConstIter cr=cl._rectlist.begin(); cr!=cl._rectlist.end(); ++cr )
    {
        OrNoReduce( *cr );
    }
    Reduce();
    return *this;
}




ClipList& ClipList::operator^= ( const ClipList &cl )
{
    for( RectListConstIter cr=cl._rectlist.begin(); cr!=cl._rectlist.end(); ++cr )
    {
        XorNoReduce( *cr );
    }
    Reduce();
    return *this;
}




} // namespace Useless


