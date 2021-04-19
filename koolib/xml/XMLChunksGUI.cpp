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
  last mod: $Id: XMLChunksGUI.cpp,v 1.13 2005/03/05 20:50:58 koolas Exp $

 ********************************************************************/


#include "koolib/xml/XMLChunksGUI.h"
#include "koolib/xml/XMLChunksProgram.h"
#include "koolib/xml/XMLChunksMethod.h"
#include "koolib/xml/XMLFactoryUtil.h"
#include "koolib/dynamic_cast_call.h"

#include "Useless/XML/Resources/CreateGUIResources.h"
#include "Useless/XML/Resources/SignalMap.h"

using namespace Useless;
using namespace XMLFactory;
using namespace XMLProgram;


namespace XMLProgram {

    // Define tag dictionaries
    namespace XML
    {
        struct GUI_EVENT {};
        struct GUI_LOAD_FRAME {};
        struct GUI_SET_VISIBLE {};
        struct GUI_SET_ENABLED {};
        struct GUI_SET_TEXT {};
        struct GUI_SET_IMAGE {};
        struct GUI_SET_POSITION {};
        struct GUI_SET_WIDTH {};
        struct GUI_SET_HEIGHT {};
        struct GUI_OPEN_DIALOG {};
        struct GUI_FREE_IMAGE {};
        struct GUI_FREE_SKIN {};
        struct GUI_GET_WIDGET {};
        struct GUI_GET_IMAGE {};
        struct GUI_CREATE_IMAGE {};
        struct GUI_GET_TEXT {};
        struct GUI_EVALUATE_TEXT {};
        struct GUI_CANVAS {};
    };
   
    USELESS_XML_TAG_DICT_DEFINE( xtdGUI, ExecutionState );
 
    // Tags defining GUI behavior
    static USELESS_XML_TAG_DICT_Extend( xtdGUI, ExecutionState, BasicProgram,
            .AddReg< XML::GUI_EVENT      >("gui-event")
            .AddReg< XML::GUI_LOAD_FRAME >("gui-load-frame")
            .AddReg< XML::GUI_SET_VISIBLE>("gui-set-visible")
            .AddReg< XML::GUI_SET_ENABLED>("gui-set-enabled")
            .AddReg< XML::GUI_SET_TEXT   >("gui-set-text")
            .AddReg< XML::GUI_SET_IMAGE  >("gui-set-image")
            .AddReg< XML::GUI_SET_POSITION >("gui-set-position")
            .AddReg< XML::GUI_SET_WIDTH  >("gui-set-width")
            .AddReg< XML::GUI_SET_HEIGHT >("gui-set-height")
            .AddReg< XML::GUI_OPEN_DIALOG>("gui-open-dialog")
            .AddReg< XML::GUI_FREE_IMAGE >("gui-free-image")
            .AddReg< XML::GUI_FREE_SKIN  >("gui-free-skin")
            .AddReg< XML::GUI_GET_WIDGET >("gui-get-widget")
            .AddReg< XML::GUI_GET_IMAGE  >("gui-get-image")
            .AddReg< XML::GUI_CREATE_IMAGE >("gui-create-image")
            .AddReg< XML::GUI_GET_TEXT   >("gui-get-text")
            .AddReg< XML::GUI_EVALUATE_TEXT >("gui-evaluate-text")
            .AddReg< XML::GUI_CANVAS     >("gui-canvas")
            );
 
    
    // Extend basic programm dict with gui stuff
    static USELESS_XML_TAG_DICT_AddBase( xtdProgram, ExecutionState, xtdGUI );
    
};//XMLProgram

namespace XMLFactory {

    using namespace XMLProgram;
    using namespace Useless;

#define LOCAL_TAG_HANDLER( _name )  USELESS_XML_TAG_HANDLER( _name, ExecutionState )


