#ifndef __INCLUDED_USELESS_BLOCK_CIPHER_H__
#define __INCLUDED_USELESS_BLOCK_CIPHER_H__

#include <string>

namespace Useless {
/*! cipher primitive
\verbatim
    template< int BlockSize >
    struct BlockCipher
    {
        enum { BLOCKSIZE = BlockSize };
        BlockCipher( const std::string &key );
        void ProcessBlock( const char *in, char * out );
    };
\endverbatim
*/


/*! Crypto++ library cipher
 */
template< class CryptoLib_BlockTransformation >
struct USELESS_API CryptoLibCipher : public CryptoLib_BlockTransformation
{
    enum { BLOCKSIZE = CryptoLib_BlockTransformation::BLOCKSIZE };

    CryptoLibCipher( const std::string &key )
        : CryptoLib_BlockTransformation( (unsigned char*&)key.begin(), key.length() )
    {}

    void ProcessBlock( const char *in, char *out )
    {
        CryptoLib_BlockTransformation::ProcessBlock( (unsigned char*&)in, (unsigned char*)out);
    }
};


/*! XOR cipher
 */
template< int BlockSize >
struct USELESS_API XorCipher
{
    enum { BLOCKSIZE = BlockSize };

    XorCipher( const std::string &key ): _key(key) {}

    void ProcessBlock( const char *in, char *out )
    {
        for ( int i=0; i!=BLOCKSIZE; ++i )
        {
            out[i] = in[i]^_key[ i%_key.size() ];
        }
    }

private:
    std::string _key;
};



};//namespace Useless
#endif//__INCLUDED_USELESS_BLOCK_CIPHER_H__
