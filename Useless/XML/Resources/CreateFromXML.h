#ifndef __INCLUDED_USELESS_CREATE_FROM_XML_H__
#define __INCLUDED_USELESS_CREATE_FROM_XML_H__

#pragma warning(disable:4786) // debug name turncation
#pragma warning(disable:4503) // debug name decorated turncation

#include "Useless/UselessConfig.h"
#include "Useless/Error/Error.h"
#include "Useless/Util/SPointer.h"
#include "Useless/XML/XMLParser.h"
#include "Useless/XML/Resources/Resources.h"

//#include <fstream>
#ifndef WIN32
#   include <typeinfo>
#endif

namespace Useless {

struct USELESS_RESOURCES_API XMLCreatorEnv : public std::map< std::string, void* >
{
    template< class T >
    bool Query( const std::string &attribute, T &ref )
    {
        iterator i=find(attribute), i2=end();
        if ( i==end() ) { return false; }
        ref = (T&)( (*i).second );
        if ( typeid(ref) != typeid(T) )
        {
            throw Error("XMLCreatorEnv::Query(): Invalid data type.");
        }
        return true;
    }

    template< class T >
    void Set( const std::string &attribute, T value )
    {
        void *bytes = 0;
        (T &)bytes = value;
        operator[]( attribute ) = bytes;
    }
};

/*! Map of resource creator functors
 */
class USELESS_RESOURCES_API __FromXMLCreators
{
public:
    typedef Resource* (*__Creator)( XMLIterator, XMLCreatorEnv* );

    Resource* Create( const std::string &category,
                      const XMLIterator &i,
                      XMLCreatorEnv *env )
    {
        CreatorMap::iterator k = _creators.find( category +"::"+ (*i)._name );
        if ( k!=_creators.end() )
        {
            __Creator &pfnCreate = (*k).second;
            return (*pfnCreate)(i, env);
        }
        else
        {
            throw Error("Element <%s> was not found in category <%s>.", (*i)._name.c_str(), category.c_str() );
        }
    }

    /*! type_id is a string: category::type_name
     *  f. ex. fonts::faced, skins::dicer etc.
     */
    void Register( const std::string &type_id, __Creator creator )
    {
        if ( _creators.find(type_id) == _creators.end() )
        {
            _creators.insert( std::make_pair( type_id, creator ) );
        }
    }

private:
    typedef std::map< std::string, __Creator > CreatorMap;
    CreatorMap _creators;
};

DECLARE_SINGLETON(__FromXMLCreators, XMLCreators, USELESS_RESOURCES_API );

struct RegisterXMLCreator
{
    RegisterXMLCreator( const std::string &type_id, __FromXMLCreators::__Creator creator )
    {
        XMLCreators::Instance().Register( type_id, creator );
    }
};

inline Resource* CreateXMLDocument( XMLIterator i, XMLCreatorEnv *env )
{
    // Store XML nodes for later execution
    XMLParser::Nodes nodes = i.SubTree();
    return (new AnyResource< XMLDocument >( nodes ));    
}

typedef Useless::ResourceQuery< Useless::XMLDocument > XMLDocumentResource;

/*! Create resources in given category
 */
USELESS_RESOURCES_API void CategoryFromXML( const std::string &category, XMLIterator i );

/*! Create child resources
 */
USELESS_RESOURCES_API void ChildsFromXML( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API void SiblingsFromXML( XMLIterator i, XMLCreatorEnv *env );

/*! Create all resources from XML string
 */
USELESS_RESOURCES_API void CreateFromXML( const std::string &xml_sring, const std::string &source = "< none >" );

/*! Create all resources from XML file
 */
USELESS_RESOURCES_API void CreateFromXMLFile( const std::string &xml_file );
USELESS_RESOURCES_API void CreateFromXMLFile( SPointer< class IFile > fileInput );


};//namespace Useless
#endif//__INCLUDED_USELESS_CREATE_FROM_XML_H__
