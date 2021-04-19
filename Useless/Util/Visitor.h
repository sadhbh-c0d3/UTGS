#ifndef __INCLUDED__USELESS_VISITOR_H__
#define __INCLUDED__USELESS_VISITOR_H__

namespace Useless {

class BaseVisitor
{
public:
    virtual ~BaseVisitor() {}
};

/*! Generic Visitor pattern. Derive from it with your visitor.
 */
template< class T, typename R = int >
class Visitor : virtual public BaseVisitor
{
public:
    typedef R ReturnType;
    virtual ReturnType Visit(T&) = 0;
};


template< typename ReturnType >
struct DummyVisitable
{
    template< class T >
    static ReturnType AcceptVisitorImpl(T &visited, BaseVisitor &guest) 
    { 
        return ReturnType(); 
    }
};

/*! Generic Visitable. Derive from it with your visitable class.
 */
template< typename R = int >
class BaseVisitable
{
public:
    typedef R ReturnType;
    
    typedef DummyVisitable<R> AncestorType;

    virtual ~BaseVisitable() {}
    virtual ReturnType AcceptVisitor(BaseVisitor&) = 0;

//protected:
    template< class T >
    static ReturnType AcceptVisitorImpl(T &visited, BaseVisitor &guest)
    {
        if( Visitor<T,R>* p = dynamic_cast< Visitor<T,R>* >(&guest))
        {
            return p->Visit(visited);
        }
        else
        {
            typedef typename T::AncestorType SuperClass;
            return SuperClass::AcceptVisitorImpl(
                        dynamic_cast<SuperClass&>(visited),
                        guest);
        }
        
    }
};


/*! Insert this macro into every Visitable class
 */
#define DEFINE_VISITABLE()\
    virtual ReturnType AcceptVisitor(BaseVisitor &guest)\
    { \
        return AcceptVisitorImpl(*this,guest); \
    }

/*! Insert this macro into every Visitable class
 */
#define DEFINE_CYCLIC_VISITABLE( ANCESTOR )\
    typedef ANCESTOR AncestorType; \
    virtual ReturnType AcceptVisitor(BaseVisitor &guest)\
    { \
        return AcceptVisitorImpl(*this,guest); \
    }


};//namespace Useless
#endif//__INCLUDED__USELESS_VISITOR_H__
