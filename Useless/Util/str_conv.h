#ifndef __INCLUDED_USELESS_STR_CONV_H__
#define __INCLUDED_USELESS_STR_CONV_H__

#include <sstream>
#include <string>
#include <vector>
#include <iterator>

namespace Useless {

template< class char_type1 > struct easy_to_string_t
{
	static char_type1 txc( char c )
	{
		return (char_type1)c;
	}

	static char_type1 txwc( wchar_t wc )
	{
		return (char_type1)wc;
	}

	/*! Copy narrow characters to wide string filling higher bits with 0.
	*/
	static std::basic_string< char_type1 > apply( const std::basic_string< char > &s )
	{
		std::basic_string< char_type1 > result;
		std::transform( s.begin(), s.end(), std::back_inserter( result )
			, &easy_to_string_t< char_type1 >::txc);
		return result;
	}

	/*! Copy wide characters to narrow string turncating higher bits.
	*/
	static std::basic_string< char_type1 > apply( const std::basic_string< wchar_t > &s )
	{
	    std::basic_string< char_type1 > result;
	    std::transform( s.begin(), s.end(), std::back_inserter( result )
			, &easy_to_string_t< char_type1 >::txwc);
	    return result;
	}
};

template< typename char_type1, typename char_type2 >
std::basic_string< char_type1 > easy_to_string( const std::basic_string< char_type2 > &s )
{
	return easy_to_string_t< char_type1 >::apply( s );
}


inline std::basic_string< char > make_string( const std::basic_string< wchar_t > &s )
{
    return Useless::easy_to_string< char >( s );
}

inline std::basic_string< wchar_t > make_wstring( const std::basic_string< char > &s )
{
    return Useless::easy_to_string< wchar_t >( s );
}


/*! Remove white spaces from begining and end of the string
 */
template< class char_type >
std::basic_string< char_type > chomp
    ( const std::basic_string< char_type > &s,
	  const std::basic_string< char_type > &white_string =
		Useless::easy_to_string_t< char_type >::apply(" \t\r\n") )
{
    int i = 0;
    int j = s.length();
    
    while (( white_string.find( s[i] ) != -1 ) && ( ++i < j ))
    {}
    
    while ((i < --j) && ( white_string.find( s[j] ) != -1 ))
    {}
        
    if ( i < ++j )
    {
        return s.substr( i, j-i );
    }
    else
    {
        return std::basic_string< char_type >();
    }
}

/*! Converts hexadecimal string into integer
 */
template< class char_type >
int to_hex( const std::basic_string< char_type > &s )
{
    unsigned int u=0;
	if ( s.substr(0,2) == Useless::easy_to_string_t< char_type >::apply("0x") )
    {
        for ( int j=0,i=s.length(); i>2; --i, ++j )
        {
            char_type c = s[i-1];
            c = (c>='0' && c<='9')? c-'0' :
                (c>='A' && c<='F' || c>='a' && c<='f')? c-'A'+0xA : 0;

            u |= (c & 0xF) << (j*4);
        }
    }
    return u;
}

/*! Converts numeric string into integer
 */
template< class char_type >
int to_integer( const std::basic_string< char_type > &s )
{
    std::basic_stringstream< char_type > i; i << s;
    int x; i>>x; return x; 
}

/*! Converts integer to numeric string
 */
template< class char_type >
std::basic_string< char_type > from_integer( int x )
{
    std::basic_stringstream< char_type > i; i << x;
    std::basic_string< char_type > s; i>>s; return s; 
}

/*! Converts numeric string to double prec. float
 */
template< class char_type >
double to_real( const std::basic_string< char_type > &s )
{
    std::basic_stringstream< char_type > i; i << s;
    double x; i>>x; return x; 
}

/*! Converts double to numeric string
 */
template< class char_type >
std::basic_string< char_type > from_real( double x )
{
    std::basic_stringstream< char_type > i; i << x;
    std::basic_string< char_type > s; i>>s; return s; 
}

/*! Converts escaped character code to char
 */
template< class char_type >
int to_char( const std::basic_string< char_type > &s )
{
    if ( s[0]=='\\' )
    {
        std::basic_stringstream< char_type > i; i << s.substr(1,-1);
        int x; i>>x; return x; 
    }
    else { return s[0]; }
}

/*! Tokenizes string into vector of numbers (removes white spaces)
 */
template< class char_type >
std::vector<int> to_integer_vector__(
        const std::basic_string< char_type > &s,
        const std::basic_string< char_type > &delimiter = Useless::easy_to_string< char_type >(",") )
{
    std::vector<int> out;
    int x, q=0, p=0;
    
    do {
        p = s.find( delimiter, p+1);
        std::basic_stringstream< char_type > i;
        i << s.substr( q, p-q);
        i>>x;
        out.push_back(x);
        q=p+1;
    } while ( p != -1 );
   
    return out;
}

inline std::vector< int > to_integer_vector(
        const std::basic_string< char > &s,
        const std::basic_string< char > &delimiter = ",")
{
    return to_integer_vector__( s, delimiter );
}

inline std::vector< int > to_integer_vector(
        const std::basic_string< wchar_t > &s,
        const std::basic_string< wchar_t > &delimiter = L",")
{
    return to_integer_vector__( s, delimiter );
}

/*! Tokenizes string into vector of sub-strings (removes white spaces)
 */
template< class char_type >
std::vector< std::basic_string< char_type > > to_tokens__(
        const std::basic_string< char_type > &s,
        const std::basic_string< char_type > &delimiter = Useless::easy_to_string< char_type >(",") )
{
    std::vector< std::basic_string< char_type > > out;
    int q=0, p=0;
    std::basic_string< char_type > token;
    
    do {
        p = s.find( delimiter, p+1 );
        std::basic_stringstream< char_type > i;
        i << s.substr( q, p-q);
        i >> token;
        out.push_back(token);
        q=p+1;
    } while ( p != -1 );
   
    return out;
}

inline std::vector< std::basic_string< char > > to_tokens(
        const std::basic_string< char > &s,
        const std::basic_string< char > &delimiter = "," )
{
    return to_tokens__( s, delimiter );
}

inline std::vector< std::basic_string< wchar_t > > to_tokens(
        const std::basic_string< wchar_t > &s,
        const std::basic_string< wchar_t > &delimiter = L"," )
{
    return to_tokens__( s, delimiter );
}

/*! Converts string containing flag names to integer built of flags
 */
template< class char_type, class FlagMap >
int to_flags( const std::basic_string< char_type > &s, const FlagMap &flag_names )
{
    typedef typename FlagMap::const_iterator iterator;
    iterator p=flag_names.begin(), q=flag_names.end();
    int flags = 0;

    for (; p!=q; ++p )
    {
        if ( s.find( (*p).first ) != -1 ) { flags|=(*p).second; }
    }

    return flags;
}

/*! Get unit ( '100 px' => 'px', '10%' => '%' )
 */
template< class char_type >
std::basic_string< char_type > get_unit( const std::basic_string< char_type > &s )
{
    const char_type *i=&*s.begin(), *i2=&*s.end();
    for (; i!=i2; ++i )
    {
        if ( (*i) < '0' || (*i) > '9' ) { break; }
    }
    return chomp( s.substr() );
}

/* Matches parentheses in string from given position
 */
template< class char_type >
std::pair<int, int> match_parenthess(
        const std::basic_string< char_type > &s, int p=0, char_type par='[' )
{
    char_type lt, rt;
    std::pair<int, int> x;
    x.first=x.second=-1;

    if ( par == '[' ) { lt='['; rt=']'; } else
    if ( par == '(' ) { lt='('; rt=')'; } else
    if ( par == '<' ) { lt='<'; rt='>'; } else
    if ( par == '{' ) { lt='{'; rt='}'; } else
    { return x; }

    int l = s.find( lt, p );
    int r = s.find( rt, p );
    if ( l!=-1 && r!=-1 && l < r )
    {
        x.first=l; x.second=r;
    }
    return x;
}

/*! Finds in string escaped character codes and converts them to chars
 */
template< class char_type >
    std::basic_string< char_type > to_chars( const std::basic_string< char_type > &s )
    {
        std::basic_string< char_type > str;

        typedef std::basic_string< char_type >::const_iterator const_iterator;
        const_iterator i = s.begin();
        const_iterator iEnd = s.end();

        while (i != iEnd)
        {
            if (*i == '\\') 
            {
                ++i;
                std::string sub="\\";
                for (int count = 0;  (i != iEnd) && (*i >= '0') && (*i <= '9'); ++count) 
                { 
                    sub += (*i);
                    ++i;
                }
                str += (sub.empty() ? (*i) : to_char(sub));
            }
            else
            {
                str += (*i);
                ++i;
            }
        }
        return str;
    }

};//namespace Useless
#endif //__INCLUDED_USELESS_STR_CONV_H__
