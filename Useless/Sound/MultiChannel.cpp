#include "UselessPch.h"


#include "MultiChannel.h"

namespace Useless {

MultiChannel::~MultiChannel()
{}

void MultiChannel::CleanUp()
{
    ChannelMap::iterator i,j = _channels.begin();

    while( j!=_channels.end() )
    {
        i=j; ++j;

        if ( !(*i).IsPlaying() ) 
        {
            _channels.erase(i);
        }
    }
}

Channel& MultiChannel::Play( const Channel &channel )
{
    CleanUp();
    _channels.push_back( channel );
    return ( *_channels.rbegin() );
}

};//namespace Useless
