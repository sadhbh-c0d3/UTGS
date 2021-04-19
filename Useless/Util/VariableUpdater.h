#ifndef __INCLUDED_USELESS_VARIABLE_UPDATER_H__
#define __INCLUDED_USELESS_VARIABLE_UPDATER_H__

#include "Useless/Functional/Signal.h"
#include "Useless/Functional/Holder.h"
#include <vector>

namespace Useless {
/*! \ingroup ObjSync
 *  Used to connect changes of destination variable
 *  with variables on the list.
 */
template< class T >
class VariableUpdater
{
public:
    class VarID
    {
    public:
        VarID(): _updater(0) {}
        void Untie() { _updater->Untie(_id); }

    private:
        VarID( int id, VariableUpdater<T> *updater ): _updater(updater), _id(id) {}
        VariableUpdater<T> *_updater;
        int         _id;
        friend class VariableUpdater<T>;
    };

    VariableUpdater( const T &v) : _value(v) {}

    VarID Tie(const VariableHolder<T> &var)
    {
        int i=0;
        int id=0;
        int n = _holders.size();
        while( i<n && _holders[i++].first==id ) { ++id; }
        _holders.push_back( std::make_pair( id, var ) );
        return VarID(id,this);
    }

    void Untie(int id) 
    { 
        int i;
        int n = _holders.size();
        void *ptr = NULL;
        for (i=0; i!=n && _holders[i].first!=id; ++i) {}
        while( ++i < n )
        {
            _holders[i-1] = _holders[i];
        }
        _holders.resize(n-1);
    }

    void Update(const T& t)
    {
        if ( QuietUpdate(t) ) { OnUpdate.Send_1(_value); }
    }

    bool QuietUpdate(const T& t)
    {
        int i=_holders.size();
        while(i--)
        {
            (T&)_holders[i].second = t;
        }
        if (_value != t)
        {
            _value = t;
            return true;
        }
        return false;
    }

    VariableUpdater<T>& operator=(const T& t)
    {
        Update(t);
        return *this;
    }

    operator T&()
    {
        int i=_holders.size();
        while( i-- && _holders[i].second==_value ) {}
        if (i>=0)
        {
            _value = (T&)_holders[i].second;
            int i=_holders.size();
            while( i-- ) { (T&)_holders[i].second=_value; }
            OnUpdate.Send_1(_value);
        }
        return _value;
    }

    operator const T&() const
    {
        return _value;
    }


    Signal_1<T> OnUpdate;

private:
    T _value;
    typedef std::pair< int, VariableHolder<T> > __Variable;
    typedef std::vector< __Variable > __HoldersList;
    __HoldersList _holders;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_VARIABLE_UPDATER_H__
