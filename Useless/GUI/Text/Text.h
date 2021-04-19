#ifndef __INCLUDED_USELESS_TEXT_H__
#define __INCLUDED_USELESS_TEXT_H__

#include <string>
#include "Useless/Util/Storage/SortedVector.h"
#include "Useless/Util/StaticCheck.h"
#include "Useless/System/Assert.h"

namespace Useless {

//Everything in 16bit Unicode
typedef wchar_t UniCode;
typedef std::basic_string< UniCode >  UniCodeString;

class USELESS_API Text : public UniCodeString
{
public:
    Text() {}

    Text( const unsigned char  *pchar ) { MakeText(pchar);  }
    //Text( const unsigned short *pshort) { MakeText(pshort); } wchar_t ambiguos
    Text( const unsigned int   *pint  ) { MakeText(pint);   }
    Text( const unsigned long  *plong ) { MakeText(plong);  }

    Text( const signed char  *pchar ) { MakeText( (unsigned char *)(pchar) ); }
    Text( const signed short *pshort) { MakeText( (unsigned short*)(pshort)); }
    Text( const signed int   *pint  ) { MakeText( (unsigned int  *)(pint)  ); }
    Text( const signed long  *plong ) { MakeText( (unsigned long *)(plong) ); }
    
    Text( const char    *pchar,  ... );//!< sprintf formated text
    Text( const wchar_t *pshort, ... );//!< swprintf formated text

    Text( UniCode code )       { UniCodeString::operator+=(code);  }

    Text( const std::basic_string<unsigned char > &data ) { MakeFromString(data); }
    Text( const std::basic_string<unsigned short> &data ) { MakeFromString(data); }
    Text( const std::basic_string<unsigned int  > &data ) { MakeFromString(data); }
    Text( const std::basic_string<unsigned long > &data ) { MakeFromString(data); }

    Text( const std::basic_string<char > &data ) { MakeFromString( (const std::basic_string<unsigned char >&)data); }
    Text( const std::basic_string<short> &data ) { MakeFromString( (const std::basic_string<unsigned short>&)data); }
    Text( const std::basic_string<int  > &data ) { MakeFromString( (const std::basic_string<unsigned int >&)data); }
    Text( const std::basic_string<long > &data ) { MakeFromString( (const std::basic_string<unsigned long>&)data); }
    Text( const std::basic_string< wchar_t > &data ) { MakeFromString( (const std::basic_string< wchar_t >&)data); }

    //Safe operator[]
    UniCode        operator[]( int pos ) const;

    //Concatenation operator
    Text& operator +=( const Text &data);
    Text  operator + ( const Text &data) const;
    bool  operator ==( const Text &data) const;
    bool  operator !=( const Text &data) const;


    //Create substring
    Text   operator()(int start, int end=-1) const;

    //Cut & Paste
    Text&  Cut    (int start, int end=-1);
    Text&  Paste  (int pos, const Text &text);
    Text&  Replace(int start, int end, const Text &replacement);

      int Match  (const Text& pattern, StupidVector<int> &matches) const;
     long Length () const;

     std::string
         GetString() const;
     
     std::basic_string<short>
         GetWString() const;

private:
    template< class T > void MakeText(const T *p);
    template< class T > void MakeFromString( const std::basic_string<T> &s );
   
};

////////////////////////////////////////////////////////////

template< class T > void Text::MakeText(const T* p)
{
    //STATIC_CHECK( (const bool)(CTC::Conversion< T, UniCode>::exists), __CAST_ERROR__MUST_BE_INTEGRAL_ARRAY__);

    while( *p >0 )
    {
        UniCode code = *(p++);
        UniCodeString::operator+=(code);
    }
}

template< class T > void Text::MakeFromString( const std::basic_string<T> &s )
{
    resize( s.size() );
    iterator j = begin();
    for ( std::basic_string<T>::const_iterator i = s.begin(); i != s.end(); ++i, ++j )
    {
        *j = (UniCode)( *i );
    }
}

//Safe operator[]
inline UniCode Text::operator[]( int pos ) const
{
    try { 
        return UniCodeString::operator[](pos);
    }
    catch(...)   
    {  
        return UniCode(0); 
    }
}

inline long Text::Length() const
{
    return UniCodeString::length();
}

inline Text Text::operator ()(int start, int end) const
{
    return Text( substr( start, (end==-1)? -1 : end-start ) );
}

//Concatenation operator
inline Text& Text::operator +=( const Text &data)
{
    UniCodeString::operator+=(data);
    return *this;
}

inline Text Text::operator  +( const Text &data) const
{
    return Text(*this) += data;
}

inline bool Text::operator ==( const Text &data) const
{
    return std::operator == ( (const UniCodeString&)*this, (const UniCodeString&)data );
}

inline bool Text::operator !=( const Text &data) const
{
    return std::operator != ( (const UniCodeString&)*this, (const UniCodeString&)data );
}


};//namespace Useless
#endif//__INCLUDED_USELESS_TEXT_H__
