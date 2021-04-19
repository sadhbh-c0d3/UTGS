#ifndef __INCLUDED__THE_USELESS_H__
#define __INCLUDED__THE_USELESS_H__

#include "Useless/Util/TemplateDefines.h"
#include "Useless/Util/SafePtr.h" // - preserves safety of multiple simultanous creations

namespace Useless {

/*! Creates any-class instance
 *
 *  User may need to include other creators.
 *
 * \sa CMedia.h CSound.h CScreen.h
 */
template< class T >
class Creator
{
public:
    typedef T type;

    _TPL_1 static T* Create(  _TPL_ARGS_1 ) { return new T(_TPL_PASS_ARGS_1); }
    _TPL_2 static T* Create(  _TPL_ARGS_2 ) { return new T(_TPL_PASS_ARGS_2); }
    _TPL_3 static T* Create(  _TPL_ARGS_3 ) { return new T(_TPL_PASS_ARGS_3); }
    _TPL_4 static T* Create(  _TPL_ARGS_4 ) { return new T(_TPL_PASS_ARGS_4); }
    _TPL_5 static T* Create(  _TPL_ARGS_5 ) { return new T(_TPL_PASS_ARGS_5); }
    _TPL_6 static T* Create(  _TPL_ARGS_6 ) { return new T(_TPL_PASS_ARGS_6); }
    _TPL_7 static T* Create(  _TPL_ARGS_7 ) { return new T(_TPL_PASS_ARGS_7); }
    _TPL_8 static T* Create(  _TPL_ARGS_8 ) { return new T(_TPL_PASS_ARGS_8); }
};

/*------------------------------------------------*/
template< class T > SafePtr<T> Create()
{
    return Creator<T>::Create();
}

#define __CREATE_MACRO(n) \
template< class T, _TPL_##n##_ > SafePtr<T> Create( _TPL_ARGS_##n ) \
{ \
    return Creator<T>::Create(_TPL_PASS_ARGS_##n); \
}
__CREATE_MACRO(1)
__CREATE_MACRO(2)
__CREATE_MACRO(3)
__CREATE_MACRO(4)
__CREATE_MACRO(5)
__CREATE_MACRO(6)
__CREATE_MACRO(7)
__CREATE_MACRO(8)

#undef __CREATE_MACRO

};//namespace Useless


#endif//__INCLUDED__THE_USELESS_H__
