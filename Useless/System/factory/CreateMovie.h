#ifndef __INCLUDED__USELESS_CREATE_MEDIA_H__
#define __INCLUDED__USELESS_CREATE_MEDIA_H__

#include "Useless/System/factory/CreateBase.h"
#include "Useless/Movie/Audio.h"
#include "Useless/Movie/Video.h"

namespace Useless {

extern Audio * CreateAudio( const char *file_name);
extern Video * CreateVideo( const char *file_name);

template<> class Creator< Audio >
{
public:
    typedef Audio type;

    static Audio* Create( const char *f )
    {
        return CreateAudio(f);
    }
};

template<> class Creator< Video >
{
public:
    typedef Video type;

    static Video* Create( const char *f )
    {
        return CreateVideo(f);
    }
};


};//namespace Useless

#endif//__INCLUDED__USELESS_CREATE_MEDIA_H__
