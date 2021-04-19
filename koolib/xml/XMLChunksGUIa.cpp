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

  function:
  last mod: $Id: XMLChunksGUIa.cpp,v 1.6 2005/03/18 17:48:36 koolas Exp $

 ********************************************************************/


#include "koolib/xml/XMLChunksGUI.h"
#include "koolib/xml/XMLChunksUseless.h"

#include "Useless/XML/Resources/CreateGUIResources.h"
#include "Useless/XML/Resources/SignalMap.h"
#include "Useless/Graphic/Device/SurfaceExtFun.h"
#include "Useless/Graphic/Planes/TransparentImage.h"

#include <GL/gl.h>

using namespace Useless;
using namespace XMLFactory;
using namespace XMLProgram;

namespace XMLProgram {

    // Define tag dictionaries
    namespace XML
    {
        struct GUI_GET_IMAGE {};
        struct GUI_CREATE_IMAGE {};
        struct GUI_GET_WIDGET {};
        struct GUI_EVENT {};
        struct GUI_CANVAS {};
    };



    struct SignalParametrizer_None
    {
        void Apply( IBlockPtr env )
        {
        }
    };
    
    struct SignalParametrizer_1Pos
    {
        typedef Signal_1< Pos > SigType;
        SigType *_pSignal;
        SignalParametrizer_1Pos( SigType *pSignal ): _pSignal(pSignal) {}

        void Apply( IBlockPtr env )
        {
            const Pos &p = _pSignal->GetArg1();
            env->AddChunk(L"x", make_value_chunk( p.x ));
            env->AddChunk(L"y", make_value_chunk( p.y ));
        }
    };

    struct SignalParametrizer_1WidgetPainter
    {
        typedef Signal_1< WidgetPainter > SigType;
        SigType *_pSignal;
        CompoundWidget *_pCompound;

        SignalParametrizer_1WidgetPainter( SigType *pSignal, CompoundWidget *pCompound )
            : _pSignal(pSignal), _pCompound(pCompound) {}

        void Apply( IBlockPtr env )
        {
            env->AddChunk(L"__painter__", new WidgetPainterProxy( _pSignal->GetArg1() ));
        }

    };


    template< class _SignalParametrizer > struct XMLSignalSlot2 : SignalFIDChunk, XMLCodeChunk
    {
        IFiniteStateMachine *_fsm;
        _SignalParametrizer  _parametrizer;

        XMLSignalSlot2( Signal &signal
                , Node node
                , IFiniteStateMachine *fsm
                , const _SignalParametrizer &para )
            : _parametrizer( para ), XMLCodeChunk( node ), _fsm( fsm )
        {
            _fid = signal.TieVoid( this, &XMLSignalSlot2::_Execute );
        }

        void _Execute()
        {
            try {
                FSMStackGuard guard( _fsm );
                IBlockPtr newBlock( new XMLCodeBlock() );
                ExecutionState newState( guard.NextState( newBlock.get() ));
                _parametrizer.Apply( newBlock );
                newState.ExecuteChildren( _subDoc.Root() );
            }
            catch ( Error &e ) {
                throw Error("Error while signal was fired:\n%s", e.GetError());
            }
        }
        
        bool Execute( Node node, ExecutionState &state )
        {        
            _Execute();
            return true;
        }
        
        bool operator >> ( XMLFactory::AttrUniBase &attr )
        {
            if ( attr._name == L"type-name" )
            {
                attr.str(L"extended-slot");
                return true;
            }
            return false;
        }
    };

};//XMLProgram

namespace XMLFactory {    

#define LOCAL_TAG_HANDLER( _name )  USELESS_XML_TAG_HANDLER( _name, ExecutionState )

    LOCAL_TAG_HANDLER( XML::GUI_GET_IMAGE )
    {
        Attr< TextAnsi, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        ImageResource res("images", TextAnsi( _id.str() ));
        _state.SetResult( new ImageProxy( *res ));
    }

    LOCAL_TAG_HANDLER( XML::GUI_CREATE_IMAGE )
    {
        Attr< std::string, true, char > _src("src");
        Attr< std::string, false, char > _alpha("alpha");
        GetAttr( _src, _node, _state );
        if ( !GetAttr( _alpha, _node, _state ))
        {
            _state.SetResult( new ImageProxy( new Image( _src.str().c_str() )));
        }
        else
        {
            _state.SetResult( new ImageProxy( new TransparentImage( _src.str().c_str(), _alpha.str().c_str() )));
        }
    }

    LOCAL_TAG_HANDLER( XML::GUI_GET_WIDGET )
    {
        Attr< TextAnsi, true, wchar_t > id(L"id");
        GetAttr( id, _node, _state );
        Widget *widget = WidgetResource< Widget >::Query( *id );
        _state.SetResult( new WidgetProxy( widget ));
    }
    
