#include "UselessPch.h"

#include "MoviePlayer.h"

#include "Useless/Graphic/Planes/MemSurface.h"


namespace Useless {

MoviePlayer::MoviePlayer(): _forceFinish(0)
{
    _bpp = CSurfaceRender::_prefered_bpp;
}
    
void
MoviePlayer::SetVideoBpp    ( int bpp)
{
    CSurfaceRender::_prefered_bpp = bpp;
}

void
MoviePlayer::CreateStream   ( const char *fileName, bool noSound )
{
    _media = new DSMedia( fileName, noSound ? DSMedia::VIDEO : (DSMedia::VIDEO + DSMedia::AUDIO) );
    _bpp = CSurfaceRender::_prefered_bpp;
}

void
MoviePlayer::DoPlay ()
{
    _media->Play();
    
    if ( _media->IsVideo())
    {
        C_assert( _bpp == CSurfaceRender::_prefered_bpp);
        
        _p_screen->SetClipper( Rect( 0,0,
                    _p_screen->GetWidth(),
                    _p_screen->GetHeight()));
        
#ifdef NDEBUG
        _p_screen->Clear(0x000000);
        _p_screen->Swap();
        _p_screen->Clear(0x000000);
        _p_screen->Swap();
#else
        _p_screen->Clear(0x00008F);
        _p_screen->Swap();
        _p_screen->Clear(0x00008F);
        _p_screen->Swap();
#endif
        double fps = _media->GetFPS();
        int sleep_ms = (int)(1000/fps);

        // Application::Step flags
        int flags = Application::DEQUEUE | Application::IDLE | Application::TIMER | Application::QUIT_EXCEPTION;
        int cursor = Application::GetCursorStatus();
        // hide system cursor
        if ( cursor == Application::CURSOR_VISIBLE ) { Application::HideCursor(); }

        while( (_media->GetTime() < _media->GetDuration()-0.05) &&
               !_forceFinish && Application::Step( flags ))
        {
            Timer t0;
           
            int w = _surface2->GetWidth();
            int h = _surface2->GetHeight();

            RenderFrame( w, h);

            double aspect = ((double)w)/(double(h));

            int scrW = _p_screen->GetWidth();
            int scrH = _p_screen->GetHeight();

            Surf::BlitFX blitFX;
            blitFX.stretch = true;

            int dstW = scrH * aspect;
            int dstH = scrW / aspect;
            int x=0;
            int y=0;

            if ( dstH < scrH )
            {
                blitFX.width  = scrW;
                blitFX.height = dstH;
                y = (scrH - dstH)/2;
            }            
            else
            {
                blitFX.width  = dstW;
                blitFX.height = scrH;
                x = (scrW - dstW)/2;
            }

            _p_screen->GetSurface()->Blit(x,y, *_surface2, Rect( w, h), blitFX);
            _p_screen->Swap();
            
            Timer t1;
            double dt = 1000*(t1-t0);
            int remain = (sleep_ms-dt>0)? sleep_ms-dt : 0;
            Sleep( remain);
        }
        
        // restore system cursor
        if ( cursor == Application::CURSOR_VISIBLE ) { Application::ShowCursor(); }
    }
    _media->Stop();
    _forceFinish = false;
}

bool
MoviePlayer::SingleUpdate   ()
{
    int w = _surface2->GetWidth();
    int h = _surface2->GetHeight();

    RenderFrame( w, h);
    return HasFinished();
}

bool
MoviePlayer::HasFinished    () const
{
    return ( _media->GetTime() < _media->GetDuration()-0.05);
}

void
MoviePlayer::Cooperate      ( Screen &screen, bool useBuffer)
{
    if ( _media->IsVideo())
    {
        _p_screen = &screen;
        
        if ( useBuffer)
        {
            MemSurface ref;
            _media->Cooperate( ref);

            _surface1 = _media->_surface;

            Surf::Properties prop;
            prop.width = _surface1->GetWidth();
            prop.height= _surface1->GetHeight();

            Surface *creator = _p_screen->GetSurface();

            _surface2.SetTarget( creator->CreateSurface( prop));
        }
        else
        {
            _media->Cooperate(*_p_screen->GetSurface());
            _surface2 = _media->_surface;
            _surface1.SetTarget(0);
        }
    }
}

void
MoviePlayer::Cooperate      ( SoundCard &card)
{
    if ( _media->IsAudio())
    {
        _media->Cooperate( card);
    }
}

void
MoviePlayer::SetVolume      ( int decibels)
{
    if ( _media->IsAudio() )
    {
        Snd::PlayFX playFx;
        playFx.volume = decibels;
        
        _media->_sample_data->SetFX( playFx);
    }
}


void
MoviePlayer::RenderFrame    ( int w, int h)
{
    if ( !!_surface1)
    {
        Surf::Ptr srcPtr;
        LockableSurface &src = static_cast<LockableSurface&>(*_surface1);

        src.Lock( Surface::COLOR, &srcPtr);
        SPointer<PixelTransfer> pixTr = _surface2->CreateWriter(
                Surface::COLOR);
        pixTr->SetSourcePointer( srcPtr.ptr);

        ImageFormat pixFormat = srcPtr.format;
        if ( pixFormat.Bits()==32)
        {
            pixFormat = ImageFormat::B8G8R8A0;
        }
        pixTr->SetSource( w, h, srcPtr.pitch, pixFormat);
        pixTr->Transform();
        src.Unlock( Surface::COLOR);
    }
}

};//namespace Useless
