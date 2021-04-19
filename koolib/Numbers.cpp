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
  last mod: $Id: Numbers.cpp,v 1.2 2004/08/28 13:32:53 koolas Exp $

 ********************************************************************/


#include "koolib/Numbers.h"
#include <algorithm>

namespace Debug {
    
    std::pair< int, int > decode_fixed_number( Numbers::Fixed< 8 > &x )
    {
        return std::pair< int, int >( x, x.GetFraction() );
    }
    
    float decode_float_fixed_number( Numbers::Fixed< 8 > &x )
    {
        return (float)x;
    }

};//Debug
