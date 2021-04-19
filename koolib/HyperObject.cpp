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

#include <koolib/HyperObject.h>

// May be used to tell Useless library to associate HyperObject with given xml tag.
Useless::Resource * CreateHyperObject( Useless::XMLIterator i, Useless::XMLCreatorEnv * )
{
    boost::shared_ptr< HyperObject > o( new HyperObject );
    XMLFactory::Node( i ) >> (*o);
    return new Useless::AnyResource< boost::shared_ptr< HyperObject > >( o );
}

// From XML deserialization.
void operator >> ( XMLFactory::Node node, HyperObject &o )
{
    if ( node->_value.empty())
    {
        o._name = node->_name;
    }
    else
    {
        o._name = Useless::make_string( node->_value );
    }
    // copy attributes;
    for ( std::map< std::string, std::string >::const_iterator
            itA = node->_attributes.begin();
            itA != node->_attributes.end(); ++itA )
    {
        o.get( (*itA).first ) = (*itA).second;
    }
    // add children objects
    for ( XMLFactory::Node child( node.GetFirstChild() ); !!child; ++child )
    {
        XMLFactory::Attr< std::string, false > id("id");
        if (!(child >> id))
        {
            id = child->_name;
        }
        child >> o.get( *id );
    }
}

// To XML serialization.
void operator << ( Useless::XMLTag &outXML, const HyperObject &o )
{
    // serialize attributes
    for ( HyperObject::const_iterator itA = o.begin();
            itA != o.end(); ++itA )
    {
        // attribute is an object (since it has other attributes)
        if (! (*itA).second->empty() )
        {
            Useless::XMLTag tag( (*itA).second->_name );
            tag.setattr("id", (*itA).first );
            tag << (*(*itA).second);
            outXML.appendvalue( tag );
        }
        // attribute is a string (does not have any attributes)
        else
        {
            outXML.setattr( (*itA).first, (*itA).second->_name );
        }
    }
}

