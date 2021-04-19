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
last mod: $Id: $

 ********************************************************************/

#include "koolib/TextUtf8.h"
#include "Useless/Util/UTF8.h"


void ReadUtf8( std::wstring &o, const std::string &i )
{
    Useless::ReadUtf8( o, i );
}

void WriteUtf8( std::string &o, const std::wstring &i )
{
    Useless::WriteUtf8( o, i );
}
