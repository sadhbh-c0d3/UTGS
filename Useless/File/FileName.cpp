#include "UselessPch.h"

#include "FileName.h"
#include "Useless/ErrorConfig.h"

namespace Useless {

#ifdef WIN32
    bool FileName::s_beCaseSensitive = false;
#else
    bool FileName::s_beCaseSensitive = true;
#endif

FileName::FileName( const std::string &name )
{
    std::string::const_iterator i=name.begin(), iend=name.end();

    if ( i!=iend )  {
        if ( *(iend-1)=='/' || *(iend-1)=='\\' ) { --iend; }
        if ( (*i)=='/' || (*i)=='\\' ) { ++i; }
    }

    char cur=0x0, prev=0x0;

    for (; i!=iend; ++i )
    {
        cur = (*i);
        if ( cur == '/' || cur == '\\' )
        {
            if ( prev == '/' || prev == '\\' ) { continue; }
            else { cur = '/'; }
        }
        _valid_name += cur;
    }
    
    if ( s_beCaseSensitive )
    {
        _search_name = _valid_name;
    }
    else
    {
        std::transform( _valid_name.begin(), _valid_name.end(),
                std::back_inserter( _search_name ), tolower );
    }
}

FileName& FileName::operator-=( const FileName &f )
{
    if ( f._search_name.empty() ) { return *this; }
    size_t pos = _search_name.find( f._search_name, 0 );
    if ( pos == 0 )
    {
        size_t p = f._search_name.size()+1;
        if ( p < _search_name.size() )
        {
            _valid_name = _valid_name.substr( p, -1);
            _search_name = _search_name.substr( p, -1);
        }
        else
        {
            _valid_name.resize(0);
            _search_name.resize(0);
        }
    }
    else
    {
        APP_ERROR( Error("operator -= failed"), "FileName");
    }
    return *this;
}

FileName& FileName::operator+=( const FileName &f )
{
    if ( !f._valid_name.empty() )
    {
        _valid_name += "/" + f._valid_name;
        _search_name += "/" + f._search_name;
    }
    return *this;
}


};//namespace Useless
