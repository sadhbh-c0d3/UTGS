/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE KooLAS C++ Template & Class Library     *
 * CALLED KooLib SOURCE CODE.                                       *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE KooLib SOURCE CODE IS (C) COPYRIGHT 2004                     *
 * by the Sadhbh Code (https://github.com/sadhbh-c0d3)              *
 *                                                                  *
 ********************************************************************

  function:
  last mod: $Id: dynamic_cast_call.h,v 1.1.1.1 2004/06/02 19:35:52 koolas Exp $

 ********************************************************************/


#ifndef __INCLUDED_DYNAMIC_CAST_CALL_H__
#define __INCLUDED_DYNAMIC_CAST_CALL_H__

template< class _T, class _S, class _F >
bool dynamic_cast_call( _S *ptr, _F f )
{
    if ( _T * ptr1 = dynamic_cast< _T * >( ptr ))
    {
        ((*ptr1).*f)();
        return true;
    }
    else
    {
        return false;
    }
}

template< class _T, class _S, class _F, class _A1 >
bool dynamic_cast_call( _S *ptr, _F f, const _A1 &a1 )
{
    if ( _T * ptr1 = dynamic_cast< _T * >( ptr ))
    {
        ((*ptr1).*f)( a1 );
        return true;
    }
    else
    {
        return false;
    }
}

template< class _T, class _S, class _F, class _A1, class _A2 >
bool dynamic_cast_call( _S *ptr, _F f, const _A1 &a1, const _A2 &a2 )
{
    if ( _T * ptr1 = dynamic_cast< _T * >( ptr ))
    {
        ((*ptr1).*f)( a1, a2 );
        return true;
    }
    else
    {
        return false;
    }
}

template< class _T, class _S, class _F, class _A1, class _A2, class _A3 >
bool dynamic_cast_call( _S *ptr, _F f, const _A1 &a1, const _A2 &a2, const _A3 &a3 )
{
    if ( _T * ptr1 = dynamic_cast< _T * >( ptr ))
    {
        ((*ptr1).*f)( a1, a2, a3 );
        return true;
    }
    else
    {
        return false;
    }
}

template< class _T, class _S, class _F, class _A1, class _A2, class _A3, class _A4 >
bool dynamic_cast_call( _S *ptr, _F f, const _A1 &a1, const _A2 &a2, const _A3 &a3, const _A4 &a4 )
{
    if ( _T * ptr1 = dynamic_cast< _T * >( ptr ))
    {
        ((*ptr1).*f)( a1, a2, a3, a4 );
        return true;
    }
    else
    {
        return false;
    }
}


#endif//__INCLUDED_DYNAMIC_CAST_CALL_H__
