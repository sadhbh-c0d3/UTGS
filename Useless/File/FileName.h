#ifndef __INCLUDED_USELESS_FILENAME_H__
#define __INCLUDED_USELESS_FILENAME_H__

#include <string>

namespace Useless {

class USELESS_API FileName
{
public:
    FileName() {}
    FileName( const std::string &name );

    operator const std::string&() { return _valid_name; }

    const std::string &Get() const { return _valid_name; }
    const std::string &GetSearchName() const { return _search_name; }

    FileName& operator-=( const FileName &f );
    FileName& operator+=( const FileName &f );

    bool operator < ( const FileName &fn ) const
        { return _search_name < fn._search_name; }
    
private:
    std::string _valid_name;
    std::string _search_name;

public:
    static bool s_beCaseSensitive;
};

inline FileName operator-( FileName a, const FileName &b ) { return a-=b; }
inline FileName operator+( FileName a, const FileName &b ) { return a+=b; }


};//namespace Useless

#endif//__INCLUDED_USELESS_FILENAME_H__
