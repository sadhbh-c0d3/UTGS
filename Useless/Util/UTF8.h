#include "Useless/UselessConfig.h"

#include <string>

namespace Useless {

    USELESS_API void ReadUtf8( std::wstring &wstr, const std::string &strIn );
    USELESS_API void WriteUtf8( std::string &strOut, const std::wstring &wstr );
    
};
