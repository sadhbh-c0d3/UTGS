#include "Useless/Util/LocaleIO.h"
#include "Useless/Util/Singleton.h"

namespace Useless {

struct USELESS_API __LangSelector
{
public:
    __LangSelector();    

    std::locale operator [] ( const std::string &localeISOName );

private:
    std::map< std::string, std::locale > _locales;
};

DECLARE_SINGLETON( __LangSelector, LangSelector, USELESS_API );

}; //Useless
