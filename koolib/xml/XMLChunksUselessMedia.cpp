#include "XMLChunksUseless.h"

#include <koolib/xml/XMLKompiler.h> //used for faster symbol access
#include <koolib/xml/XMLChunksUseless.h>
#include <gl/GL.h> //@TODO: needed for PainterProxy::SubCanvasPaint
#include "Useless/Graphic/Planes/MemSurface.h" //needed by QueryPixelColor
#include "Useless/Graphic/Planes/ImageToFile.h" //needed by WriteImage

namespace XMLProgram {

    using namespace Useless;
    using namespace XMLFactory;
    using CXML::LazyGetChunkInScope;
    using CXML::LazyGetChunkMember;

    
    /*******************************

      ImageProxy

    */
    ImageProxy::ImageProxy( SPointer< ImageBase > spImage ): _spImage( spImage )
    {
        add_methods( _spImage.get(), this )
            .def("GetWidth", unconst_method_cast( &ImageBase::GetWidth ))
            .def("GetHeight", unconst_method_cast( &ImageBase::GetHeight ))
            .def("GetDestWidth", unconst_method_cast( &ImageBase::GetDestWidth ))
            .def("GetDestHeight", unconst_method_cast( &ImageBase::GetDestHeight ))
            .def("SetDestWidth", &ImageBase::SetDestWidth, "width" )
            .def("SetDestHeight", &ImageBase::SetDestHeight, "height" )
            ;

        add_methods( this )
            .def("QueryPixelColor", &ImageProxy::QueryPixelColor, "x", "y" )
            .def("CreateSubImage", &ImageProxy::CreateSubImage, "area" )
            .def("WriteImage", &ImageProxy::WriteImage, "file" )
            ;

        if ( dynamic_cast< OGraphics *>( spImage.get() ))
        {
            add_methods( this )
                .def("CreatePainter", &ImageProxy::CreatePainter )
                ;
        }
    }

    ImageProxy::~ImageProxy()
    {
    }

    IChunkPtr ImageProxy::CreatePainter()
    {
        OGraphics *pOutPlane = dynamic_cast< ImageBuffer *>( _spImage.get() );
        return new PainterProxy( Painter( *pOutPlane ));
    }
    
    IChunkPtr ImageProxy::CreateSubImage( const Rect &area )
    {
        if ( OGraphics *out = dynamic_cast< OGraphics *>( _spImage.get() ))
        {
            return new ImageProxy( SPointer< ImageBase >( out->QueryIGraphics( area ), dynamic_cast_tag ));
        }
        else
        {
            return new ImageProxy( SPointer< ImageBase >( _spImage->Copy( area ), dynamic_cast_tag ));
        }
    }
    
    void ImageProxy::WriteImage( std::string file )
    {
        ImageToFile::Store( *_spImage->GetSurface(), file );
    }
    
    IChunkPtr ImageProxy::QueryPixelColor( int x, int y )
    {
        const Surface *pSurface = _spImage->GetSurface();
        // Validate surface
        if ( 0 == dynamic_cast< const MemSurface *>( pSurface )) // - @TODO: CreateReader is not supported by GLTextureSurface - so we use MemSurface
        {
            _spImage->Cooperate( MemSurface() );
            pSurface = _spImage->GetSurface();
        }
        int red, green, blue, alpha;
        NormalPixel data;
        {   // Read COLOR
            SPointer< PixelTransfer > pixTrans = pSurface->CreateReader( Surface::COLOR );
            pixTrans->SetDestination( _spImage->GetWidth(), _spImage->GetHeight(), 1, 32 );
            pixTrans->Transform();
            pixTrans->Store( (unsigned char *)&data, Rect( x, y, 1, 1 ));
            red = data.c.r;
            green = data.c.g;
            blue = data.c.b;
		}
		{   // Read ALPHA
            SPointer< PixelTransfer > pixTrans = pSurface->CreateReader( Surface::ALPHA );
            pixTrans->SetDestination( _spImage->GetWidth(), _spImage->GetHeight(), 1, 8 );
            pixTrans->Transform();
            pixTrans->Store( &(data.c.a = 0x00), Rect( x, y, 1, 1 ));
            alpha = data.c.a;
        }
        IBlockPtr block = XMLProgram::CreateBlock();
        block->AddChunk(L"red", XMLProgram::CreateValue( red ));
        block->AddChunk(L"green", XMLProgram::CreateValue( green ));
        block->AddChunk(L"blue", XMLProgram::CreateValue( blue ));
        block->AddChunk(L"alpha", XMLProgram::CreateValue( alpha ));
        return block.get();
    }
    

    
    /*******************************

      SampleProxy

    */
    SampleProxy::SampleProxy( Useless::SPointer< Useless::Sample > spSample ): _spSample( spSample )
    {
        add_methods( this )
            .def("GetDuration", &SampleProxy::GetDuration )
            ;
    }

