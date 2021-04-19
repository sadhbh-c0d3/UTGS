#ifndef __INCLUDED_USELESS_WIDGET_H__
    #error "This file is included by in Widget.h.\nPlease don't include this file on your own"
#endif

#ifndef __INCLUDED_USELESS__WIDGET_ITERATOR_H__
#define __INCLUDED_USELESS__WIDGET_ITERATOR_H__

#include "Useless/System/Assert.h"

namespace Useless {

/*
------------------------------------------------------------------------------*/
class USELESS_API Widget::Iterator
{
public:
    Iterator() : _widget(NULL) {}
    virtual ~Iterator() {}
    virtual     void  Next()                         {}
    virtual     void  Previous()                     {}

    Iterator& operator++()                          { Next();     return *this; }
    Iterator& operator--()                          { Previous(); return *this; }
    Widget*          Who()                   const  { return _widget; }
    Widget*   operator->()                   const  { C_assert(!!_widget); return _widget; }
    Widget&   operator* ()                   const  { return *_widget; }
       bool   operator !()                   const  { return !_widget; }

       bool   operator!=(const Iterator &i)  const  { return _widget!=i._widget; }
       bool   operator==(const Iterator &i)  const  { return _widget==i._widget; }

    typedef SPointer<Iterator> Ptr;

/*
    struct WIRef
    {
        WIRef( Iterator *p ): _p(p) {}
        Widget& operator *() const { return **_p; }
        operator Widget*  () const { return _p->Who();  }
    private:
        Iterator *_p;
    };

    WIRef Ref()                                     { return WIRef(this); }
*/
protected:
    Widget *_widget;
};

/*
----------------------------------------------------------------------------*/
class Widget::RefIter
{
public:
    RefIter()
    {}
    RefIter( Widget::Iterator *p_iterator ): _sp_iterator(p_iterator)
    {}

    bool operator !() const
    { 
        return !(*_sp_iterator); 
    }
    bool operator !=(const RefIter &ref_it)  const
    { 
        (*ref_it._sp_iterator)!=(*_sp_iterator);
    }
    bool operator ==(const RefIter &ref_it)  const
    { 
        (*ref_it._sp_iterator)==(*_sp_iterator);
    }
    
    Widget*        Who() const { return (*_sp_iterator).Who(); }

            operator Widget* () const { return (*_sp_iterator).Who(); }

    Widget* operator->() const { return (*_sp_iterator).operator->(); }
    Widget& operator *() const { return (*_sp_iterator).operator *(); }
    
    RefIter& operator++()      { ++*_sp_iterator; return *this; }
    RefIter& operator--()      { --*_sp_iterator; return *this; }

    void  EvalFwd( PkFunc );
    void  EvalBwd( PkFunc );

private:
    Widget::Iterator::Ptr  _sp_iterator;
};



inline void  Widget::RefIter::EvalFwd( PkFunc pfunc )
{
    for ( ;!!(*_sp_iterator); (*_sp_iterator).Next() )
    {
        pfunc();
    }
}

inline void  Widget::RefIter::EvalBwd( PkFunc pfunc )
{
    for ( ;!!(*_sp_iterator); (*_sp_iterator).Previous() )
    {
        pfunc();
    }
}

template< class FObj > void Widget::ForEach( const FObj &fobj )
{
    RefIter ref_it = FirstChild(); for ( ; !!ref_it; ref_it++ )
    {
        fobj( *ref_it );
    }
}

template< class FObj > void Widget::ForEachRev( const FObj &fobj )
{
    RefIter ref_it = LastChild(); for ( ; !!ref_it; ref_it-- )
    {
        fobj( *ref_it );
    }
}

};//namespace Useless
#endif//__INCLUDED_USELESS__WIDGET_ITERATOR_H__
