#ifndef __INCLUDED_USELESS_TYPE_INFO_H__
#define __INCLUDED_USELESS_TYPE_INFO_H__
/*
#ifdef WIN32
#   include <typeinfo.h>
#else
#   include <typeinfo>
#endif
*/
#include <typeinfo>

#pragma warning(disable:4800)   // disable 'forcing value to bool...' warning

namespace Useless {

/*! \ingroup RTTI
 *  This class may be used to hold \a typeid information on some storage.
 *  Oveloaded operators guarantee equality comparation and sorting capability.
 */
class TypeInfo
{
public:
    TypeInfo(): pi(0) {}
    TypeInfo(const std::type_info &i): pi(&i) {}
    ~TypeInfo() {}

    TypeInfo& operator=(const TypeInfo& ti)
    {
        pi = ti.pi;
        return *this;
    }

    bool operator ==(const TypeInfo& i) const
    {
        if (!pi) return false;
        else     return (*pi)==(*(i.pi));
    }
    bool operator !=(const TypeInfo& i) const
    {
        if (!pi) return false;
        else     return (*pi)!=(*(i.pi));
    }
    bool operator <(const TypeInfo& i) const
    {
        if (!pi) return false;
        else     return pi->before(*i.pi);
    }
    bool operator <=(const TypeInfo& i) const
    {
        if (!pi) return false;
        else     return (*this)==i || (*this)<i;
    }
    bool operator >=(const TypeInfo& i) const
    {
        if (!pi) return false;
        else     return !((*this)<i);
    }
    bool operator >(const TypeInfo& i) const
    {
        if (!pi) return false;
        else     return !( (*this)==i || (*this)<i );
    }

    const char* Name() const { return pi->name(); }

private:    
    const std::type_info *pi;

};

};//namespace Useless
#endif// __INCLUDED_USELESS_TYPE_INFO_H__
