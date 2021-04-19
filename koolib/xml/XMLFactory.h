/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE KooLAS C++ Template & Class Library     *
 * CALLED KooLib SOURCE CODE.                                       *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE KooLib SOURCE CODE IS (C) COPYRIGHT 2004                     *
 * by the Sadhbh Code (https://github.com/sadhbh-c0d3)              *
 *                                                                  *
 ********************************************************************

  function: Facilite easily with XML tags.
  last mod: $Id: XMLFactory.h,v 1.10 2005/03/05 20:50:58 koolas Exp $

  description:

    1. Attribute querying:

    struct AttrBase
        _name       : name of XML attribute

    struct AttrBaseType< _Type >
        str( string )  : initialize string to _Type translator
        str()       : get whole result as string
        operator *  : get current result of translation
        operator->  : get current result of translation
        operator++  : get current result of translation and translate next portion
        operator !  : get current state of translator (false == EOF)
        operator =  : set current value (ignore translated one)

    struct Attr< _Type, _Required > : acts like AttrBaseType< _Type >,
        with one difference of _Required property. When attribute is _Required,
        then failure of attribute querrying raises exception.

    struct Node : higher-level object for XML iterator
        operator ++     : goto next sibling
        StepInto        : goto first child
        GetFirstChild   : get new Node object pointing to first child
        operator ->     : get pointed node
        operator >>     : querry attribute

    Example 1.  : get 'energy' attribute from node, if not found use defult value 10.

        int GetEnergy( Node node )
        {
            Attr< int, false > energy("energy");
            if (! node >> energy )
            {
                energy = 10;
            }
            return *energy;
        }

    Example 2.  : get 'point' array attribute from node. Node might look like this: <bird point="15 20"/>

        struct Point { int _x,y;}

        Point GetPoint( Node node )
        {
            Attr< int, true > point("point");
            node >> point;
            Point result;
            result._x = point++;
            result._y = *point;
            return result;
        }


    2. XML Tag dictionary - automatic tag handler

        struct XMLTagDict< _ExecutionState > : dictionary containing tag execution methods.
        
        Tag execution method proto is like this:

            _ExecutionState::_ResultType Method( Node, _ExecutionState &state )

        _ExecutionState is user defined structure passed thru XMLTagDict to executor methods.
        User may use it to remember some important data while XML tags being executed.

    Example 1.  : build static xml tag dictionary (xtd)
        
        struct Zoo
        {
            AddAnimal( Animal *animal );
        };

        namespace ANIMALS
        {
            struct ELEPHANT {};
            struct HORSE {};
        };

        static USELESS_XML_TAG_DICT( xtdAnimal, Zoo,
                .AddReg< ANIMALS::ELEPHANT >("elephant")
                .AddReg< ANIMALS::HORSE >("horse")
                );

        USELESS_XML_TAG_HANDLER( ANIMALS::ELEPHANT, Zoo )
        {
            Attr< int, true > weight("weight");
            _node >> weight;
            _state.AddAnimal( new Elephant( weight ) );
        }

        USELESS_XML_TAG_HANDLER( ANIMALS::HORSE, Zoo )
        {
            Attr< int, true > weight("weight");
            _node >> weight;
            _state.AddAnimal( new Horse( weight ) );
        }




 ********************************************************************/


#ifndef __INCLUDED_XML_FACTORY_H__
#define __INCLUDED_XML_FACTORY_H__

#include "Useless/UselessConfig.h"
#include "Useless/XML/XMLParser.h"
#include "Useless/Util/XMLTag.h"
#include "koolib/koolibConfig.h"
#include "koolib/TextUTF8.h"
#include "koolib/FourCC.h"

#include <set>

namespace XMLFactory {

    using Useless::XMLIterator;
    using Useless::XMLNode;
    using Useless::XMLAttributes;

    /*! AttrBase - abstract attribute access interface.
     */
    template< class _CharType >
    struct basic_AttrBase
    {
        typedef _CharType CharType;
        std::basic_string< _CharType > _name;
        virtual void str( const std::basic_string< _CharType > &value ) = 0;
        virtual std::basic_string< _CharType > str() const = 0;
    };

    //! Compatibility
    typedef basic_AttrBase< char > AttrBase;
    typedef basic_AttrBase< wchar_t > AttrUniBase;

    /*! AttrBaseType< _Type > - used to access atribute converting to _Type. 
     */
    template< class _Type, class _CharType = char >
    struct AttrBaseType : basic_AttrBase< _CharType >
    {
        typedef _Type Type;
        //private:
        std::basic_stringstream< _CharType > _ss;
        _Type _current;
        
        //public:
        AttrBaseType( const std::basic_string< _CharType > &name ) { _name = name; }
        
        virtual ~AttrBaseType()
        {
        }

        void str( const std::basic_string< _CharType > &value )
        {
            _ss << value;
            _ss >> _current;
        }

        virtual std::basic_string< _CharType > str() const
        {
            return _ss.str();
        }

        _Type * operator-> ()
        {
            return &_current;
        }

        _Type & operator * ()
        {
            return _current;
        }

        AttrBaseType & operator ++ ()
        {
            _ss >> _current;
            return (*this);
        }
        
        _Type operator ++ ( int )
        {
            _Type c = _current;
            _ss >> _current;
            return c;
        }

        bool operator ! () const
        {
            return !_ss;
        }

        AttrBaseType & operator = ( const _Type &x )
        {
            _current = x;
            return (*this);
        }
    };
    
    /*! Attr< _Type, _Required > - used to access atribute checking _Required, and converting to _Type. 
     */
    template< class _Type, const bool _Required = true, class _CharType = char >
    struct Attr : AttrBaseType< _Type, _CharType >
    {
        Attr( const std::basic_string< _CharType > &name ) : AttrBaseType< _Type, _CharType >( name ) {}
        
        Attr & operator = ( const _Type &x )
        {
            _current = x;
            return (*this);
        }
    };

    /*! AttrProxy - used to access attribute under alternative name.
     */
    template< class _CharType >
    struct basic_AttrProxy : basic_AttrBase< _CharType >
    {
        basic_AttrBase< _CharType > &_attr;
        basic_AttrProxy( const std::basic_string< _CharType > &name , basic_AttrBase< _CharType > &attr ): _attr( attr )
        {
            _name = name;
        }
        void str( const std::basic_string< _CharType > &value ) { _attr.str( value ); }
        std::basic_string< _CharType > str() const { return _attr.str(); }
    };

    //! Compatibility
    typedef basic_AttrProxy< char > AttrProxy;
    typedef basic_AttrProxy< wchar_t > AttrUniProxy;


    /*! Node - adaptor for XMLIterator.
     *
     *  calss XMLIterator must define itertor for some xml document,
     *  where such methods are defined:
     *
     *      StepInto - step inside current tag
     *      HasChildren - tell if current tag has inner tags
     *      GetNumChildren - tell the number of inner children tags
     *      GetNumRemainingSiblings - tell the remaining number of siblings from current point of iteration
     *      GetFirstChild - return Node pointing to first inner tag
     *      Advance - move forwards thru sibling tags n-times
     *      SubTree - make copy of current node and all inner nodes
     *      operator * - get accesss to node
     *
     *  XMLParser::Node must have such attributes:
     *
     *      _name - the name of tag
     *      _value - the value enclosed within tag
     *      _attributes - dictionary { string -> string }
     *      _source - name of source
     *      _line - line in that source
     *
     */
    struct Node
    {
        //public:
        Node & operator ++ ()
        {
            ++_iter; return (*this);
        }

        Node & StepInto()
        {
            _iter.StepInto(); return (*this);
        }

        bool HasChildren() const
        {
            return _iter.HasChildren();
        }

        int GetNumChildren() const
        {
            return _iter.GetNumChildren();
        }

        int GetNumRemainingSiblings() const
        {
            return _iter.GetNumRemainingSiblings();
        }
    
        Node GetFirstChild() const
        {
            return Node( _iter.GetChildrenIterator());
        }

        Node & Advance( int numSiblings )
        {
            _iter.Advance( numSiblings );
            return (*this);
        }

        Useless::XMLParser::Nodes SubTree()
        {
            return _iter.SubTree();
        }

        const XMLNode * operator ->() const { return &(*_iter); }

        bool operator ! () const { return !const_cast< XMLIterator & >( _iter ); }

        template< class _Type, const bool _Required >
        bool operator >> ( Attr< _Type, _Required > &attr ) const
        {
            if ( !(*this) )
            {
                if ( _Required )
                {
                    RaiseError( attr );
                }

                return false;
            }

            const XMLAttributes &attributes( (*_iter)._attributes );
            XMLAttributes::const_iterator itAttr = attributes.find( attr._name );
            if ( itAttr == attributes.end() )
            {
                if ( _Required )
                {
                    RaiseError( attr );
                }

                return false;
            }

            attr.str( (*itAttr).second );

            return true;
        }
        
        bool operator >> ( AttrBase &attr ) const
        {
            if ( !(*this) )
            {
                return false;
            }

            const XMLAttributes &attributes( (*_iter)._attributes );
            XMLAttributes::const_iterator itAttr = attributes.find( attr._name );
            if ( itAttr == attributes.end() )
            {
                return false;
            }

            attr.str( (*itAttr).second );

            return true;
        }
        
        
        Node() {}
        Node( XMLIterator iter ) : _iter( iter ) {}

        //private:
        XMLIterator _iter;

        template< class _Type, const bool _Required >
        void RaiseError( Attr< _Type, _Required > &attr ) const
        {
            throw Useless::Error("XML-Element: Attribute '%s' was not found in current node.", attr._name.c_str() );
        }
    };



    /*! use Attr< Hex > to access integer represented as hex.
     */
    struct Hex
    {
        int GetInteger() const { return _int; }
        operator int () { return _int; }
        int _int;
    };

    /*! custom convertion from std::string to Hex
     */
    inline std::stringstream & operator >> ( std::stringstream &ss, Hex &hex )
    {
        std::string value;
        ss >> value;
        hex._int = Useless::to_hex< char >( value );
        return ss;
    }


    template< class _Ctor, class _ExecutionState >
    struct XMLConstructor
    {
        typedef typename _ExecutionState::ResultType ResultType;
        static ResultType Create( Node node, _ExecutionState &state );
    };

    template< class _ExecutionState >
    struct XMLTagDict
    {
        typedef typename _ExecutionState::ResultType ResultType;
        typedef ResultType (*NodeExecutor)( Node node, _ExecutionState &state );

        typedef std::map< std::string, NodeExecutor > Dict;
        Dict _dict;

        typedef std::set< XMLTagDict *> Bases;
        Bases _bases;

        typename Dict::const_iterator _FindExecutor( Node node, _ExecutionState &state )
        {
            Dict::const_iterator itExecutor = _dict.find( node->_name );
            if ( itExecutor != _dict.end() )
            {
                return itExecutor;
            }
            else
            {
                for ( Bases::const_iterator itBase = _bases.begin();
                        itBase != _bases.end(); ++itBase )
                {
                    Dict::const_iterator itExecutor = (*itBase)->FindExecutor( node, state );
                    if ( itExecutor != (*itBase)->_dict.end() )
                    {
                        return itExecutor;
                    }
                }
            }
            return _dict.end();
        }

        bool _Execute( Node node, _ExecutionState &state )
        {
#ifdef NDEBUG
            bool result = 0;
            Dict::const_iterator itExecutor = _dict.find( node->_name );
            if ( itExecutor != _dict.end() )
            {
                NodeExecutor executor = (*itExecutor).second;
                (*executor)( node, state );
                return true;
            }
            else
            {
                for ( Bases::const_iterator itBase = _bases.begin();
                        itBase != _bases.end(); ++itBase )
                {
                    if ( (*itBase)->_Execute( node, state ))
                    {
                        return true;
                    }
                }
            }
            return false;
#else
            int result = 0;            
            Dict::const_iterator itExecutor = _dict.find( node->_name );
            if ( itExecutor != _dict.end() )
            {
                NodeExecutor executor = (*itExecutor).second;
                (*executor)( node, state );
                ++result;
            }
            
            for ( Bases::const_iterator itBase = _bases.begin();
                    itBase != _bases.end(); ++itBase )
            {
                result += (*itBase)->_Execute( node, state );
            }

            if ( result > 1 )
            {
                throw Useless::Error("XML-Factory: Ambiguous element <%s> found in dictionary.", node->_name.c_str() );
            }
            return result;
#endif
        }

        void Execute( Node node, _ExecutionState &state )
        {
            try {
                if (! _Execute( node, state ))
                {
                    RaiseError( node, state );
                }
            }
            catch( Useless::Error &err )
            {
                throw Useless::XMLParser::XMLError( node->_source, node->_line, err.GetError() );
            }
        }

        void ExecuteChildren( Node node, _ExecutionState &state )
        {
            for ( node.StepInto(); !!node; ++node )
            {
                Execute( node, state );
            }
        }

        void Register( std::string name, NodeExecutor executor )
        {
            _dict.insert( std::make_pair( name, executor ));
        }

        void RaiseError( const Node &node, _ExecutionState &state )
        {
            throw Useless::Error("XML-Factory: Element <%s> not found in dictionary.", node->_name.c_str() );
        }

        template< class _Ctor >
        XMLTagDict & AddReg( std::string name )
        {
            Register( name, &XMLConstructor< _Ctor, _ExecutionState >::Create );
            return (*this);
        }

        XMLTagDict & Extend( const XMLTagDict &other )
        {
            std::copy( other._dict.begin(), other._dict.end(), std::inserter( _dict, _dict.begin() ));
            std::copy( other._bases.begin(), other._bases.end(), std::inserter( _bases, _bases.begin() ));
            return (*this);
        }

        XMLTagDict & AddBase( XMLTagDict *other )
        {
            _bases.insert( _bases.end(), other );
            return (*this);
        }

        XMLTagDict & DropReg( std::string name )
        {
            Dict::iterator it = _dict.find( name );
            if ( it != _dict.end() )
            {
                _dict.erase( it );
            }
            else
            {
                throw Useless::Error("XML-Factory-DropReg: Element <%s> not found in dictionary.", name.c_str() );
            }
            return (*this);
        }

        XMLTagDict & HideReg( std::string name )
        {
            if ( _dict.find( name ) != _dict.end() )
            {
                DropReg( name );
            }
            Register( name, _HiddenRegAssert );
            return (*this);
        }

        XMLTagDict & Clear()
        {
            _dict.clear();
            _bases.clear();
            return (*this);
        }
        
        XMLTagDict & Nop()
        {
            return (*this);
        }

        static ResultType _HiddenRegAssert( Node node, _ExecutionState &state)
        {
            throw Useless::Error("XML-Factory: Element <%s> was found in dictionary, but it is HIDDEN.", node->_name.c_str() );
        }
    };


    template< class _ExecutionState, class _Ctor >
    struct XMLTagDict_AddReg
    {
        XMLTagDict_AddReg( std::string name, XMLTagDict< _ExecutionState > &dict )
        {
            dict.Register( name, &XMLConstructor< _Ctor, _ExecutionState >::Create );
        }
    };
    
    template< class _ExecutionState >
    struct XMLTagDict_AddBase
    {
        XMLTagDict_AddBase( XMLTagDict< _ExecutionState > &dict2, XMLTagDict< _ExecutionState > &dict )
        {
            dict.AddBase( &dict2 );
        }
    };
    
    template< class _ExecutionState >
    struct XMLTagDict_Extend
    {
        XMLTagDict_Extend( const XMLTagDict< _ExecutionState > &dict2, XMLTagDict< _ExecutionState > &dict )
        {
            dict.Extend( dict2 );
        }
    };

    template< class _T >
    struct XMLES_Value
    {
        typedef void ResultType;
        XMLES_Value( _T &x ) : _x(x) {}
        _T &_x;
        _T * operator ->() { return &_x; }
        _T & operator * () { return _x; }
        _T & get() { return _x; }
    };

    int OnXMLTagDictInit( const std::string &name );

    template< class _ExecutionState >
    XMLTagDict< _ExecutionState > & SingleInstance( const std::string &name )
    {
        typedef XMLTagDict< _ExecutionState > Dict;
        typedef std::map< std::string, Dict > Map;
        static Map dictMap;
        std::pair< Map::iterator, bool > result = dictMap.insert( std::make_pair( name, Dict() ));
        if ( result.second ) {
            OnXMLTagDictInit( name );
        }
        return (*result.first).second;
    };


};//XMLFactory

#define USELESS_XML_TAG_DICT_DECLARE( _name, _ExecutionState ) \
KOOLIB_API XMLFactory::XMLTagDict< _ExecutionState > & _name##SingleInstance();\
static XMLFactory::XMLTagDict< _ExecutionState > &_name = _name##SingleInstance();


#define USELESS_XML_TAG_DICT_DEFINE( _name, _ExecutionState ) \
XMLFactory::XMLTagDict< _ExecutionState > & _name##SingleInstance() {\
    return XMLFactory::SingleInstance< _ExecutionState >( #_name );\
}


/*
#define USELESS_XML_TAG_DICT_DECLARE( _name, _ExecutionState ) \
extern XMLFactory::XMLTagDict< _ExecutionState > _name; \
int _name##LinkerInitialize(); \
struct _name##LinkerInitializeStruct { _name##LinkerInitializeStruct() { _name##LinkerInitialize(); } }; \
static _name##LinkerInitializeStruct s_##_name##LinkerInitializeStruct;

#define USELESS_XML_TAG_DICT_DEFINE( _name, _ExecutionState ) \
XMLFactory::XMLTagDict< _ExecutionState > _name; \
int _name##LinkerInitialize() { return XMLFactory::OnXMLTagDictInit( #_name ); }
*/

#define USELESS_XML_TAG_DICT( _name, _ExecutionState, _Tags ) \
XMLFactory::XMLTagDict< _ExecutionState > _name ( XMLFactory::XMLTagDict< _ExecutionState >() _Tags );

#define USELESS_XML_TAG_DICT_Extend( _name, _ExecutionState, _extName, _Tags ) \
XMLFactory::XMLTagDict_Extend< _ExecutionState > _name##_Extend_##_extName ( XMLFactory::XMLTagDict< _ExecutionState >() _Tags, _name );

#define USELESS_XML_TAG_DICT_AddReg( _name, _ExecutionState, _regName, _regType )\
XMLFactory::XMLTagDict_AddReg< _ExecutionState, _regType > _name##_AddReg_##_regName( #_regName, _name );

#define USELESS_XML_TAG_DICT_AddBase( _name, _ExecutionState, _regName )\
XMLFactory::XMLTagDict_AddBase< _ExecutionState > _name##_AddBase_##_regName( _regName, _name );


#define USELESS_XML_TAG_HANDLER( _type, _ExecutionState ) \
_ExecutionState::ResultType \
XMLFactory::XMLConstructor< _type, _ExecutionState >::\
Create( XMLFactory::Node _node, _ExecutionState &_state )

#endif//__INCLUDED_XML_FACTORY_H__
