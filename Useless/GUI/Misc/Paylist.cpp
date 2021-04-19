#include "UselessPch.h"

#include "Useless/GUI/Misc/Paylist.h"
#include "Useless/GUI/Misc/DT_Painter.h"

#include <fstream>
#include <strstream>
#include "Useless/Util/str_conv.h"
#include "Useless/Util/stl_addon.h"
#include "Useless/File/IFileStream.h"
#include "Useless/File/VIFS.h"

namespace Useless {

Paylist::Paylist(): _distance(0), _length(0)
{
}

Paylist::Paylist( const Font &header_font, const Font &normal_font, int distnace )
    :_header_font(header_font), _normal_font(normal_font), _distance(distnace)
{
}

Paylist::Paylist( const char *paylist_file ): _distance(8)
{
    std::string ln,data;
    //std::fstream desc_file( paylist_file );
    IFileStream desc_file( IFS::Instance().OpenFile(paylist_file));

    while ( std::getline( desc_file, ln) )
    {
        data += ln;
        data += "\n";
    }
    //XMLParser xml(data);
    XMLDocument xml(data);

    //XMLParser::Iterator i=xml.begin(), i2=xml.end();
    XMLIterator i=xml.Root();
    LoadFromXML( i );
}

Paylist::Paylist( XMLIterator i ): _distance(8)
{
    LoadFromXML( i );
}

void
Paylist::LoadFromXML( XMLIterator i )
{
    if ( (*i)._name != "paylist" )
    {
        throw Error("Paylist: error - no paylist tag");
    }
    try_attribute_i( (*i)._attributes, "distance", &_distance );

    //for (; i!=i2; ++i )
    for ( i.StepInto(); !!i; ++i )
    {
        if ( (*i)._name == "font"  )
        {
            const std::string& tpe = get_attribute( (*i)._attributes, "type" );
            const std::string& img = get_attribute( (*i)._attributes, "image" );
            const std::string& dsc = get_attribute( (*i)._attributes, "desc" );
            if ( tpe == "normal" )
            {
                _normal_font = Font( img.c_str(), dsc.c_str() );
            }
            else if ( tpe == "header" )
            {
                _header_font = Font( img.c_str(), dsc.c_str() );
            }
        }
        else if ( (*i)._name == "image" )
        {
            const std::string& src = get_attribute( (*i)._attributes, "src" );
            std::string str;
            int placement = 50;
            if ( try_attribute( (*i)._attributes, "placement", &str ) )
            {
                placement = to_integer( str );
            }
            if ( try_attribute( (*i)._attributes, "color_key", &str ) )
            {
                unsigned int key = to_hex( str );
                Lock().push_back( new DT_Image( Image(src.c_str(), key), placement) );
            }
            else
            {
                Lock().push_back( new DT_Image( src.c_str(), placement) );
            }
        }
        else if ( (*i)._name == "text" )
        {
            int placement = 0;
            try_attribute_i( (*i)._attributes, "placement", &placement );

            std::wstring v;
            XMLIterator j = i;
            for ( j.StepInto(); !!j; ++j )
            {
                assert( (*j)._name=="p" );
                v += (*j)._value;
                v += L"\n";
            }

            Lock().push_back( new DT_Text( v, placement ) );
        }
        else if ( (*i)._name == "entry" )
        {
            const std::string& nam = get_attribute( (*i)._attributes, "name" );
            const std::string& val = get_attribute( (*i)._attributes, "value" );
            int name_plc=50, value_plc=50;

            try_attribute_i( (*i)._attributes, "placement", &name_plc );
            value_plc = name_plc;
            try_attribute_i( (*i)._attributes, "name_placement", &name_plc );
            try_attribute_i( (*i)._attributes, "value_placement", &value_plc );

            Lock().push_back( new DT_PayEntry( nam.c_str(), val.c_str(), name_plc, value_plc) );
        }
        else if ( (*i)._name == "entries" )
        {
            const std::string& nam = get_attribute( (*i)._attributes, "name" );
            int columns = 1, name_plc = 50, value_plc = 0;
            try_attribute_i( (*i)._attributes, "columns", &columns );
            try_attribute_i( (*i)._attributes, "placement", &name_plc );
            value_plc = name_plc;
            try_attribute_i( (*i)._attributes, "name_placement", &name_plc );
            try_attribute_i( (*i)._attributes, "value_placement", &value_plc );
            DT_PayEntries *p = new DT_PayEntries( nam.c_str(), columns, name_plc, value_plc );

            /*std::strstream interpreter;
            interpreter << (*i).second._value;
            std::string entry;
            while( std::getline( interpreter, entry))
            {
                if ( !entry.empty())
                {   p->Add( entry );
                }
            }*/
            XMLIterator j = i;
            for ( j.StepInto(); !!j; ++j )
            {
                assert( (*j)._name=="e" );
                p->Add( (*j)._value );
            }
            Lock().push_back(p);
        }
    }
}

Paylist::~Paylist()
{
}

void Paylist::Paint( const Painter &painter, Rect area )
{
    Iterator i = begin();
    Iterator j = end();
    
    int y0 = area.GetY();

    for ( ; i!=j; ++i )
    {
        DT_Painter paint( painter, _header_font, _normal_font, area );
        area += Pos( 0, (*i)->AcceptVisitor( paint ) + _distance );
    }

    _length = area.GetY() - y0;
}

};//namespace Useless
