#ifndef __INCLUDED__USELESS_MULTI_CHANNEL_H__
#define __INCLUDED__USELESS_MULTI_CHANNEL_H__

#include "Useless/Sound/Channel.h"
#include <list>

namespace Useless {

class USELESS_API MultiChannel
{
public:
    ~MultiChannel();

    Channel& Play( const Channel &channel );

    void CleanUp();

private:
    typedef std::list< Channel > ChannelMap;
    ChannelMap _channels;
};


};//namespace Useless
#endif//__INCLUDED__USELESS_MULTI_CHANNEL_H__
