#ifndef __INCLUDED_USELESS_MOVIE_PLAYER_H__
#define __INCLUDED_USELESS_MOVIE_PLAYER_H__

#include "Useless/UselessConfig.h"
#include "Useless/Movie/DShow/DSMedia.h"
#include "Useless/Graphic/Device/Screen.h"

namespace Useless {

    /*! Creates DSMedia object, setup additional buffer (for Renderware),
     *  plays until it finishes, or renders single frame.
     */
    class USELESS_API MoviePlayer
    {
    public:
        MoviePlayer();
       
        //! Set video playback bits per pixel (use before all other methods)
        void
        SetVideoBpp     ( int bpp);

        //! Prepare stream for playback
        void
        CreateStream    ( const char *fileName, bool noSound = false );

        //! Release stream resources
        void
        ReleaseStream   () { _media = 0; _surface1.SetTarget(0); _surface2.SetTarget(0); }

        //! Do playback (wait until it ends)
        void
        DoPlay          ();
        
        //! Make single step of playback loop
        bool
        SingleUpdate    ();

        //! Finish playback
        void
        Finish          ()  { _forceFinish=true; }
        
        bool
        HasFinished     () const;

        //! Setup video output
        void
        Cooperate       ( Screen &screen, bool useBuffer = false);
        
        //! Setup audio output
        void
        Cooperate       ( SoundCard &card);

        //! Set audio volume
        void
        SetVolume       ( int decibels);
        
        Signal::FuncID
        TieFinish       ( Signal &s)
                        { return s.TieVoid( this, &MoviePlayer::Finish); }
        
        DSMedia* operator->() { return &*_media;}
        
        const Share<Surface> &
        GetSurface      ()  { return _surface2;}
        
    private:
        SPointer<DSMedia> _media;
        Share<Surface>    _surface1;
        Share<Surface>    _surface2;
        bool              _forceFinish;
        int               _bpp;
        Screen           *_p_screen;
        
        void
        RenderFrame     ( int w, int h);
    };

    
};//namespace Useless
#endif//__INCLUDED_USELESS_MOVIE_PLAYER_H__
