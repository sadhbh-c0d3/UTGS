#ifndef __INCLUDED_USELESS_HATCHERY_CONFIG_H__
#define __INCLUDED_USELESS_HATCHERY_CONFIG_H__

#include "Useless/File/CryptoIFile.h"
#include "Useless/File/CryptoOFile.h"
#include "Useless/File/BlockCipher.h"

#ifdef USELESS_HAS_CRYPTOPP
#   define __USE_RIJNDAEL__
//#define __USE_BLOWFISH__
#else
#pragma message("Get CryptoPP library and set USELESS_HAS_CRYPTOPP")
#   define __USE_XOR__
#endif
#define __ENCRYPT_DATA__


//---------------------------

#ifdef __USE_RIJNDAEL__
#   include <rijndael.h> // - CryptoPP library
#endif

#ifdef __USE_BLOWFISH__
#   include <blowfish.h> // - CryptoPP library
#endif

namespace Useless {

#ifdef __USE_RIJNDAEL__
struct HatcheryCipher
{
    typedef CryptoOFile< CryptoLibCipher< CryptoPP::RijndaelEncryption > > Out;
    typedef CryptoIFile< CryptoLibCipher< CryptoPP::RijndaelDecryption > > In;
    enum { BLOCKSIZE = Out::BLOCKSIZE };
};
static const char *__cipherType = "HCAS";
#endif

#ifdef __USE_BLOWFISH__
struct HatcheryCipher
{
    typedef CryptoOFile< CryptoLibCipher< CryptoPP::BlowfishEncryption > > Out;
    typedef CryptoIFile< CryptoLibCipher< CryptoPP::BlowfishDecryption > > In;
    enum { BLOCKSIZE = Out::BLOCKSIZE };
};
static const char *__cipherType = "HCBF";
#endif

#ifdef __USE_XOR__
struct HatcheryCipher
{
    typedef CryptoOFile< XorCipher<16> > Out;
    typedef CryptoIFile< XorCipher<16> > In;
    enum { BLOCKSIZE = Out::BLOCKSIZE };
};
static const char *__cipherType = "HCXR";
#endif

// The global key
extern USELESS_API const char *g_szKey;

};//namespace Useless
#endif//__INCLUDED_USELESS_HATCHERY_CONFIG_H__
