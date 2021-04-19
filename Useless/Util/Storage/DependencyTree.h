#ifndef __INCLUDED_USELESS_DEPENDENCY_TREE__
#define __INCLUDED_USELESS_DEPENDENCY_TREE__

#include <list>
#include <map>
#include "Useless/Error/Error.h"
#include "Useless/TemplateConfig.h"

namespace Useless {
/*! Creates relationship map between indexed elements
 */
template< class ParentDetector >
class DependencyTree
{
public:
    typedef USELESS_TYPENAME ParentDetector::Index Index;
    enum { NullIndex = ParentDetector::NullIndex };
    typedef std::list< Index > Indices;
    typedef std::map< Index, Indices > Dependiences;
    typedef USELESS_TYPENAME Indices::const_iterator Iterator;
    DependencyTree() {}

    /*! This ctor creates dependency tree, which may be accessed
     *  then, as list of childs (indices) per node (index)
     */
    template< class It >
    DependencyTree( It begin, It end, const ParentDetector &detector )
        :_detector(detector)
    {
        std::list< Index > dependencies;

        for ( It it=begin; it!=end; ++it )
        {
            Index current = _detector.GetIndex(it);
            It    iparent = _detector.FindParent(it);

            if ( iparent == end )
            {
                _floating.push_back( current );
            }
            else
            {
                Index parent = _detector.GetIndex(iparent);
                _deps[parent].push_back( current );
            }
        }
    }

    const Indices& operator [] ( const Index &id ) const 
    {
        Dependiences::const_iterator i = _deps.find(id);
        if ( i == _deps.end() )
        {
            throw Error("DependencyTree: Index not found");
        }
        else
            return (*i).second;
    }

    typedef std::pair< USELESS_TYPENAME Indices::const_iterator, USELESS_TYPENAME Indices::const_iterator > Range;

    Range ChildsOf( const Index &id ) const
    {
        Range rg;
        Dependiences::const_iterator i = _deps.find(id);
        if ( i != _deps.end() )
        {
            rg.first = (*i).second.begin();
            rg.second = (*i).second.end();
        }
        else
        {
            rg.first = rg.second = _floating.end();
        }
        return rg;
    }

    Range Root() const
    {
        Range rg;
        rg.first = _floating.begin();
        rg.second = _floating.end();
        return rg;;
    }

    Dependiences    _deps;
    ParentDetector  _detector;
    Indices         _floating;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_DEPENDENCY_TREE__
