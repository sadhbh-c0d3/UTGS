#ifndef __INCLUDED_USELESS_GARBAGE_H__
#define __INCLUDED_USELESS_GARBAGE_H__

/*
 *    $Id: Garbage.h,v 1.4 2002/09/13 16:22:11 koolas Exp $
 *
 *    NAME
 *        Garbage
 *
 *    PURPOSE
 *        Holds pointer to object which has to be removed
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#include "Useless/Util/SPointer.h"

namespace Useless {

/*! \ingroup Util 
 *  Use this class to shedule erase action for later time.
 *  Use template<class T> MakeGarbage(T*) to create Garbage from your object.
 */
class Garbage
{
public:
    virtual ~Garbage() {}
    virtual void* Recycle() = 0;
    typedef SPointer<Garbage> Ptr;
};

template< class T>
class GarbageTpl : public Garbage
{
public:
    GarbageTpl(const T &garbage): _garbage(garbage) {}
    virtual ~GarbageTpl()
    {
        delete _garbage;
    }
    virtual void* Recycle()
    {
        void *ptr = _garbage;
        _garbage=NULL;
        return (void*)ptr;
    }

private:
    T _garbage;
};

/*! \fn Garbage::Ptr MakeGarbage(T*)
 *  Creates garbage from object
 */
template< class T>
Garbage::Ptr MakeGarbage(T *ptr)
{
    return new GarbageTpl<T*>(ptr);
}

};//namespace Useless

#endif//__INCLUDED_USELESS_GARBAGE_H__
