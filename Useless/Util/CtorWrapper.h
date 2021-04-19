#ifndef __INCLUDED__USELESS__CTOR_WRAPPER_H__
#define __INCLUDED__USELESS__CTOR_WRAPPER_H__

namespace Useless {

/*! Constructor Wrapper wraps between base type reference and
 *  derived class ctor. Use this class in this manner:
 *\code
 *  class C0 
 *  {
 *  //place for some virtual interface
 *  };
 *
 *  class C1 
 *  {
 *  public:
 *      C1( const T &x ); //construct C1 from T
 *  };
 *
 *  class Client_Of_C0
 *  {
 *  public:
 *      typedef CtorWrapper<C0,C1,T> Wrap_C0;
 *      Client_Of_C0( const Wrap_C0 &c0 );
 *  };
 *
 *
 *  Possible constructions:
 *  ----------------------
 *  Client_Of_C0 client1( C0() );
 *  Client_Of_C0 client1( C1() );
 *  Client_Of_C0 client2( T() ); //< this is the reason to use CtorWrapper
 *
 *  WARNING: DO NOT EXPLICITLY INSTANTIATE CLASS CtorWrapper
 *
 *  NOTE:
 *  CtorWrapper_2 is extended version, alternative of two classes C1 & C2
 *
 *\endcode
 */
template< class C0, class C1, class T >
class CtorWrapper
{
public:
    CtorWrapper( const C0 &a ): _b(&a), _a(0) {}
    CtorWrapper( const T &x ): _b(0) { _a = new C1(x), _data; }
    ~CtorWrapper() { delete _a, _data; }

    const C0& get() const { return ( (_a)? *_a : *_b ); }
    operator const C0& () const { return get(); }

private:
    char _data[ sizeof(C1) ];
    const C0 *_b;
    C0 *_a;

    CtorWrapper();
    CtorWrapper(const CtorWrapper&);
    void operator =(const CtorWrapper&);
    void* operator new ( size_t );
};

template< class C0, class C1, class T1, class C2, class T2 >
struct CtorWrapper_2
{
public:
    CtorWrapper_2( const C0 &a ): _b(&a), _a(0) {}
    CtorWrapper_2( const T1 &x ): _b(0) { _a = new C1(x), _data; }
    CtorWrapper_2( const T2 &x ): _b(0) { _a = new C2(x), _data; }
    ~CtorWrapper_2() { delete _a, _data; }

    const C0& get() const { return ( (_a)? *_a : *_b ); }
    operator const C0& () const { return get(); }

private:
    char _data[ (sizeof(C1)>sizeof(C2))? sizeof(C1):sizeof(C2) ];
    const C0 *_b;
    C0 *_a;

    CtorWrapper_2();
    CtorWrapper_2(const CtorWrapper_2&);
    void operator =(const CtorWrapper_2&);
    void* operator new ( size_t );
};

};//namespace Useless
#endif//__INCLUDED__USELESS__CTOR_WRAPPER_H__
