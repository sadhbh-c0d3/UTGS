#ifndef __INCLUDED_USELESS_INDEPENDENT_STORAGE_H__
#define __INCLUDED_USELESS_INDEPENDENT_STORAGE_H__

// Include all possible traits
#include "Useless/Util/std_traits.h"
#include "Useless/Util/Storage/VectorTraits.h"
// Include type checking
#include "Useless/Util/CTC.h"
#include "Useless/TemplateConfig.h"

namespace CTC {

template< class T >
struct SelectStorageDefaults
{
    typedef USELESS_TYPENAME CTC::IF< CTC::VectorTraits<T>::isVector, CTC::VectorDefaults<T>,
            USELESS_TYPENAME CTC::IF< CTC::std_traits<T>::is_std, CTC::std_defaults<T>, void >::type >::type type;
};

};//namespace CTC


/*! Helper pseudo-function to traverse whole container
 *
 *  Example:
 *      
 *  let's assume '_my_data' type is 'std::vector<int>' then
 *
 *  __FOR_EACH( std::vector<int>, i, _my_data)
 *  {
 *      printf( "%i\n", (*i));
 *  }
 *
 * Error: VC.NET !!! It doesn't seem to be working !!! (worked on vc6.0)
 * Use __FOR_EACH_USELESS() or __FOR_EACH_STD() instead
 *
 */
#define __FOR_EACH( C_Type, _I, _C) \
    for ( ::CTC::SelectStorageDefaults< C_Type >::type::iterator _I = \
          ::CTC::SelectStorageDefaults< C_Type >::type::begin_of(_C); \
         !::CTC::SelectStorageDefaults< C_Type >::type::is_end( _I, _C); \
          ++_I)

#define __FOR_EACH_C( C_Type, _I, _C) \
    for ( ::CTC::SelectStorageDefaults< C_Type >::type::const_iterator _I = \
          ::CTC::SelectStorageDefaults< C_Type >::type::begin_of(_C); \
         !::CTC::SelectStorageDefaults< C_Type >::type::is_end( _I, _C); \
          ++_I)

#define __FOR_EACH_USELESS( _T, _I, _C) \
    for ( _T::Iterator _I=_C.Begin(); !!_I; ++_I)
    
#define __FOR_EACH_USELESS_C( _T, _I, _C) \
    for ( _T::ConstIterator _I=_C.ConstBegin(); !!_I; ++_I)

#define __FOR_EACH_STD( _T, _I, _C) \
    for ( _T::iterator _I=_C.begin(); _I!=_C.end(); ++_I)
    
#define __FOR_EACH_STD_C( _T, _I, _C) \
    for ( _T::const_iterator _I=_C.begin(); _I!=_C.end(); ++_I)

#endif//__INCLUDED_USELESS_INDEPENDENT_STORAGE_H__
