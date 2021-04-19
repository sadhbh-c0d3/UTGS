#include "UselessPch.h"

#ifndef __INCLUDE__USELESS_DSOUND_STRUCTS_H__
#define __INCLUDE__USELESS_DSOUND_STRUCTS_H__

#include "DSStructs.h"

namespace Useless{
namespace Snd {


DSBufferDesc::DSBufferDesc()
{
    ZeroMemory(&_desc,sizeof(_desc));
    ZeroMemory(&_fmt,sizeof(_fmt));
}
DSBufferDesc::DSBufferDesc( const DSBUFFERDESC &desc )
{
    _desc = desc;
    _fmt = *desc.lpwfxFormat;
    _desc.lpwfxFormat = &_fmt;
}

DSBufferDesc::DSBufferDesc( const Properties &props )
{
    ZeroMemory( &_fmt, sizeof(_fmt) );
    ZeroMemory( &_desc,sizeof(_desc));

    _fmt.wFormatTag = WAVE_FORMAT_PCM;
    _fmt.nChannels = props.channels;
    _fmt.nSamplesPerSec = props.sample_rate;
    _fmt.nAvgBytesPerSec = props.sample_rate * props.channels*(props.bits/8);
    _fmt.nBlockAlign = props.channels*(props.bits/8);
    _fmt.wBitsPerSample = props.bits;

    _desc.dwSize = sizeof(_desc);
    _desc.lpwfxFormat = &_fmt;
    _desc.dwBufferBytes = _fmt.nBlockAlign * props.length;
    
    _desc.guid3DAlgorithm = DS3DALG_DEFAULT;
    _desc.dwFlags = DSBCAPS_GLOBALFOCUS + DSBCAPS_CTRLPOSITIONNOTIFY;

    if (props.volume_ctl) _desc.dwFlags |= DSBCAPS_CTRLVOLUME;
    if (props.pan_ctl)    _desc.dwFlags |= DSBCAPS_CTRLPAN;
    if (props.pitch_ctl)  _desc.dwFlags |= DSBCAPS_CTRLFREQUENCY;
    if (props.sfx_ctl)    _desc.dwFlags |= DSBCAPS_CTRLFX;
    if (props.space_ctl)  _desc.dwFlags |= DSBCAPS_CTRL3D;
}

DSBufferDesc::operator Properties() const
{
    Properties props;

    props.length = _desc.dwBufferBytes / _fmt.nBlockAlign;
    props.sample_rate = _fmt.nSamplesPerSec;
    props.channels = (char)_fmt.nChannels;
    props.bits = (char)_fmt.wBitsPerSample;

    props.volume_ctl = false;
    props.pan_ctl = false;
    props.pitch_ctl= false;
    props.sfx_ctl = false;
    props.space_ctl = false;

    props.global_focus = false;
    props.primary = false;
    props.sndcard_memory = false;

    if (_desc.dwFlags & DSBCAPS_CTRLVOLUME)     props.volume_ctl = true;
    if (_desc.dwFlags & DSBCAPS_CTRLPAN)        props.pan_ctl    = true;
    if (_desc.dwFlags & DSBCAPS_CTRLFREQUENCY)  props.pitch_ctl  = true;
    if (_desc.dwFlags & DSBCAPS_CTRLFX)         props.sfx_ctl    = true;
    if (_desc.dwFlags & DSBCAPS_CTRL3D)         props.space_ctl  = true;

    return props;
}


};//namespace Snd
};//namespace Useless
#endif//__INCLUDE__USELESS_DSOUND_STRUCTS_H__
