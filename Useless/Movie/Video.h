#ifndef __INCLUDED_USELESS_VIDEO_H__
#define __INCLUDED_USELESS_VIDEO_H__

#include "Useless/UselessConfig.h"
#include "Useless/Graphic/Planes/ImageBuffer.h"

namespace Useless {

/*! Abstract for video playback rendered onto Surface
 */
class USELESS_API Video
{
public:
    virtual ~Video() {}

    virtual void  Play() = 0;
    virtual void  Stop() = 0;
    virtual bool  IsPlaying() const = 0;
    virtual bool  RenderFrame() = 0;

    virtual int  GetWidth () const = 0;
    virtual int  GetHeight() const = 0;

    virtual double  GetTime    () const = 0;
    virtual double  GetDuration() const = 0;
    virtual void    SetTime    (double) = 0;
    virtual double  GetFPS     () const = 0;
    
    virtual ImageBuffer GetVideoBuffer() const = 0;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_VIDEO_H__
