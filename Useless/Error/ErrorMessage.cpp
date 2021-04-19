#include "UselessPch.h"

#include "ErrorMessage.h"
#include "Useless/Util/Singleton.h" // for RTTIWarnEnabledAttr
#include "Useless/System/Application.h" // for app_title
#include <set>
#include <string>
#include <fstream>


namespace Useless {

char Format<int>::x[3] = { '%', 'd', ' '};
char Format<float>::x[3] = { '%', 'f', ' '};
char Format<char *>::x[3] = { '%', 's', ' '};
char Format<void *>::x[3] = { '%', 'x', ' '};
char Format<const int>::x[3] = { '%', 'd', ' '};
char Format<const float>::x[3] = { '%', 'f', ' '};
char Format<const char *>::x[3] = { '%', 's', ' '};
char Format<const void *>::x[3] = { '%', 'x', ' '};


// RTTI warning table stuff
struct __RTTIWarnEnabledAttr
{
    __RTTIWarnEnabledAttr()
        {
            std::string fileName( Application::GetTitle() );
#   ifdef NDEBUG
            // AppTitle.err - enable error reports (Release)
            LoadWarnTable( fileName + ".err" );
#   else
            // AppTitle.erd - disable error reports (Debug)
            LoadWarnTable( fileName + ".erd" );
#   endif
        }
    
    void
    LoadWarnTable( const std::string &fileName )
        {
            std::ifstream f( fileName.c_str() );
            std::string s;
            while ( std::getline( f, s ))
            {
                if ( !s.empty() && s[0]!='#' )
                    { _table.insert( s ); }
            }
        }

    bool
    IsEnabled( const std::type_info &tp, int id )
    {
#   ifdef NDEBUG // _table enables reports (Release)
        return ( _table.find( tp.name() ) != _table.end() );
#   else         // _table disables reports (Debug)
        return ( _table.find( tp.name() ) == _table.end() );
#   endif
    }
    
    std::set< std::string > _table;
};

DECLARE_SINGLETON( __RTTIWarnEnabledAttr, Singleton_RTTIWarnEnabledAttr, USELESS_API);
INIT_SINGLETON( Singleton_RTTIWarnEnabledAttr, USELESS_API );

bool RTTIWarnEnabledAttr::IsEnabled( const std::type_info &tp, int id )
{
    return Singleton_RTTIWarnEnabledAttr::Instance().IsEnabled( tp, id );
}

};//namespace Useless
