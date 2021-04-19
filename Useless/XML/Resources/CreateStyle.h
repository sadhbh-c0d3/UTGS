#ifndef __INCLUDED_USELESS_CREATE_STYLE_H__
#define __INCLUDED_USELESS_CREATE_STYLE_H__

#include "Useless/XML/Resources/CreateFromXML.h"

namespace Useless {

typedef std::map< std::string, XMLAttributes > XMLStyle;

USELESS_RESOURCES_API Resource* CreateStyle( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_style( "styles::style", CreateStyle );


inline const XMLStyle& QueryStyle( const std::string &id )
{
    XMLStyle *p_style;
    Resources::Instance().Query("styles", id, p_style );
    return *p_style;
}


void CascadeStyles
     (
          const XMLIterator &i,
          XMLCreatorEnv *env,
          XMLAttributes &attributes,
          XMLStyle &branch_style,
          XMLCreatorEnv &ch_env
      );

};//namespace Useless
#endif//__INCLUDED_USELESS_CREATE_STYLE_H__