    SampleProxy::~SampleProxy()
    {
    }

    float SampleProxy::GetDuration()
    {
        Snd::Properties p;
        _spSample->GetProperties( p );
        return (p.length / (double)p.sample_rate);
    }
    
    
    
    /*******************************

      ChannelProxy

    */
    ChannelProxy::ChannelProxy( const Useless::Channel &channel ): _channel( channel )
    {
        add_methods( this )
            .def("Play", &ChannelProxy::Play)
            .def("LoopPlay", &ChannelProxy::LoopPlay)
            .def("Stop", &ChannelProxy::Stop)
            .def("SetVolume", &ChannelProxy::SetVolume, "volume")
            .def("SetPan", &ChannelProxy::SetPan, "pan")
            .def("IsPlaying", &ChannelProxy::IsPlaying )
            ;
    }

    ChannelProxy::~ChannelProxy()
    {
    }
        
    void ChannelProxy::Play()
    {
        _channel.Play( false );
    }

    void ChannelProxy::LoopPlay()
    {
        _channel.Play( true );
    }

    void ChannelProxy::Stop()
    {
        _channel.Stop();
    }

    void ChannelProxy::SetVolume( int volume )
    {
        _channel.SetVolume( volume );
    }

    void ChannelProxy::SetPan( int pan )
    {
        _channel.SetPan( pan );
    }

    int  ChannelProxy::IsPlaying()
    {
        return _channel.IsPlaying();
    }
    
#ifdef USELESS_HAS_DSHOW    
    /*******************************

      DSMediaProxy

    */

    DSMediaProxy::DSMediaProxy( Useless::SPointer< Useless::DSMedia > spMedia ): _spMedia( spMedia )
    {
        add_methods( _spMedia.get(), this )
            .def("Play", &DSMedia::Play )
            .def("Pause", &DSMedia::Pause )
            .def("Stop", &DSMedia::Stop )
            .def("IsPlaying", &DSMedia::IsPlaying )
            .def("IsPaused", &DSMedia::IsPaused )
            .def("IsVideo", unconst_method_cast( &DSMedia::IsVideo ))
            .def("IsAudio", unconst_method_cast( &DSMedia::IsAudio ))
            .def("GetTime", unconst_method_cast( &DSMedia::GetTime ))
            .def("GetDuration", unconst_method_cast( &DSMedia::GetDuration ))
            .def("SetTime", &DSMedia::SetTime, "time")
            .def("GetFPS", unconst_method_cast( &DSMedia::GetFPS ))
            ;

        add_methods( this )
            .def("IsFinished", &DSMediaProxy::IsFinished )
            .def("GetImage", &DSMediaProxy::GetImage )
            .def("SetVolume", &DSMediaProxy::SetVolume, "volume" )
            ;
    }

    DSMediaProxy::~DSMediaProxy()
    {
    }

    IChunkPtr DSMediaProxy::GetImage()
    {
        return new ImageProxy( new ImageBuffer( _spMedia->_surface ));
    }

    void DSMediaProxy::SetVolume( int volume )
    {
        Snd::PlayFX fx;
        fx.volume = volume;
        _spMedia->_sample_data->SetFX( fx );
    }

    bool DSMediaProxy::IsFinished()
    {
        return ( _spMedia->IsPlaying() && ( _spMedia->GetTime() >= _spMedia->GetDuration() - 0.01 ));
    }
#endif

#ifdef USELESS_HAS_VORBIS    
    /*******************************

      OggVorbisProxy

    */

    OggVorbisProxy::OggVorbisProxy( Useless::SPointer< Useless::IFile > file, Useless::SPointer< Useless::SoundCard > card )
    {
        _file = file;
        _spOggVorbis = new OggVorbisDecoder( *_file, *card );

        add_methods( _spOggVorbis.get(), this )
            .def("Play", &OggVorbisDecoder::Play )
            .def("Stop", &OggVorbisDecoder::Stop )
            .def("IsFinished", unconst_method_cast( &OggVorbisDecoder::EoS ))
            ;

        add_methods( this )
            .def("GetChannel", &OggVorbisProxy::GetChannel )
            ;
    }

    OggVorbisProxy::~OggVorbisProxy()
    {
    }
    
    IChunkPtr OggVorbisProxy::GetChannel()
    {
        return new ChannelProxy( _spOggVorbis->GetChannel() );
    }
#endif

    
};//XMLProgram

