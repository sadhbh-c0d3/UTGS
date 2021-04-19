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

function:
last mod: $Id: $

 ********************************************************************/

#ifndef __INCLUDED_HYPER_OBJECT_H__
#define __INCLUDED_HYPER_OBJECT_H__

#include <map>
#include <boost/shared_ptr.hpp>
#include <sstream>
#include "koolib/koolibConfig.h"

#   ifndef KOOLIB_NON_CONTRIB
#   include "Useless/UselessConfig.h"
#   include "Useless/Error/Error.h"
#   endif


/*! HyperObject is a runtime object.
 *  HyperObject::_name is term name.
 *  Attributes of HyperObject are accessed via HyperObject::operator[] and HyperObject::get method.
 *
 *  Example:
 *
 *  HyperObject bookcase("bookcase");   // create new HyperObject
 *
 *  bookcase["book"]["chapters"]["introduction"] = HyperObject("chapter"); // add attribute "chapters"
 *  bookcase["book.chapters.introduction.numPages"] = 5; // alternative add attribute numPages
 *
 *  HyperObject *chapters = &bookcase["book"]["chapters"];  // query attributes
 *  HyperObject *chapters = &bookcase["book.chapters"]; // alternative notation
 *
 *  for ( HyperObject::iterator it = chapters->begin(); it != chapters->end(); ++it )
 *  {
 *      std::string chapterName = (*it).first;
 *      int numPages = (*(*it).second)["numPages"];
 *  }
 *
 */
struct KOOLIB_API HyperObject
{
    typedef std::map< std::string, boost::shared_ptr< HyperObject > > Map;
    typedef Map::iterator iterator;
    typedef Map::const_iterator const_iterator;
    Map _attributes;    
    std::string _name;
    HyperObject() {}
    HyperObject( const std::string &name ) :_name( name ) {}
    HyperObject( const char *name ) :_name( name ) {}
    HyperObject( int value ) { (*this) = value; }
    HyperObject( float value ) { (*this) = value; }
    HyperObject( double value ) { (*this) = value; }

    HyperObject & operator = ( const std::string value )
    {
        _name = value;
        return (*this);
    }

    template< class _T > HyperObject & operator = ( _T value )
    {
        std::stringstream s; s << value; _name = s.str();
        return (*this);
    }

    int as_int() const
    {
        std::stringstream s; s << _name; int y; s >> y;
        return y;
    }        
    
    float as_float() const
    {
        std::stringstream s; s << _name; float y; s >> y;
        return y;
    }        
    
    double as_double() const
    {
        std::stringstream s; s << _name; double y; s >> y;
        return y;
    }        
    
    operator std::string () const
    {
        return _name;
    }        

    bool has_attr( const std::string &name ) const
    {
        return ( _attributes.find( name ) != _attributes.end() );
    }

    bool empty() const { return _attributes.empty(); }
    
    int size() const { return _attributes.size(); }

    iterator begin() { return _attributes.begin(); }
    const_iterator begin() const { return _attributes.begin(); }
    
    iterator end() { return _attributes.end(); }
    const_iterator end() const { return _attributes.end(); }

    iterator find( const std::string &name ) { return _attributes.find( name ); }
    const_iterator find( const std::string &name ) const { return _attributes.find( name ); }

    boost::shared_ptr< HyperObject > & get_ptr( const std::string &name )
    {
        int start = 0, dot = 0;
        dot = name.find(".");
        if ( dot == -1 )
        {
            return _attributes[ name ];

        }
        return get( name.substr( start, dot-start ) ).get_ptr( name.substr( dot+1, name.size()-dot-1 ));
    }

    HyperObject & get( const std::string &name )
    {
        boost::shared_ptr< HyperObject > &ap = get_ptr( name );
        if ( !ap.get() )
        {
            ap.reset( new HyperObject );
        }
        return *ap;
    }

    HyperObject & get( const std::string &name ) const
    {
        boost::shared_ptr< HyperObject > &ap = const_cast< HyperObject *>( this )->get_ptr( name );
        if ( !ap.get() )
        {
#   ifndef KOOLIB_NON_CONTRIB
            throw Useless::Error("HyperObject: Bad attribute '%s'", name.c_str());
#   else
            throw std::string("HyperObject: Bad attribute '") + name + std::string("'");
#   endif
        }
        return *ap;
    }

    HyperObject & operator[] ( const HyperObject &name )
    {
        return get( name );
    }

    const HyperObject & operator [] ( const HyperObject &name ) const
    {
        return get( name );
    }
};


#   ifndef KOOLIB_NON_CONTRIB

#   include <koolib/xml/XMLFactory.h>
#   include "Useless/XML/Resources/CreateFromXML.h"
#   include "Useless/Util/XMLTag.h"

// May be used to tell Useless library to associate HyperObject with given xml tag.
Useless::Resource * CreateHyperObject( Useless::XMLIterator i, Useless::XMLCreatorEnv * );

// From XML deserialization.
void operator >> ( XMLFactory::Node node, HyperObject &o );

// To XML serialization.
void operator << ( Useless::XMLTag &outXML, const HyperObject &o );

typedef Useless::ResourceQuery< boost::shared_ptr< HyperObject > > HyperObjectResource;

// Register xml.hyper.data in Useless::Resources
static Useless::RegisterXMLCreator reg_create_hyper_data("hyper::data", CreateHyperObject );

#   endif

#endif//__INCLUDED_HYPER_OBJECT_H__
