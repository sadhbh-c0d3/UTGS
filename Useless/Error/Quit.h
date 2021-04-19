#ifndef __INLCUDED_USELESS_QUIT_H__
#define __INLCUDED_USELESS_QUIT_H__

namespace Useless {

// Quit may be thrown to suddenly exit application
class USELESS_API Quit
{
public:
    Quit( int level=0x0 ): _level(level) {}
    operator int() const { return _level; }
    int _level;
};

};//namespace Useless
#endif//__INLCUDED_USELESS_QUIT_H__
