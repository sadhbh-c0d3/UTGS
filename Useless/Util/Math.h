#ifndef __INCLUDED_USELESS_MATH_H__
#define __INCLUDED_USELESS_MATH_H__

/*
**  $Id: Math.h,v 1.5 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      Math
**
**  PURPOSE
**      Math support.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/


namespace Useless {


template <typename T, typename Y> inline T SimpleMin( T v1, Y v2 ) { return (v1<v2) ? v1 : v2; }
template <typename T, typename Y> inline T SimpleMax( T v1, Y v2 ) { return (v1>v2) ? v1 : v2; }


} // namespace Useless

#endif // __INCLUDED_USELESS_MATH_H__
