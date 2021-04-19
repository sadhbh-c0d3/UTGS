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
last mod: $Id: koolas Exp $

 ********************************************************************/

#include <string>
#include <map>
#include "koolib/koolibConfig.h"

#ifndef __INCLUDED_KOOLIB_SYMBOL_DICT_H__
#define __INCLUDED_KOOLIB_SYMBOL_DICT_H__
    
    struct KOOLIB_API SymbolDict
    {
        enum { NO_SYMBOL = 0 };
        static std::wstring s_NoSymbol;
        static SymbolDict & Instance() { static SymbolDict dict; return dict; }

        SymbolDict();
        ~SymbolDict();
        unsigned int        AddSymbol    ( const std::wstring &name, unsigned int preferedId = NO_SYMBOL );
        const std::wstring &GetSymbolName( unsigned int symbolId ) const;
        unsigned int        GetSymbolId  ( const std::wstring &name ) const;

        typedef std::map< std::wstring, unsigned int > Names; Names _names;
        typedef std::map< unsigned int, std::wstring > Numbers; Numbers _numbers;
    };


#endif//__INCLUDED_KOOLIB_SYMBOL_DICT_H__
