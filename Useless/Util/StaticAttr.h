#ifndef __INCLUDED_USELESS_STATIC_ATTR_H__
#define __INCLUDED_USELESS_STATIC_ATTR_H__

namespace Useless {

struct EmptyAttr {};


template< typename _Type >
struct DefaultAttr
{
    static EmptyAttr Get()
    {
        return EmptyAttr();
    }
};

template< typename _Type >
struct DefaultAttrByPointer
{
    static _Type * Get()
    {
        static _Type _default;
        return &_default;
    }
};

    
template< typename _Type >
struct StaticAttr
{
    static Set( const _Type &x )
    {
        typedef typename DefaultAttr< _Type > Def;
        Keeper( Def::Get() ) = x;
    }
    
    static const _Type & Get()
    {
        typedef typename DefaultAttr< _Type > Def;
        return Keeper( Def::Get() );
    }

    static _Type & Keeper( EmptyAttr & )
    {
        static _Type _x;
        return _x;
    }

    static _Type & Keeper( const _Type &x )
    {
        static bool _initialized = false;
        static _Type _x;
        if ( !_initialized )
        {
            _x = x;
            _initialized = true;
        }
        return _x;
    };
};

template< typename _Type, typename _PtrType >
struct Creation
{
    virtual ~Creation() {}
    virtual _PtrType operator ()() const = 0;
};

template< typename _Type, typename _PtrType >
struct DefaultCreation : Creation< _Type, _PtrType >
{
    _PtrType operator ()() const
    {
        return _PtrType( new _Type());
    }
};


}; //Useless

#endif//__INCLUDED_USELESS_STATIC_ATTR_H__
