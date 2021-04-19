/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE KooLAS C++ Template & Class Library     *
 * CALLED KooLib SOURCE CODE.                                       *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE KooLib SOURCE CODE IS (C) COPYRIGHT 2004                     *
 * by the Sadhbh Code (https://github.com/sadhbh-c0d3)              *
 *                                                                  *
 ********************************************************************
 ********************************************************************/

#ifndef __INCLUDED_KOOLIB_USELESS_CHUNKS_H__
#define __INCLUDED_KOOLIB_USELESS_CHUNKS_H__

#include <koolib/xml/XMLChunksProgram.h>
#include <koolib/xml/XMLChunksMethod.h>
#include <koolib/xml/XMLFactoryUtil.h>
#include <koolib/dynamic_cast_call.h>

#include "Useless/XML/Resources/CreateGUIResources.h"
#include "Useless/XML/Resources/CreateSounds.h"
#include "Useless/XML/Resources/CreateDatabase.h"
#include "Useless/XML/Resources/CreateFilesystems.h"
#include "Useless/GUI/Misc/Layout.h"
#ifdef USELESS_HAS_DSHOW
#	include "Useless/Movie/DShow/DSMedia.h"
#endif
#ifdef USELESS_HAS_VORBIS
#	include "Useless/Movie/Ogg/OggVorbisDecoder.h"
#endif
#include "Useless/Graphic/Device/SurfaceExtFun.h"


namespace XMLProgram {

    struct KOOLIB_BIND_API PointListProxy : XMLCodeBlock
    {
        Useless::PointList _points;
        PointListProxy( const Useless::PointList &points );
        ~PointListProxy();
        
        void AddPoint( const Useless::Pos &pos );
        void SetPoint( int index, const Useless::Pos &pos );
        int  Size();
        int Empty();
        Useless::Pos At( int index );
    };
    
    struct KOOLIB_BIND_API RectListProxy : XMLCodeBlock
    {
        Useless::RectList _rects;
        RectListProxy( const Useless::RectList &rectangles );
        ~RectListProxy();

        void AddRect( const Useless::Rect &rect );
        void SetRect( int index, const Useless::Rect &rect );
        int  Size();
        int Empty();
        Useless::Rect At( int index );

        IChunkPtr Intersect( const Useless::RectList &rectangles );
    };

    struct KOOLIB_BIND_API ScreenProxy : XMLCodeBlock
    {
        Useless::SPointer< Useless::Screen > _spScreen;

        ScreenProxy( Useless::SPointer< Useless::Screen > );
        ~ScreenProxy();

        IChunkPtr CreateImageBuffer( int width, int height );
	IChunkPtr CreateSubImage( const Useless::Rect &area );
        IChunkPtr CreatePainter();
        IChunkPtr GetClipper( const Useless::Rect &crop );
        void      WriteImage( std::string file );
    };

    struct KOOLIB_BIND_API ImageProxy : XMLCodeBlock
    {
        Useless::SPointer< Useless::ImageBase > _spImage;

        ImageProxy( Useless::SPointer< Useless::ImageBase > );
        ~ImageProxy();

        IChunkPtr QueryPixelColor( int x, int y );

        IChunkPtr CreatePainter();
	IChunkPtr CreateSubImage( const Useless::Rect &area );
        void      WriteImage( std::string file );
    };

    struct KOOLIB_BIND_API SampleProxy : XMLCodeBlock
    {
        Useless::SPointer< Useless::Sample > _spSample;

        SampleProxy( Useless::SPointer< Useless::Sample > );
        ~SampleProxy();

        float GetDuration();
    };

    struct KOOLIB_BIND_API ChannelProxy : XMLCodeBlock
    {
        Useless::Channel _channel;

        ChannelProxy( const Useless::Channel & );
        ~ChannelProxy();

        void Play();
        void LoopPlay();
        void Stop();
        void SetVolume( int volume );
        void SetPan( int pan );
        int  IsPlaying();
    };

#ifdef USELESS_HAS_DSHOW
    struct KOOLIB_BIND_API DSMediaProxy : XMLCodeBlock
    {
        Useless::SPointer< Useless::DSMedia > _spMedia;

        DSMediaProxy( Useless::SPointer< Useless::DSMedia > );
        ~DSMediaProxy();

        IChunkPtr GetImage();
        void SetVolume( int volume );
        bool IsFinished();
    };
#endif

#ifdef USELESS_HAS_VORBIS
    struct KOOLIB_BIND_API OggVorbisProxy : XMLCodeBlock
    {
        Useless::SPointer< Useless::OggVorbisDecoder > _spOggVorbis;
        Useless::SPointer< Useless::IFile > _file;

        OggVorbisProxy( Useless::SPointer< Useless::IFile >, Useless::SPointer< Useless::SoundCard > );
        ~OggVorbisProxy();

        IChunkPtr GetChannel();
    };
#endif

    struct KOOLIB_BIND_API AdvancedPaint
    {
        AdvancedPaint( Useless::Painter &painter ) {
            _pScreenSurface = painter.GetPlane()->GetSurface();
            _BlendFun = (Useless::SurfExt::PfBlendFun)(_pScreenSurface->GetExtFun("BlendFun"));
            _ConstColor = (Useless::SurfExt::PfConstColor)(_pScreenSurface->GetExtFun("ConstColor"));
            _ConstMatrix = (Useless::SurfExt::PfConstMatrix)(_pScreenSurface->GetExtFun("ConstMatrix"));
        }

        void BlendFun( int a, int b, bool e ) {
            if ( _BlendFun )
            { _BlendFun( _pScreenSurface, a, b, e); }
        }
        void ConstColor( int r, int g, int b ) {
            if ( _ConstColor )
            { _ConstColor( _pScreenSurface, r, g, b ); }
        }
        void ConstMatrix( float *m ) {
            if ( _ConstMatrix )
            { _ConstMatrix( _pScreenSurface, m ); }
        }

        Useless::Surface *               _pScreenSurface;
        Useless::SurfExt::PfBlendFun     _BlendFun;
        Useless::SurfExt::PfConstColor   _ConstColor;
        Useless::SurfExt::PfConstMatrix  _ConstMatrix;
    };


    struct KOOLIB_BIND_API PainterProxy : XMLCodeBlock
    {
        Useless::Painter   _painter;

        PainterProxy( const Useless::Painter & );
        ~PainterProxy();

        void FastBlit       ( Node, ExecutionState& );
        void FastMultiBlit  ( Node, ExecutionState& );
        void FastFillRect   ( Node, ExecutionState& );
        void FastDrawRect   ( Node, ExecutionState& );
        void FastDrawLine   ( Node, ExecutionState& );
        void FastDrawPolygon( Node, ExecutionState& );
        
        void Blit           ( Node, ExecutionState& );
        void MultiBlit      ( Node, ExecutionState& );
        void FillRect       ( Node, ExecutionState& );
        void DrawRect       ( Node, ExecutionState& );
        void DrawLine       ( Node, ExecutionState& );        
        void DrawPolygon    ( Node, ExecutionState& );
        
        void SubCanvasPaint ( Node, ExecutionState& );
       
        void Clear( int color );
        void Intersect( const Useless::Rect &crop );
        Useless::Pos GetOffset();
        IChunkPtr GetClipper( const Useless::Rect &crop );
    };
    
    struct KOOLIB_BIND_API WidgetPainterProxy : PainterProxy
    {
        Useless::WidgetPainter   _wpainter;

        WidgetPainterProxy( const Useless::WidgetPainter & );
        ~WidgetPainterProxy();

        void RepaintWidget  ();
    };

    struct KOOLIB_BIND_API WidgetProxy: XMLCodeBlock
    {
        Useless::Widget *_widget;

        WidgetProxy( Useless::Widget * );
        ~WidgetProxy();
    }; 
    
    struct KOOLIB_BIND_API ImageWidgetProxy : XMLCodeBlock
    {
        Useless::ImageWidget *_imgWidget;

        ImageWidgetProxy( Useless::ImageWidget *w );
        ~ImageWidgetProxy();

        void SetImageByName( std::string name );
        void SetSkinByName( std::string name );
    };

    struct KOOLIB_BIND_API LayoutProxy: XMLCodeBlock
    {
        Useless::LayoutBase *_layout;
        LayoutProxy( Useless::LayoutBase *layout );
        ~LayoutProxy();

        void SetWeight ( Useless::Widget *widget, int weight );
        void SetMaxSize( Useless::Widget *widget, int maxsize );
        void SetMinSize( Useless::Widget *widget, int minsize );
        void SetOverlap( Useless::Widget *widget, int overlap );

    };

    struct KOOLIB_BIND_API WorkspaceProxy: XMLCodeBlock
    {
        Useless::Workspace *_workspace;

        WorkspaceProxy( Useless::Workspace *workspace );
        ~WorkspaceProxy();
    
        void SetCursor( std::string name );
    };

    struct SkinProxy : XMLCodeBlock
    {
        Useless::VirtualValue< Useless::Skin > m_skin;
        SkinProxy( const Useless::Skin &skin );
    };
    
    struct FontProxy : XMLCodeBlock
    {
        Useless::Font m_font;
        FontProxy( const Useless::Font &font );
    };

    struct FontPainterProxy : XMLCodeBlock
    {
        Useless::FontPainter m_fontPaint;
        FontPainterProxy( const Useless::FontPainter &fp );
    };

    /*******************************************************

      Define convertions from raw C++ object to XML IChunk

    */


    inline IChunk *make_value_chunk( const Useless::Text &value )
    {
        return make_value_chunk( TextUtf8( value ));
    } 

    inline IChunk *make_value_chunk( const Useless::Pos &value )
    {
        XMLCodeBlock *block = new XMLCodeBlock();
        try {
            block->AddChunk(L"x", make_value_chunk( value.x ));
            block->AddChunk(L"y", make_value_chunk( value.y ));
            return block;
        }
        catch(...) { delete block; throw; }
    }

    inline IChunk *make_value_chunk( const Useless::Rect &value )
    {
        XMLCodeBlock *block = new XMLCodeBlock();
        try {
            block->AddChunk(L"x", make_value_chunk( value.x ));
            block->AddChunk(L"y", make_value_chunk( value.y ));
            block->AddChunk(L"w", make_value_chunk( value.w ));
            block->AddChunk(L"h", make_value_chunk( value.h ));
            return block;
        }
        catch(...) { delete block; throw; }
    }

    inline IChunk *make_value_chunk( const Useless::Painter &painter )
    {
        return new PainterProxy( painter );
    }

    inline IChunk *make_value_chunk( const Useless::WidgetPainter &painter )
    {
        return new WidgetPainterProxy( painter );
    }

    inline IChunk *make_value_chunk( Useless::Widget *widget )
    {
        return new WidgetProxy( widget );
    }

    inline IChunk *make_value_chunk( const Useless::Channel &channel )
    {
        return new ChannelProxy( channel );
    }

    inline IChunk *make_value_chunk( const Useless::Skin &skin )
    {
        return new SkinProxy( skin );
    }
    
    inline IChunk *make_value_chunk( const Useless::SPointer< Useless::Skin > &skin )
    {
        return new SkinProxy( *skin );
    }
    
    inline IChunk *make_value_chunk( const Useless::Font &font )
    {
        return new FontProxy( font );
    }
    
    KOOLIB_BIND_API IChunk *make_value_chunk( const Useless::StupidVector< int > &vect );
    KOOLIB_BIND_API IChunk *make_value_chunk( const Useless::NormalPixel &pixel );
    KOOLIB_BIND_API IChunk *make_value_chunk( const Useless::FontPainter::Carrige &car );
    KOOLIB_BIND_API IChunk *make_value_chunk( const Useless::TextInfo::Word &word );
    KOOLIB_BIND_API IChunk *make_value_chunk( const Useless::TextInfo::Row &row );
    KOOLIB_BIND_API IChunk *make_value_chunk( const Useless::FontPainter::Status &s );
    KOOLIB_BIND_API IChunk *make_value_chunk( const Useless::FontPainter::Lines &s );
    KOOLIB_BIND_API IChunk *make_value_chunk( const Useless::FontPainter::Words &s );


    /**********************************************************************************

      Define convertions from XML Node attribute or from XML IChunk to C++ object

    */

    KOOLIB_SPECIALIZATION struct argument_traits< const Useless::Pos & >
    {
        static Useless::Pos get( const TextAnsi &name, Node node, ExecutionState &state )
        {
            Useless::Pos result;
            XMLFactory::Attr< int, false > _x( name );
            if ( node >> _x )
            {
                result.x = _x++;
                result.y = *_x;
            }
            else
            {
                IChunkPtr pChunk = GetChunk( name, state );
                result.x = value_of< int >( pChunk->GetChunk(L"x"));
                result.y = value_of< int >( pChunk->GetChunk(L"y"));
            }
            return result;
        }
    };

    KOOLIB_SPECIALIZATION struct argument_traits< const Useless::Rect & >
    {
        static Useless::Rect get( const TextAnsi &name, Node node, ExecutionState &state )
        {
            Useless::Rect result;
            XMLFactory::Attr< int, false > _x( name );
            if ( node >> _x )
            {
                result.x = _x++;
                result.y = _x++;
                result.w = _x++;
                result.h = *_x;
            }
            else
            {
                IChunkPtr pChunk = GetChunk( name, state );
                result.x = value_of< int >( pChunk->GetChunk(L"x"));
                result.y = value_of< int >( pChunk->GetChunk(L"y"));
                result.w = value_of< int >( pChunk->GetChunk(L"w"));
                result.h = value_of< int >( pChunk->GetChunk(L"h"));
            }
            return result;
        }
    };
    
    KOOLIB_SPECIALIZATION struct argument_traits< const Useless::PointList & >
    {
        static Useless::PointList & get( const TextAnsi &name, Node node, ExecutionState &state )
        {
            IChunkPtr pChunk = GetChunk( name, state );
            PointListProxy *pPointList = dynamic_cast< PointListProxy * >( pChunk.get() );
            if ( 0 == pPointList )
            {
                throw Useless::Error("PointList expected");
            }
            return pPointList->_points;
        }
    };
    
    KOOLIB_SPECIALIZATION struct argument_traits< const Useless::RectList & >
    {
        static Useless::RectList & get( const TextAnsi &name, Node node, ExecutionState &state )
        {
            IChunkPtr pChunk = GetChunk( name, state );
            RectListProxy *pRectList = dynamic_cast< RectListProxy * >( pChunk.get() );
            if ( 0 == pRectList )
            {
                throw Useless::Error("RectList expected");
            }
            return pRectList->_rects;
        }
    };
    
    KOOLIB_SPECIALIZATION struct argument_traits< const Useless::Skin & >
    {
        static Useless::Skin & get( const TextAnsi &name, Node node, ExecutionState &state )
        {
            IChunkPtr pChunk = GetChunk( name, state );
            SkinProxy *pConst = dynamic_cast< SkinProxy * >( pChunk.get() );
            if ( 0 == pConst )
            {
                throw Useless::Error("SkinProxy expected");
            }
            return pConst->m_skin;
        }
    };
    
    KOOLIB_SPECIALIZATION struct argument_traits< const Useless::Font & >
    {
        static Useless::Font & get( const TextAnsi &name, Node node, ExecutionState &state )
        {
            IChunkPtr pChunk = GetChunk( name, state );
            FontProxy *pConst = dynamic_cast< FontProxy * >( pChunk.get() );
            if ( 0 == pConst )
            {
                throw Useless::Error("FontProxy expected");
            }
            return pConst->m_font;
        }
    };


    KOOLIB_SPECIALIZATION struct argument_traits< const Useless::Painter & >
    {
        static Useless::Painter & get( const TextAnsi &name, Node node, ExecutionState &state )
        {
            IChunkPtr pChunk = GetChunk( name, state );
            PainterProxy *pPainter = dynamic_cast< PainterProxy * >( pChunk.get() );
            if ( 0 == pPainter )
            {
                throw Useless::Error("Painter expected");
            }
            return pPainter->_painter;
        }
    };

    KOOLIB_SPECIALIZATION struct argument_traits< const Useless::WidgetPainter & >
    {
        static Useless::WidgetPainter & get( const TextAnsi &name, Node node, ExecutionState &state )
        {
            IChunkPtr pChunk = GetChunk( name, state );
            WidgetPainterProxy *pPainter = dynamic_cast< WidgetPainterProxy * >( pChunk.get() );
            if ( 0 == pPainter )
            {
                throw Useless::Error("Painter expected");
            }
            return pPainter->_wpainter;
        }
    };

    KOOLIB_SPECIALIZATION struct argument_traits< Useless::Widget * >
    {
        static Useless::Widget * get( const TextAnsi &name, Node node, ExecutionState &state )
        {
            IChunkPtr pChunk = GetChunk( name, state );
            WidgetProxy *pWidget = dynamic_cast< WidgetProxy * >( pChunk.get() );
            if ( 0 == pWidget )
            {
                throw Useless::Error("Widget expected");
            }
            return pWidget->_widget;
        }
    };

    /**********************************************************************************

      Signals     

    */

    template< class _T >
        void add_object_to_block( IBlock *block, const TextAnsi &name, const _T &x )
        {            
            block->AddChunk( name, result_traits< _T >::get( x ));
        }

    template< class _SignalParametrizer >
        struct KOOLIB_BIND_API UniversalSlot : SignalFIDChunk 
        {
            _SignalParametrizer     _sigParam;
            IChunkPtr               _pChunk;
            IFiniteStateMachine    *_fsm;

            UniversalSlot( const _SignalParametrizer &sigParam, IChunk *pChunk, ExecutionState &state )
                : _sigParam( sigParam ), _pChunk( pChunk ), _fsm( state._currentFSM )
                {
                    _fid = _sigParam._pSignal->TieVoid( this, &UniversalSlot::DoExecute );
                }

            ~UniversalSlot()
            {
            }

            void DoExecute()
            {            
                IBlockPtr block = CreateBlock();
                _sigParam.Apply( block );
                FSMStackGuard guard( _fsm );
                ExecutionState state = guard.NextState( block.get() );
                _pChunk->Execute( Node(), state );
            }

            bool Execute( Node __unused__1, ExecutionState &__unused__2 )
            {
                DoExecute();
                return true;
            }
        };

    struct UniversalSignalParametrizer0
    {
        typedef Useless::Signal SigType;
        SigType *_pSignal;
        std::vector< TextUtf8 > _params;
        UniversalSignalParametrizer0( SigType *pSignal, const std::vector< TextUtf8 > &params )
            : _pSignal(pSignal), _params(params) {}

        void Apply( IBlockPtr block )
        {
        }
    };

    template< class _T1 >
        struct UniversalSignalParametrizer1
        {
            typedef Useless::Signal_1< _T1 > SigType;
            SigType *_pSignal;
            std::vector< TextUtf8 > _params;
            UniversalSignalParametrizer1( SigType *pSignal, const std::vector< TextUtf8 > &params )
                : _pSignal(pSignal), _params(params) {}

            void Apply( IBlockPtr block )
            {            
                add_object_to_block( block.get(), (_params.size() > 0 ? _params[0] : L"first"), _pSignal->GetArg1() );
            }
        };

    template< class _T1, class _T2 >
        struct UniversalSignalParametrizer2
        {
            typedef Useless::Signal_2< _T1, _T2 > SigType;
            SigType *_pSignal;
            std::vector< TextUtf8 > _params;
            UniversalSignalParametrizer2( SigType *pSignal, const std::vector< TextUtf8 > &params )
                : _pSignal(pSignal), _params(params) {}

            void Apply( IBlockPtr block )
            {
                add_object_to_block( block.get(), (_params.size() > 0 ? _params[0] : L"first"), _pSignal->GetArg1() );
                add_object_to_block( block.get(), (_params.size() > 1 ? _params[1] : L"second"), _pSignal->GetArg2() );
            }
        };

    template< class _T1, class _T2, class _T3 >
        struct UniversalSignalParametrizer3
        {
            typedef Useless::Signal_3< _T1, _T2, _T3 > SigType;
            SigType *_pSignal;
            std::vector< TextUtf8 > _params;
            UniversalSignalParametrizer3( SigType *pSignal, const std::vector< TextUtf8 > &params )
                : _pSignal(pSignal), _params(params) {}

            void Apply( IBlockPtr block )
            {
                add_object_to_block( block.get(), (_params.size() > 0 ? _params[0] : L"first"), _pSignal->GetArg1() );
                add_object_to_block( block.get(), (_params.size() > 1 ? _params[1] : L"second"),_pSignal->GetArg2() );
                add_object_to_block( block.get(), (_params.size() > 2 ? _params[2] : L"third"), _pSignal->GetArg3() );
            }
        };

    template< class _SignalParametrizer >
        struct KOOLIB_BIND_API SignalChunk : XMLCodeBlock
        {
            typedef typename _SignalParametrizer::SigType SigType;
            SigType &_s;

            SignalChunk( SigType &s ): _s(s)
            {
                add_methods( this )
                    .def("Tie", &SignalChunk::Tie )
                    .def("Send", &SignalChunk::Send )
                    ;
            }

            void Tie( Node node, ExecutionState &state )
            {
                Attr< TextUtf8, false, wchar_t > _parameters(L"parameters");
                std::vector< TextUtf8 > params;
                if ( GetMultiAttr( _parameters, node, state ))
                {
                    for (; !!_parameters; ++_parameters )
                    {
                        params.push_back( *_parameters );
                    }
                }
                IChunk *pChunk = XMLProgram::GetChunk( L"callback", state );
                if ( !pChunk ) { throw Useless::Error("Signal:Tie 'callback' expected"); }
                state.SetResult( new UniversalSlot< _SignalParametrizer >( _SignalParametrizer( &_s, params ), pChunk, state ));
            }

            void Send( Node node, ExecutionState &state )
            {
                _s.Send();
            }
        };

    inline IChunk *make_signal_0_chunk( Useless::Signal &s )
    {
        return new SignalChunk< UniversalSignalParametrizer0 >( s );
    }

    template< class _T1 >
        IChunk *make_signal_1_chunk( Useless::Signal_1< _T1 > &s )
        {
            return new SignalChunk< UniversalSignalParametrizer1< _T1 > >( s );
        }

    template< class _T1, class _T2 >
        IChunk *make_signal_2_chunk( Useless::Signal_2< _T1, _T2 > &s )
        {
            return new SignalChunk< UniversalSignalParametrizer2< _T1, _T2 > >( s );
        }

    template< class _T1, class _T2, class _T3 >
        IChunk *make_signal_3_chunk( Useless::Signal_3< _T1, _T2, _T3 > &s )
        {
            return new SignalChunk< UniversalSignalParametrizer3< _T1, _T2, _T3 > >( s );
        }

};//XMLProgram

#endif//__INCLUDED_KOOLIB_USELESS_CHUNKS_H__

