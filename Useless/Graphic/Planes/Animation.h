#ifndef __INCLUDED_USELESS_ANIMATION_H__
#define __INCLUDED_USELESS_ANIMATION_H__

#include "Useless/GUI/Skins/ImageDicer.h"
#include "Useless/System/Timer.h"

namespace Useless {

    //! Animation is simple set of images.
    //  Use ImageDicer to dice source image into frames, and use it
    //  to create Animation.
    class USELESS_API Animation : public ImageBase
    {
    public:
        Animation( const ImageDicer frames);
        Animation( const Animation &anim, const Rect &rect);

        virtual SPointer<IGraphics>
        Copy            () const    { return new Animation(*this);}

        virtual SPointer<IGraphics>
        Copy            ( const Rect &rect) const
                                    { return new Animation(*this, rect);}

        virtual void
        BlitTo          ( int x, int y, OGraphics &output);

        virtual void
        MultiBlitTo     ( OGraphics &, const PointList &, const RectList &);

        virtual const Surface *
        GetSurface      ()  const;
    
        virtual void
        Cooperate       ( const Surface& surface );

        virtual const Rect&
        GetRect         ()  const;

        virtual float
        GetFrameRate    ()  const   { return _frameRate;}
        
        virtual bool
        IsSolid         ()  const;

        virtual bool
        HasColorKey     ()  const;

        virtual NormalPixel
        GetColorKey     ()  const;
        
        void
        SetFrameRate    ( double fps) { _frameRate = fps;}
        
        void
        Play            ();

        void
        Stop            ();
        
        void
        SetHoldFrame    ( double holdDuration, int frameNo = -1);
        
        void
        SetPingPong     ( bool t=true) { _pingPong=t; }
        
        void
        SetCascadeGroup ( int groupID) { _cascadeGroup=groupID; }
        
        
    private:
        SPointer<IGraphics> _source;
        std::vector< Rect > _frames;
        int     _currentFrame;
        Timer   _lastFrame;
        double  _frameRate;
        bool    _playOnBlit;
        bool    _isPlaying;
        int     _cascadeGroup;
        bool    _pingPong;
        double  _holdTime;
        int     _heldFrame;
        
    protected:
        virtual void
        Load            () {}
        
        void
        Update          ();
    };

    
};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,IGraphics,Animation);
REGISTER_COPIER(Useless,ImageBase,Animation);

#endif//__INCLUDED_USELESS_ANIMATION_H__
