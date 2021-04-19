#include "UselessPch.h"

#include "Useless/Util/Crypto.h"
#include "Useless/File/CryptoIFile.h"
#include "Useless/File/CryptoOFile.h"
#include "Useless/File/Hatchery.h"

#ifdef USELESS_HAS_CRYPTOPP
#   include <adler32.h>
#endif

namespace Useless {

    SPointer< IFile >       CreateEncryptedIFile( SPointer< IFile > f, const std::string &k )
    {
        return new HatcheryCipher::In( f, (!k.empty() ? k : g_szKey ));
    }

    SPointer< OFile >       CreateEncryptedOFile( SPointer< OFile > f, const std::string &k )
    {
        return new HatcheryCipher::Out( f, (!k.empty() ? k : g_szKey ));
    }
    
    SPointer< IFileSystem > OpenHatchery        ( const std::string &n, const std::string &b, const std::string &k )
    {
        SPointer< Hatchery > htc = new Hatchery();
        if ( !k.empty() )
        {
            htc->SetKey( k );
        }
        htc->Open( n, b );
        return htc;
    }
    
#ifdef USELESS_HAS_CRYPTOPP
    int CalculateDigest ( const void *d, size_t s )
    {
        int out;
        CryptoPP::Adler32().CalculateDigest( (byte*)&out, (byte*)d, s );
        return out;
    }
#endif

    void AlignForEncryption  ( std::string &d, char c )
    {
        int remainAlgn = d.length() % HatcheryCipher::BLOCKSIZE;        
        if ( 0 != remainAlgn )
        {
            int toAlign = HatcheryCipher::BLOCKSIZE - remainAlgn;
            while ( 0 < toAlign-- )
            {
                d += c;
            }
        }
    }

};//Useless