    LOCAL_TAG_HANDLER( XML::GUI_LOAD_FRAME )
    {
        Attr< std::string >     id("target");
        Attr< std::string >     xref("xref");
        Attr< int, false >      pos("pos");

        GetAttr( id, _node, _state );
        GetAttr( xref, _node, _state );

        CompoundWidget *parent = WidgetResource< CompoundWidget >::Query( *id );
        Widget         *widget = WidgetResource< Widget >::Query( *xref );


        if ( BridgeWidget * parentBridge = dynamic_cast< BridgeWidget *>( parent ))
        {
            static Useless::HubWidget dummyContainer;

            dynamic_cast_call< HubWidget >( widget->GetParent(), &HubWidget::Retrieve, widget );
            Widget *prev = parentBridge->DetachWidget();

            dummyContainer.Insert( prev );
            dummyContainer.Retrieve( widget );

            parentBridge->AttachWidget( widget );
        }
        
        else if ( HubWidget * parentHub = dynamic_cast< HubWidget *>( parent ))
        {
            for ( Widget::RefIter itChild = parentHub->FirstChild();
                    !!itChild; ++itChild )
            {
                if ( itChild->IsVisible() )
                {
                    itChild->Hide();
                }
            }
        }

        widget->Show();

        if ( GetAttr( pos, _node, _state ) )
        {
            Pos position;
            position.x = pos++;
            position.y = *pos;
            widget->SetPosition( position );
        }
    }
    
    LOCAL_TAG_HANDLER( XML::GUI_SET_VISIBLE )
    {
        Attr< std::string > target("target");
        Attr< int, false >  visible("visible");

        GetAttr( target, _node, _state );
        if (! GetAttr( visible, _node, _state ) ) { visible = true; }

        Widget *widget = WidgetResource< Widget >::Query( *target );

        if ( *visible )
        {
            widget->Show();
        }
        else
        {
            widget->Hide();
        }
    }

    LOCAL_TAG_HANDLER( XML::GUI_SET_ENABLED )
    {
        Attr< std::string > target("target");
        Attr< int, false >  enabled("enabled");

        GetAttr( target, _node, _state );
        if ( !GetAttr( enabled, _node, _state )) { enabled = true; }

        ActiveWidget *widget = WidgetResource< ActiveWidget >::Query( *target );
        widget->SetActive( *enabled );
    }

    LOCAL_TAG_HANDLER( XML::GUI_SET_TEXT )
    {
        Attr< std::string > target("target");
        
        GetAttr( target, _node, _state );
        Widget *widget = WidgetResource< Widget >::Query( *target );

        TextUtf8 text = EvaluateTextUtf8( _node, _state );
        text = MakeText( text );
        text = EvaluateString( text, _state );

        dynamic_cast_call< CaptionWidget >( widget, &CaptionWidget::SetCaption, text ) ||
            dynamic_cast_call< BaseTextWidget >( widget, &BaseTextWidget::SetText, text ) ||
            dynamic_cast_call< AdvTextBase >( widget, &AdvTextBase::SetText, text ) ||
            dynamic_cast_call< TextField >( widget, &TextField::SetText, text );
    }

    LOCAL_TAG_HANDLER( XML::GUI_SET_IMAGE )
    {
        Attr< std::string > target("target");
        Attr< std::string > img("img");
        
        GetAttr( target, _node, _state );
        GetAttr( img, _node, _state );
        
        ImageWidget *widget = WidgetResource< ImageWidget >::Query( *target );

        Resource *r = Resources::Instance().QueryResource("skins", *img );
        if ( 0 == r )
        {
            widget->SetSkin( *MakeImage( *img ));
        }
        else
        {
            widget->SetSkin( **SkinResource( r ));
        }
    }

    LOCAL_TAG_HANDLER( XML::GUI_SET_POSITION )
    {
        Attr< std::string > target("target");
        Attr< int >         x("x");
        Attr< int >         y("y");

        GetAttr( target, _node, _state );
        GetAttr( x, _node, _state );
        GetAttr( y, _node, _state );

        Widget *widget = WidgetResource< Widget >::Query( *target );
        widget->SetPosition( Pos( *x, *y ));
    }

    LOCAL_TAG_HANDLER( XML::GUI_SET_WIDTH )
    {
        Attr< std::string > target("target");
        Attr< int >         width("width");

        GetAttr( target, _node, _state );
        GetAttr( width, _node, _state );

        Widget *widget = WidgetResource< Widget >::Query( *target );
        widget->Resize( *width, widget->GetHeight() );
    }
    
    LOCAL_TAG_HANDLER( XML::GUI_SET_HEIGHT )
    {
        Attr< std::string > target("target");
        Attr< int >         height("height");

        GetAttr( target, _node, _state );
        GetAttr( height, _node, _state );

        Widget *widget = WidgetResource< Widget >::Query( *target );
        widget->Resize( widget->GetWidth(), *height );
    }

    // LOCAL_TAG_HANDLER( XML::GUI_GET_WIDGET ) moved to separate file

