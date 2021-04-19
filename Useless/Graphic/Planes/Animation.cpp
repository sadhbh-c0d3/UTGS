#include "UselessPch.h"

#include "Animation.h"
#include <map>

namespace Useless {


Animation::Animation    ( const ImageDicer frames)
    :_currentFrame(0), _playOnBlit(true), _isPlaying(false),
     _cascadeGroup(-1),
     _pingPong(0),
     _holdTime(0.0),
     _heldFrame(0)
{
    _source = frames.GetImage( ImageDicer::SOURCE).Copy();

    ImageBase *p;
    C_verify( ( p = dynamic_cast<ImageBase*>(&*_source)) && "Animation::Animation");
    static_cast<ImageBase&>(*this) = *p;
    
    int numFrames = frames.Size()-1;
    for ( int i=0; i!=numFrames; ++i)
    {
        C_assert( frames.Exists(i));
        const IGraphics &igfx = frames.GetImage(i);
        _frames.push_back( igfx.GetRect());
    }
    
    _frameRate = numFrames;
}
        
Animation::Animation    ( const Animation &anim, const Rect &rect)
    :_currentFrame(0), ImageBase(anim),
     _cascadeGroup(anim._cascadeGroup),
     _pingPong(anim._pingPong),
     _holdTime(anim._holdTime),
     _heldFrame(anim._heldFrame)
{
    _source = anim._source;
    
    int numFrames = anim._frames.size();
    for ( int i=0; i!=numFrames; ++i)
    {
        const Rect fr = anim._frames[i];
        Rect rc = rect;
        rc += fr.GetPos();
        rc &= fr; 
        _frames.push_back( rc);
    }
    
    _frameRate = numFrames;
}


void
Animation::BlitTo       ( int x, int y, OGraphics &output)
{
    IGraphics::BlitTo( x, y, output);
    
    Update();
}

void
Animation::MultiBlitTo  ( OGraphics &output,
        const PointList &pts, const RectList &rcs)
{
    RectList offsetRects;
    Pos p = GetRect().GetPos();
    
    for ( int i=0, N=rcs.size(); i!=N; ++i)
    {
        offsetRects.push_back( rcs[i] + p);
    }
    
    _source->MultiBlitTo( output, pts, offsetRects);
    
    Update();
}

const Surface *
Animation::GetSurface   ()  const
{
    return _source->GetSurface();
}

void
Animation::Cooperate    ( const Surface& surface )
{
    _source->Cooperate( surface);
}

const Rect&
Animation::GetRect      ()  const
{
    C_assert( _currentFrame >= 0);
    C_assert( _currentFrame < _frames.size());
    
    return _frames[ _currentFrame];
}

bool
Animation::IsSolid      ()  const
{
    return _source->IsSolid();
}

bool
Animation::HasColorKey  ()  const
{
    return _source->HasColorKey();
}

NormalPixel
Animation::GetColorKey  ()  const
{
    return _source->GetColorKey();
}

void
Animation::Play         ()
{
    _playOnBlit = true;
    _currentFrame = 0;
}

void
Animation::Stop         ()
{
    _isPlaying = false;
    _playOnBlit= false;
}

void
Animation::SetHoldFrame ( double holdDuration, int frameNo)
{
    _holdTime = holdDuration;
    _heldFrame = (frameNo==-1)? _frames.size() : frameNo;
}

//Workaround to make some delays between animations played on same screen
double tempAnimationDelay = 0.0;
std::map< Animation*, double > tempAnimationMapping;

void
Animation::Update       ()
{
    if ( !_isPlaying && _playOnBlit)
    {
        if ( _cascadeGroup>=0)
        if ( tempAnimationMapping.find(this) == tempAnimationMapping.end())
        {
            tempAnimationMapping[this] = tempAnimationDelay;
            tempAnimationDelay += 0.1;
            if ( tempAnimationDelay > 0.8) { tempAnimationDelay-=0.8; }
        }
    
        _lastFrame = Timer();
        _isPlaying = true;
        _playOnBlit= false;
    }
    else
    {
        double animDelay = ( _cascadeGroup<0)? 0.0 : tempAnimationMapping[this];
        
        Timer t;
        int numFrames  = _frames.size();
        double fullTime = ((double)numFrames)/_frameRate;
        double interval = t-_lastFrame - animDelay;
        _currentFrame = (interval + fullTime)*_frameRate;
    
        int holdFrames = _holdTime * _frameRate;
        int fd = _currentFrame/(numFrames + holdFrames);
        _currentFrame %= numFrames + holdFrames;
        
        if ( _pingPong && ((fd%2)==0))
        {
            _currentFrame = numFrames + holdFrames - _currentFrame -1;
        }

        if (_currentFrame >= numFrames) { _currentFrame = numFrames-1;}
        
        _currentFrame += _heldFrame;
        _currentFrame %= numFrames;
    }
}

};//namespace Useless
