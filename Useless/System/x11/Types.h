#ifndef __INCLUDED_USELESS_TYPES_H__
#define __INCLUDED_USELESS_TYPES_H__

/*
**  $Id: Types.h,v 1.2 2003/01/20 20:54:09 koolas Exp $
**
**  NAME
**      Types
**
**  PURPOSE
**      Platform independent data types.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

namespace Useless {

/*! \defgroup Types Types */
namespace Types {

//@{
//! byte
typedef signed char      BYTE;
//! unsigned byte
typedef unsigned char    UBYTE;
//! signed 16 bit integer
typedef signed short     WORD;
//! unsigned 16 bit integer
typedef unsigned short   UWORD;
//! signed 32 bit integer
typedef signed long      LONG;
//! unsigned 32 bit integer
typedef unsigned long    ULONG;
//! signed 64 bit integer
typedef signed long long   QUAD;
//!unsigned 64 bit integer
typedef unsigned long long UQUAD;
//@}

} // namespace Types

} // namespace Useless
#endif // __INCLUDED_USELESS_TYPES_H__