    LOCAL_TAG_HANDLER( XML::GUI_EVENT )
    {
        Attr< TextAnsi, true, wchar_t >   id(L"source");
        Attr< TextAnsi, true, wchar_t >   signal(L"signal");

        GetAttr( id, _node, _state );
        GetAttr( signal, _node, _state );
            
        Widget *widget = WidgetResource< Widget >::Query( *id );

        int pLB = signal.str().find(L"(");
        int pRB = signal.str().find(L")", pLB);
        
        if ( pLB == -1 || pRB == -1 )
        {
            Signal &signal1 = SignalMap::Instance().GetSignalByName( *signal, widget );

            // on signal chunk will be executed.
            XMLSignalSlot *chunk = new XMLSignalSlot( signal1, _node, _state );

            // add new chunk of code from node subtree.
            _state._currentBlock->AddChunk( id.str() +L'.'+ signal.str(), chunk );
        }

        else if ( pLB > 0 && (signal.str().size() - 1) == pRB )
        {
            TextAnsi sigName = signal.str().substr( 0, pLB );
            TextAnsi sigProto = signal.str().substr( pLB+1, pRB-pLB-1 );
            Attr< TextAnsi, true, wchar_t > arg(L""); arg.str( sigProto );
            std::vector< std::string > argNames;
            for (; !!arg; arg++ )
            {
                argNames.push_back( *arg );
            }

            if ( L"onpaint" == sigName )
            {
                typedef SignalParametrizer_1WidgetPainter::SigType PaintSignal;
                typedef XMLSignalSlot2< SignalParametrizer_1WidgetPainter > PaintSlot;

                ActiveAreaWidget *pWidget1 = dynamic_cast< ActiveAreaWidget *>( widget );
                CompoundWidget   *pWidget2 = dynamic_cast< CompoundWidget *>( widget );
                PaintSignal *pPaint;
                if ( pWidget1 ) { pPaint = &pWidget1->OnPaint; }
                else if ( pWidget2 ) { pPaint = &pWidget2->OnPaint; }
                else {
                    throw Error("signal 'onpaint' is only available for activearea or hub");
                }

                PaintSlot *slot = new PaintSlot( *pPaint, _node, _state._currentFSM,
                        SignalParametrizer_1WidgetPainter( pPaint, pWidget2 ));
            
                _state._currentBlock->AddChunk( id.str() +L'.'+ sigName, slot );
            }
            else
            {
                Signal *pSignal = &SignalMap::Instance().GetSignalByName( sigName, widget );
            
                typedef SignalParametrizer_1Pos::SigType PointerSignal;
                typedef XMLSignalSlot2< SignalParametrizer_1Pos > PointerSlot;
                if ( PointerSignal *pPointer = dynamic_cast< PointerSignal *>( pSignal ))
                {
                    PointerSlot *slot= new PointerSlot( *pSignal, _node, _state._currentFSM,
                            SignalParametrizer_1Pos( pPointer ));
                
                    _state._currentBlock->AddChunk( id.str() +L'.'+ sigName, slot );
                }
            }            

        }

        else
        {
            throw Error("Bad signal name or proto");
        }

    }
    
    LOCAL_TAG_HANDLER( XML::GUI_CANVAS )
    {
        Attr< int, false, wchar_t >     _alpha(L"alpha");
        Attr< Color, false, wchar_t >   _color(L"color");
        Attr< TextAnsi, false, wchar_t >    _blending(L"blending");
        Attr< int, false, wchar_t >     _crop(L"crop");
        Attr< int, false, wchar_t >     _offset(L"offset");

        Pos offset(0,0);
        if ( GetMultiAttr( _offset, _node, _state ))
        {
            offset.x = _offset++;
            offset.y = _offset++;
        }

        IChunk *pChunk = GetChunk(L"__painter__", _state );

        WidgetPainterProxy *pProxy = dynamic_cast< WidgetPainterProxy *>( pChunk );
        IBlockPtr newBlock = new XMLCodeBlock();
        ExecutionState state( _state );
        state._prevState = &_state;
        state._currentBlock = newBlock.get();
        WidgetPainterProxy *pProxy1 = new WidgetPainterProxy( WidgetPainter( pProxy->_wpainter, offset ));
        newBlock->AddChunk(L"__painter__", pProxy1 );

        if ( GetMultiAttr( _crop, _node, _state ))
        {
            Rect crop;
            crop.x = _crop++;
            crop.y = _crop++;
            crop.w = _crop++;
            crop.h = _crop++;
            pProxy1->_painter.Intersect( crop );
        }

        if ( GetAttr( _alpha, _node, _state ))
        {
            pProxy1->_painter.MultiplyAlpha( *_alpha );
        }

        AdvancedPaint advPaint( pProxy->_painter );
        int APtouched = 0;

        if ( GetAttr( _blending, _node, _state ))
        {
            APtouched |= 1;
            if ( L"normal" == *_blending )
            {
                advPaint.BlendFun( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, true );
            }
            else if ( L"replace" == *_blending )
            {
                advPaint.BlendFun( GL_ONE, GL_ZERO, true );
            }
            else if ( L"add" == *_blending ) // add RGBA
            {
                advPaint.BlendFun( GL_SRC_ALPHA, GL_ONE, true );
            }
            else if ( L"multiply" == *_blending ) // multiply RGBA
            {
                advPaint.BlendFun( GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA, true );
            }
            else if ( L"add-color" == *_blending ) // add RGB
            {
                advPaint.BlendFun( GL_ONE, GL_ONE_MINUS_SRC_ALPHA, true );
            }
            else if ( L"multiply-color" == *_blending ) // multiply RGB
            {
                advPaint.BlendFun( GL_DST_COLOR, GL_ZERO, true );
            }
            else if ( L"shadow" == *_blending )
            {
                advPaint.BlendFun( GL_ZERO, GL_ONE_MINUS_SRC_ALPHA, true );
            }
        }

        if ( GetAttr( _color, _node, _state ))
        {
            APtouched |= 2;
            NormalPixel color( *_color );
            advPaint.ConstColor( color.c.r, color.c.g, color.c.b );
        }

        state.ExecuteChildren( _node );

        if ( APtouched & 1 )
        {
            advPaint.BlendFun( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, true );
        }
        if ( APtouched & 2 )
        {
            advPaint.ConstColor( 255, 255, 255 );
        }
    }

}
