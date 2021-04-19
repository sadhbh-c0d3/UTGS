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

#ifndef __INCLUDED_KOOLIB_FOUR_CC_H__
#define __INCLUDED_KOOLIB_FOUR_CC_H__

inline unsigned int Make4CC( char a, char b, char c, char d )
{
    unsigned int x = ((unsigned int)a) + (((unsigned int)b) << 8) + (((unsigned int)c) << 16) + (((unsigned int)d) << 24);
    return x;
}

inline unsigned int Make4CC( const char *s )
{
    unsigned int x = ((unsigned int)s[0]) + (((unsigned int)s[1]) << 8) + (((unsigned int)s[2]) << 16) + (((unsigned int)s[3]) << 24);
    return x;
}

inline unsigned int Make4CC( const wchar_t *s )
{
    unsigned int x = (((unsigned int)s[0])&0xFF) + ((((unsigned int)s[1])&0xFF) << 8) +
        ((((unsigned int)s[2])&0xFF) << 16) + ((((unsigned int)s[3])&0xFF) << 24);
    return x;
}

inline std::string StringFrom4CC( unsigned int &fcc )
{
    std::string s("F4CC");
    s[0] = fcc & 0xFF;
    s[1] = (fcc >> 8) & 0xFF;
    s[2] = (fcc >> 16) & 0xFF;
    s[3] = (fcc >> 24) & 0xFF;
    return s;
}

static const unsigned int FOURCC_LIST_HEAD = Make4CC('h','e','a','d');
static const unsigned int FOURCC_LIST_TAIL = Make4CC('t','a','i','l');
static const unsigned int FOURCC_LIST_1ST  = Make4CC('1','-','s','t');
static const unsigned int FOURCC_LIST_2ND  = Make4CC('2','-','n','d');
static const unsigned int FOURCC_LIST_3RD  = Make4CC('3','-','r','d');
static const unsigned int FOURCC_LIST_4TH  = Make4CC('4','-','t','h');
static const unsigned int FOURCC_LIST_5TH  = Make4CC('5','-','t','h');
static const unsigned int FOURCC_LIST_6TH  = Make4CC('6','-','t','h');
static const unsigned int FOURCC_LIST_7TH  = Make4CC('7','-','t','h');
static const unsigned int FOURCC_LIST_8TH  = Make4CC('8','-','t','h');
static const unsigned int FOURCC_LIST_9TH  = Make4CC('9','-','t','h');

static const unsigned int FOURCC_NAME_TYPE = Make4CC('t','y','p','e');
static const unsigned int FOURCC_NAME_LAZY = Make4CC('l','a','z','y');
static const unsigned int FOURCC_NAME_CONST= Make4CC('c','o','n','s');
static const unsigned int FOURCC_NAME_GET  = Make4CC('g','e','t','c');

static const unsigned int FOURCC_TYPE_LIST  = Make4CC('l','i','s','t');
static const unsigned int FOURCC_TYPE_BLOCK = Make4CC('b','l','o','c');
static const unsigned int FOURCC_TYPE_INT   = Make4CC('i','n','t','e');
static const unsigned int FOURCC_TYPE_FLOAT = Make4CC('f','l','o','a');
static const unsigned int FOURCC_TYPE_STRING= Make4CC('s','t','r','i');
static const unsigned int FOURCC_TYPE_TEXT  = Make4CC('t','e','x','t');
static const unsigned int FOURCC_TYPE_EMPTY = Make4CC('e','m','p','t');
static const unsigned int FOURCC_TYPE_LAZY  = Make4CC('l','a','z','y');

#endif//__INCLUDED_KOOLIB_FOUR_CC_H__
