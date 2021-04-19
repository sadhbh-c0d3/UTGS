#ifndef __INLCUDED__USELESS__DSOUND_STRUCTS_H__
#define __INLCUDED__USELESS__DSOUND_STRUCTS_H__

#include <dsound.h>
#include "Useless/Sound/Device/SoundStructs.h"

namespace Useless {

namespace Snd {

class USELESS_API DSBufferDesc
{
public:
    DSBufferDesc();
    DSBufferDesc( const DSBUFFERDESC &desc );
    DSBufferDesc( const Properties &props );
    operator Properties() const;

    const DSBUFFERDESC& GetDesc() const { return _desc; }
    const WAVEFORMATEX& GetFormat() const { return _fmt; }

private:
    DSBUFFERDESC _desc;
    WAVEFORMATEX _fmt;
};

};//namespace Snd
};//namespace Useless
#endif//__INLCUDED__USELESS__DSOUND_STRUCTS_H__