    LOCAL_TAG_HANDLER( XML::GUI_GET_TEXT )
    {
        Attr< std::string > id("id");

        GetAttr( id, _node, _state );

        TextUtf8 result = MakeText( *id );

        _state.SetResult( make_value_chunk( result ));
    }

    LOCAL_TAG_HANDLER( XML::GUI_EVALUATE_TEXT )
    {
        Attr< std::string > id("id");

        GetAttr( id, _node, _state );

        TextUtf8 toEval = MakeText( id.str() );
        TextUtf8 result = EvaluateString( toEval, _state );

        _state.SetResult( make_value_chunk( result ));
    }


    struct GuiOpenDialog_Leave : IChunk
    {
        Popup               *_popup;
        IFiniteStateMachine *_fsm;
        ExecutionState       _state;
        IChunkPtr            _postCall;
        bool _leaveState;
        bool _closeDialog;

        GuiOpenDialog_Leave( Popup *popup, ExecutionState &state )
            : _popup( popup )
            , _fsm( state._currentFSM )
            , _state( state )
            , _leaveState( true )
            , _closeDialog( true )
        {
        }

        // OnLeave close dialog if not closed.
        virtual bool Execute( XMLFactory::Node node, ExecutionState &state )
        {
            if ( _popup->IsVisible() && _closeDialog )
            {
                _leaveState = false;
                _popup->Hide();
            }

            return true;
        }
        
        // LeaveState if dialog was closed explicitly.
        void OnVisibility( bool visible )
        {
            IBlockPtr guard( _state._currentBlock );
            if ( !visible && _leaveState )
            {
                assert( _fsm->GetCurrentState() == L"gui-open-dialog" );
                assert( _fsm->GetCurrentExecutionState()._currentBlock == _state._currentBlock );
                _closeDialog = false;
                _fsm->LeaveState();
            }
            if ( !visible  && !!_postCall )
            {
                //@TODO: empty Node
                _postCall->Execute( Node(), _state );
            }
        }

        // Slots were not executed, hold block.
        void OnSignalBegin()
        {
            _state._currentBlock->AddRef();
        }

        // All slots have finished their execution.
        void OnSignalFinish()
        {
            _state._currentBlock->RemRef();
        }
    };

