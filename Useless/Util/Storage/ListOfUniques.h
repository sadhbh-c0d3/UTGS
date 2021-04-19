#ifndef __INCLUDED_USELESS_LIST_OF_UNIQUES_H__
#define __INCLUDED_USELESS_LIST_OF_UNIQUES_H__

#include <list>
#include <set>

namespace Useless {

template< class _T >
struct ListOfUniques
{
public:
    typedef std::list< _T > List;
    typedef std::set< _T > Set;

    typedef List::iterator iterator;
    typedef List::const_iterator const_iterator;

    bool insert( const _T &x)
    {
	if ( true == _set.insert(x).second)
	{
	    _list.push_back(x);
	    return true;
	}
	return false;
    }

    bool exists( const _T &x) const
    {
	return (_set.find(x) !=_set.end());
    }

    bool erase( iterator it)
    {
	Set::iterator i = _set.find(*it);
	if ( i != _set.end())
	{
	    _set.erase(i);
	    _list.erase(it);
	    return true;
	}
	return false;
    }

    iterator begin()
    {
	return _list.begin();
    }

    iterator end()
    {
	return _list.end();
    }

    const_iterator begin() const
    {
	return _list.begin();
    }

    const_iterator end() const
    {
	return _list.end();
    }

    template< class _C >
    int insert_from( const _C &container)
    {
	int inserted = 0;
	_C::const_iterator i = container.begin();
	for(; i != container.end(); ++i)
	{
	    inserted += insert(*i);
	}
	return inserted;
    }

    template< class _C >
    int erase_by( const std::set<_C> &container)
    {
	int erased = 0;
	iterator i = begin();
	for(; i != end();)
	{
	    iterator j = i; ++i;
	    if ( container.find(*j) != container.end())
	    {
	        erased += erase(j);
	    }
	}
	return erased;
    }
    
    template< class _C >
    int erase_by( const ListOfUniques<_C> &container)
    {
        return erase_by( container.GetSet() );
    }
    
    const List& GetList() const { return _list;}
    const Set & GetSet () const { return _set; }

private:
    List _list;
    Set  _set;
};


}; //Useless
#endif//__INCLUDED_USELESS_LIST_OF_UNIQUES_H__
