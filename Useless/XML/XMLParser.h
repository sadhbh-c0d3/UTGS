#ifndef __INCLUDED_USELESS_XML_PARSER_H__
#define __INCLUDED_USELESS_XML_PARSER_H__

#pragma warning(disable:4786)

#include <string>
#include <vector>
#include <map>
#include "Useless/System/Assert.h"
#include "Useless/Util/Storage/DependencyTree.h"

// I include those headers cause everywhere we use XML we need them
#include "Useless/Util/str_conv.h"
#include "Useless/Util/stl_addon.h"
#include "Useless/Util/LangSelector.h" // locale support

namespace Useless {

/*! XMLParser may be used with short XML files.
 *  After parsing we get nodes with their attributes.
 *  We can access nodes using begin(), end() or at()
 */
class USELESS_API XMLParser
{
public:
    XMLParser();
    XMLParser( const std::string &xml_string, const std::string &source = "< none >" );
    virtual ~XMLParser();

    struct USELESS_API XMLError : public Useless::Error
    {
        XMLError( const std::string &source,
                    int lineNo,
                    const char *text );
    };

    void RaiseError( const Error &err );

    /*! Parse string as XML stream */
    void Parse();

    typedef std::map< std::string, std::string > Attributes;

    struct Node
    {
        Node(): _parent_node(-1), _line(-1) {}

        std::string     _name;
        std::wstring    _value;
        int             _parent_node;
        Attributes      _attributes; 
        std::string     _source;
        int             _line;
    };

    typedef std::map< int, Node > Nodes;
    typedef Nodes::const_iterator Iterator;

    XMLParser( const Nodes &nodes );

    Iterator    begin() const { return _nodes.begin(); }
    Iterator    end()   const { return _nodes.end(); }
    Iterator    find(int id) const { return _nodes.find(id); }
    const Node& at( int id ) const { return (*_nodes.find(id)).second; }

private:
    std::string _xml_string;
    int         _parser_position;
    std::string _source;
    std::locale _locale;


    Nodes       _nodes;
    int         _last_id;
    void ParseImpl( int parent_id );
};

typedef XMLParser::Attributes XMLAttributes; // set of attributes
typedef XMLParser::Node       XMLNode; // name, value and attributes
typedef XMLParser::Nodes      XMLNodes;// set of nodes


// Stuff below is used with DependencyTree to solve
// relations between nested entities

/*! Tells whether xml entity has parent, and parent index
 *  Defines Index type and NullIndex value
 */
struct XMLParentDetector
{
    typedef int Index;
    enum { NullIndex = 0 };
    XMLParentDetector() {}

    XMLParentDetector( XMLParser &xml ): _xml(&xml) {}

    Index GetIndex( XMLParser::Iterator it ) const
    {
        return (*it).first;
    }

    XMLParser::Iterator FindParent( XMLParser::Iterator it )
    {
        return _xml->find( (*it).second._parent_node );
    }

    XMLParser *_xml;
};

// Quick typedef for XML dependency tree
typedef DependencyTree< XMLParentDetector > XMLDependencyTree;

/*! XMLParser enhanced to have nested structure information
 */
struct USELESS_API XMLDocument : public XMLParser
{
    XMLDocument()
    {
    }

    XMLDocument( const std::string &xml_string, const std::string &source = "< none >" ): XMLParser(xml_string, source)
    {
        _deps = XMLDependencyTree( begin(), end(), *this );
    }

    XMLDocument( const Nodes &nodes ): XMLParser(nodes)
    {
        _deps = XMLDependencyTree( begin(), end(), *this );
    }

    friend struct XMLIterator;

    XMLIterator Root();

    XMLDependencyTree _deps;
};

/*! Simple iterator to use with XMLParser
 */
struct USELESS_API XMLIterator
{
    XMLIterator(): _xml(0) {}
    XMLIterator( XMLDocument &xml ): _xml(&xml)
    {
        XMLDependencyTree::Range rg = _xml->_deps.Root();
        _it = rg.first;
        _it_end = rg.second;
        Update();
    }

    XMLIterator& operator++()
    {
        return Step();
    }

    XMLIterator& Step()
    {
        ++_it;
        return Update();
    }

    XMLIterator& StepInto()
    {
        assert(!!_xml);
        XMLDependencyTree::Range rg =  _xml->_deps.ChildsOf( *_it);
        _it = rg.first;
        _it_end = rg.second;
        return Update();
    }

    bool HasChildren() const
    {
        return (!!XMLIterator( *this ).StepInto());
    }

    int GetNumRemainingSiblings() const
    {
        return (std::distance( _it, _it_end ) - 1);
    }

    int GetNumChildren() const
    {
        return (XMLIterator( *this ).StepInto().GetNumRemainingSiblings() + 1);
    }

    XMLIterator GetChildrenIterator() const
    {
        return (XMLIterator( *this ).StepInto());
    }

    XMLIterator& Advance( int numSiblings )
    {
        std::advance( _it, numSiblings );
        return Update();
    }

    const XMLParser::Node& operator * () const
    {
        return Node();
    }

    int Key() const
    {
        return *_it;
    }

    const XMLParser::Node& Node() const
    {
        return (*_node);
    }

    bool operator !()
    {
        return ((0 == _xml) || (_it == _it_end));
    }

    XMLParser::Nodes SubTree();

protected:
    XMLDependencyTree::Iterator  _it;
    XMLDependencyTree::Iterator  _it_end;
    XMLDocument                 *_xml;
    const XMLParser::Node       *_node;

    XMLIterator& Update()
    {
        assert(!!_xml);
        _node = (_it != _it_end ? &(_xml->at(*_it)) : NULL);
        return (*this);
    }

};

inline XMLIterator XMLDocument::Root()
{
    return XMLIterator(*this);
}


};//namespace Useless
#endif//__INCLUDED_USELESS_XML_PARSER_H__
