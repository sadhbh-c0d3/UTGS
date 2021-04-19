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

#include <koolib/SymbolDict.h>
#include <koolib/FourCC.h>

#ifdef USELESS_HAS_CRYPTOPP_ADLER32
#include "Useless/Util/Crypto.h"
#endif

KOOLIB_API std::wstring SymbolDict::s_NoSymbol;

SymbolDict::SymbolDict()
{
    AddSymbol( L"head", FOURCC_LIST_HEAD );
    AddSymbol( L"tail", FOURCC_LIST_TAIL );
    AddSymbol( L"1-st", FOURCC_LIST_1ST );
    AddSymbol( L"2-nd", FOURCC_LIST_2ND );
    AddSymbol( L"3-rd", FOURCC_LIST_3RD );
    AddSymbol( L"4-th", FOURCC_LIST_4TH );
    AddSymbol( L"5-th", FOURCC_LIST_5TH );
    AddSymbol( L"6-th", FOURCC_LIST_6TH );
    AddSymbol( L"7-th", FOURCC_LIST_7TH );
    AddSymbol( L"8-th", FOURCC_LIST_8TH );
    AddSymbol( L"9-th", FOURCC_LIST_9TH );

    AddSymbol( L"func" );
    AddSymbol( L"list" );
    AddSymbol( L"accum" );
    AddSymbol( L"iter" );
    AddSymbol( L"count" );
    AddSymbol( L"first" );
    AddSymbol( L"dict" );
    AddSymbol( L"name" );
    AddSymbol( L"value" );
    AddSymbol( L"default" );
    AddSymbol( L"timeDelta" );
    AddSymbol( L"first" );
    AddSymbol( L"second" );
    AddSymbol( L"third" );
    AddSymbol( L"x" );
    AddSymbol( L"y" );
    AddSymbol( L"w" );
    AddSymbol( L"h" );
}

SymbolDict::~SymbolDict()
{
    FILE *useSymbols = fopen("Symbols.txt", "r");
    if ( useSymbols )
    {
        fclose( useSymbols );
        FILE *symbols = fopen("Symbols.txt", "a+");
        for ( Names::iterator it = _names.begin(); it != _names.end(); ++it )
        {
            char *c = (char*)(&(*it).second);
            fprintf( symbols, "%0x: {%c%c%c%c} '%S'\n",  (*it).second
                    , (c[0] > 0x20 ? c[0] : 0x20)
                    , (c[1] > 0x20 ? c[1] : 0x20)
                    , (c[2] > 0x20 ? c[2] : 0x20)
                    , (c[3] > 0x20 ? c[3] : 0x20)
                    , (*it).first.c_str() );
        }
        fprintf( symbols, "===\n");
        fclose( symbols );
    }
}

unsigned int
SymbolDict::AddSymbol    ( const std::wstring &name, unsigned int preferedId )
{
    Names::iterator itName = _names.find( name );
    
    if ( itName != _names.end() )
    {
        return (*itName).second;
    }
    
    if ( NO_SYMBOL == preferedId )
    {
#ifdef USELESS_HAS_CRYPTOPP_ADLER32
        preferedId = CalculateDigest( name );
#else
        wchar_t fchars[5] = {0,};
        std::memcpy( fchars, &name[0], std::min( (int)name.size(), (int)4 ));
        preferedId = Make4CC( fchars );
#endif
    }
        
    Numbers::iterator itNum = _numbers.find( preferedId );

    while ( itNum != _numbers.end() )
    {
        itNum = _numbers.find( ++preferedId );
    }

    _numbers[ preferedId ] = name;
    _names[ name ] = preferedId;

    return preferedId;
}

const std::wstring &
SymbolDict::GetSymbolName( unsigned int symbolId ) const
{
    Numbers::const_iterator it = _numbers.find( symbolId );

    if ( _numbers.end() == it )
    {
        return s_NoSymbol;
    }
    else
    {
        return (*it).second;
    }
}
    
unsigned int
SymbolDict::GetSymbolId  ( const std::wstring &name ) const
{
    Names::const_iterator it = _names.find( name );

    if ( _names.end() == it )
    {
        return 0;
    }
    else
    {
        return (*it).second;
    }
}