    LOCAL_TAG_HANDLER( XML::GUI_OPEN_DIALOG )
    {
        Attr< std::string >        id("popup");
        Attr< std::string, false > close("close");
        Attr< std::string, false > import("import");

        GetAttr( id, _node, _state );

        ExecutionState newState = _state._currentFSM->EnterState(L"gui-open-dialog");

        IBlockPtr importBlock = new XMLCodeBlock;

        if ( GetAttr( import, _node, _state ))
        {
            for ( ; !!import; import++ )
            {
                std::string name = *import;
                IChunk *pChunk = XMLProgram::GetChunk( name, _state );
                if ( pChunk )
                {
                    importBlock->AddChunk( name, pChunk );
                    newState._currentBlock->AddChunk( name, pChunk );
                }
                else
                {
                    throw Error("GuiOpenDialog: Import failed, no such symbol %s.", name.c_str());
                }
            }
        }

        Popup        *popup = WidgetResource< Popup >::Query( *id );
        
        GuiOpenDialog_Leave *guiOpenDialogLeave = new GuiOpenDialog_Leave( popup, newState );

        typedef std::map< std::string, ActiveWidget * > ButtonMap;
        ButtonMap choiceButtons;
        
        
        // Make sure block is held before signal handlers.
        for( Node node( _node.GetFirstChild() ); !!node; ++node )
        {
            if ( node->_name == "case" )
            {
                XMLFactory::Attr< std::string > caseName("id");
                node >> caseName;

                try {
                    ActiveWidget *button = WidgetResource< ActiveWidget >::Query( *id + "." + *caseName );
                    newState._currentBlock->AddChunk( "CloseDialogFinish." + (*caseName)
                            , new SignalFIDChunk(
                                Tie2Signal( button->OnClick[0]
                                    , guiOpenDialogLeave
                                    , &GuiOpenDialog_Leave::OnSignalBegin ) ));
                }
                catch( Useless::Error &e )
                {
                    throw Useless::Error("GuiOpenDialog: No such button %s in dialog. (%s)"
                            , (*caseName).c_str(), e.GetError() );
                }
            }
        }

        // Bind all closing buttons.
        if ( GetAttr( close, _node, _state ) )
            for ( ; !!close; close++ )
        {
            ActiveWidget *button = WidgetResource< ActiveWidget >::Query( *id + "." + *close ); 
            newState._currentBlock->AddChunk( "CloseDialog." + (*close)
                    , new SignalFIDChunk( button->OnClick[0].TieVoid( popup, &Popup::Hide ) ));

        }

        // Setup dialog-text, dialog-title and bind all cases.
        for( Node node( _node.GetFirstChild() ); !!node; ++node )
        {
            if ( node->_name == "dialog-text" )
            {
                Widget *inner = WidgetResource< Widget >::Query( *id +".dialog-text" );
        
                TextUtf8 text = EvaluateTextUtf8( node, _state );
                text = MakeText( text );
                text = EvaluateString( text, _state );

                dynamic_cast_call< BaseTextWidget >( inner, &BaseTextWidget::SetText, text ) ||
                    dynamic_cast_call< AdvTextBase >( inner, &AdvTextBase::SetText, text );
            }
            else if ( node->_name == "dialog-title" )
            {
                TextUtf8 text = EvaluateTextUtf8( node, _state );
                text = MakeText( text );
                text = EvaluateString( text, _state );

                popup->SetTitle( text );
            }
            else if ( node->_name == "on-close-dialog" )
            {
                guiOpenDialogLeave->_postCall = new XMLCodeChunk( node );
            }
            else
            {
                if ( node->_name == "case" )
                {
                    XMLFactory::Attr< std::string > caseName("id");
                    node >> caseName;

                    ActiveWidget *button = WidgetResource< ActiveWidget >::Query( *id + "." + *caseName );
                    newState._currentBlock->AddChunk( "onclick." + (*caseName)
                            , (new XMLSignalSlot( button->OnClick[0], node, newState ))->SetBlock( importBlock.get() ));
                }
                else
                {
                    newState.Execute( node );
                }
            }
        }
        
        // Make sure block is released after signal handlers.
        for( Node node( _node.GetFirstChild() ); !!node; ++node )
        {
            if ( node->_name == "case" )
            {
                XMLFactory::Attr< std::string > caseName("id");
                node >> caseName;

                ActiveWidget *button = WidgetResource< ActiveWidget >::Query( *id + "." + *caseName );
                newState._currentBlock->AddChunk( L"CloseDialogFinish." + TextAnsi( caseName.str() )
                        , new SignalFIDChunk(
                            Tie2Signal( button->OnClick[0]
                                , guiOpenDialogLeave
                                , &GuiOpenDialog_Leave::OnSignalFinish ) ));
            }
        }
       
        // OnLeave call GuiOpenDialog_Leave::Execute()
        newState._currentBlock->AddChunk( L"OnLeave", guiOpenDialogLeave );

        // Attach to Popup::Hide() event.
        newState._currentBlock->AddChunk( L"OnClose"
                , new SignalFIDChunk(
                    Tie2Signal( popup->OnVisible
                        , guiOpenDialogLeave
                        , &GuiOpenDialog_Leave::OnVisibility ) ));


        // Show dialog :-)
        popup->Show();
    }


    LOCAL_TAG_HANDLER( XML::GUI_FREE_IMAGE )
    {
        Attr< std::string > img("img");
        GetAttr( img, _node, _state );

        __Resources::ResourceMap::iterator it = Resources::Instance().Find("images", *img);
        while ( it != Resources::Instance().End() && (*it).first.first == *img )
        {
            ImageResource imgRes( (*it).second );
            (*imgRes)->GetSharedSurface().SetTarget(0);
        }
    }
    
    LOCAL_TAG_HANDLER( XML::GUI_FREE_SKIN )
    {
        Attr< std::string > skin("skin");
        GetAttr( skin, _node, _state );

        SkinManResource sset("skins", *skin);
        
        if ( SkinSet *s = dynamic_cast< SkinSet *>( sset->get() ))
        {
            for ( int i=0, c=0; c < s->Size(); ++i )
            {
                if ( s->Exists(i) )
                {
                    Skin *sk = &s->GetSkin(i);
                    if ( ImageSkin *ims = dynamic_cast< ImageSkin *>( sk ))
                    {
                        ims->GetImageBase().GetSharedSurface().SetTarget(0);
                    }
                    else
                    {
                        throw Error("<gui-free-skin>: At least one skin in SkinSet %s is not ImageSkin", skin->c_str());
                    }
                    ++c;
                }

            }
        }
        else
        {
            throw Error("<gui-free-skin>: SkinMan %s is not SkinSet", skin->c_str());
        }
    }


};//XMLFactory
