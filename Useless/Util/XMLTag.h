#ifndef __INCLUDED_USELESS_XML_TAG_H__
#define __INCLUDED_USELESS_XML_TAG_H__

#include "Useless/Util/str_conv.h"

namespace Useless {

struct XMLTag
{
    std::string to_string() const
    {
        if ( -1 != _val.find('\n') )
        {
            static const std::string indent = "    ";
            std::stringstream ss;
            ss << _val;
            std::string line;
            std::string formatedValue;

            while ( std::getline( ss, line ) )
            {
                formatedValue += indent + line + "\n";
            }
        
            return ("<" + _name + _str + ">\n" + formatedValue + "</" + _name + ">\n");
        }
        else
        {
            if ( _val.empty() )
            {
                return ("<" + _name + _str + "/>\n");
            }
            else
            {
                return ("<" + _name + _str + ">" + _val + "</" + _name + ">\n");
            }
        }
    }

    template< class _V >
        struct str_conv_traits
        {
            static std::string to_string( const _V & v ) { return std::string( v ); }
        };

    template<>
        struct str_conv_traits< int >
        {
            static std::string to_string( int v ) { return from_integer< char >( v ); }
        };
    
    template<>
        struct str_conv_traits< float >
        {
            static std::string to_string( float v ) { return from_real< char >( v ); }
        };
    
    template<>
        struct str_conv_traits< double >
        {
            static std::string to_string( double v ) { return from_real< char >( v ); }
        };

    template<>
        struct str_conv_traits< XMLTag >
        {
            static std::string to_string( const XMLTag &v ) { return v.to_string(); }
        };


    XMLTag( std::string name ) : _name( name )
    {}

    template< class _V >
        XMLTag & setattr( std::string attrName, const _V & attrValue )
        {
            typedef str_conv_traits< _V > conv;
            _str += " " + attrName + "=\"" + conv::to_string( attrValue ) + "\"";
            return (*this);
        }

    template< class _V >
        XMLTag & setvalue( const _V & value )
        {
            typedef str_conv_traits< _V > conv;
            _val = conv::to_string( value );
            return (*this);
        }
    
    template< class _V >
        XMLTag & appendvalue( const _V & value )
        {
            typedef str_conv_traits< _V > conv;
            _val += conv::to_string( value );
            return (*this);
        }
    
    std::string _name, _str, _val;
};

}; //Useless



#endif//__INCLUDED_USELESS_XML_TAG_H__
