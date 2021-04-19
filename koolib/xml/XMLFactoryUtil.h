#ifndef __INCLUDED_XML_FACTORY_UTIL_H__
#define __INCLUDED_XML_FACTORY_UTIL_H__

#include <koolib/xml/XMLFactory.h>

namespace XMLFactory {

    struct Integer
    {
        void set( int y ) { _int = y; }
        int GetInteger() const { return _int; }
        operator int () { return _int; }
        int _int;
    };

    struct Color : Integer
    {
        Color(){}
        Color( int c ) { set(c); }
    };

    struct CardinalPoint : Integer {};

    
    template< class _T >
    inline std::basic_stringstream< _T > & operator >> ( std::basic_stringstream< _T > &ss, Color &y )
    {
        std::basic_string< _T > xvalue = ss.str();
        std::string value;
        std::copy( xvalue.begin(), xvalue.end(), std::inserter( value, value.end() ));

             if ( value == "black" )        { y.set( 0xFF000000 ); }
        else if ( value == "white" )        { y.set( 0xFFFFFFFF ); }
        else if ( value == "dark grey" )    { y.set( 0xFF808080 ); }
        else if ( value == "light grey" )   { y.set( 0xFFC0C0C0 ); }
        else if ( value == "dark red" )     { y.set( 0xFF800000 ); }
        else if ( value == "red" )          { y.set( 0xFFFF0000 ); }
        else if ( value == "dark yellow" )  { y.set( 0xFF808000 ); }
        else if ( value == "yellow" )       { y.set( 0xFFFFFF00 ); }
        else if ( value == "dark green" )   { y.set( 0xFF008000 ); }
        else if ( value == "green" )        { y.set( 0xFF00FF00 ); }
        else if ( value == "dark cyan" )    { y.set( 0xFF008080 ); }
        else if ( value == "cyan" )         { y.set( 0xFF00FFFF ); }
        else if ( value == "dark blue" )    { y.set( 0xFF000080 ); }
        else if ( value == "blue" )         { y.set( 0xFF0000FF ); }
        else if ( value == "dark magenta" ) { y.set( 0xFF800080 ); }
        else if ( value == "magenta" )      { y.set( 0xFFFF00FF ); }
        else if ( value == "dark siena" )   { y.set( 0xFF808050 ); }
        else if ( value == "light siena" )  { y.set( 0xFFFFFF80 ); }
        else if ( value == "dark turquoise"){ y.set( 0xFF008080 ); }
        else if ( value == "turquoise" )    { y.set( 0xFF00FF80 ); }
        else if ( value == "azure" )        { y.set( 0xFF0080FF ); }
        else if ( value == "light azure" )  { y.set( 0xFF80FFFF ); }
        else if ( value == "ultramarine" )  { y.set( 0xFF80FFFF ); }
        else if ( value == "violet" )       { y.set( 0xFF8080FF ); }
        else if ( value == "blueberry" )    { y.set( 0xFF5000FF ); }
        else if ( value == "raspberry" )    { y.set( 0xFFFF0080 ); }
        else if ( value == "brown" )        { y.set( 0xFF805000 ); }
        else if ( value == "orange" )       { y.set( 0xFFFF8050 ); }
        else
        {
            y.set( Useless::to_hex< char >( value ));
        }
        return ss;
    }

    inline std::stringstream & operator >> ( std::stringstream &ss, CardinalPoint &y )
    {
        std::string value = ss.str();

             if ( value == "N" )    { y.set( 90 ); }
        else if ( value == "NW" )   { y.set( 135 ); }
        else if ( value == "W" )    { y.set( 180 ); }
        else if ( value == "SW" )   { y.set( 225 ); }
        else if ( value == "S" )    { y.set( 270 ); }
        else if ( value == "SE" )   { y.set( 315 ); }
        else if ( value == "E" )    { y.set( 0 ); }
        else if ( value == "NE" )   { y.set( 45 ); }
        else
        {
            y.set( Useless::to_integer< char >( value ));
        }
        return ss;
    }

    inline std::stringstream & operator >> ( std::stringstream &ss, TextUtf8 &y )
    {
        std::string value;
        ss >> value;
        y = TextUtf8( value );
        return ss;
    }

    /*inline std::stringstream & operator << ( std::stringstream &ss, const TextUtf8 &y )
    {
        std::string value = y.as_string();
		typedef std::stringstream & (*FT)( std::stringstream &, const std::string & );
        ((FT)(operator <<))( ss, value );
        return ss;
    }*/



};//namespace XMLFactory

#endif//__INCLUDED_XML_FACTORY_UTIL_H__
