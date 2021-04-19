#include "Useless/UselessConfig.h"
#include "Useless/File/IFile.h"
#include "Useless/File/OFile.h"
#include "Useless/File/IFileSystem.h"
#include <vector>

namespace Useless {

    USELESS_API SPointer< IFile >       CreateEncryptedIFile( SPointer< IFile > f, const std::string &k = std::string() );
    USELESS_API SPointer< OFile >       CreateEncryptedOFile( SPointer< OFile > f, const std::string &k = std::string() );
    USELESS_API SPointer< IFileSystem > OpenHatchery        ( const std::string &n, const std::string &b, const std::string &k = std::string() );
    USELESS_API int                     CalculateDigest     ( const void *d, size_t s );
    USELESS_API void                    AlignForEncryption  ( std::string &d, char c = '\n' );

    template< class _T >
        int CalculateDigest( const std::vector< _T > &d )
        {
            return CalculateDigest( &d[0], d.size() * sizeof( _T ));
        }
    
    template< class _T >
        int CalculateDigest( const std::basic_string< _T > &d )
        {
            return CalculateDigest( &d[0], d.size() * sizeof( _T ));
        }

};
