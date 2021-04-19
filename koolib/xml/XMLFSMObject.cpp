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
  last mod: $Id: XMLFSMObject.cpp,v 1.4 2005/03/09 15:52:34 koolas Exp $

 ********************************************************************/

#include "koolib/xml/XMLChunksProgram.h"
#include "koolib/xml/XMLChunksMethod.h"
#include "koolib/xml/XMLFactoryUtil.h"
#include "koolib/dynamic_cast_call.h"

#define MEAN_AND_LEAN
#include "Useless/System/Application.h"
#include "Useless/System/Window.h"
#include "Useless/File/VIFS.h"
#include "Useless/File/IFileStream.h"
#include "Useless/Functional/SignalScope.h"
#include "Useless/XML/Resources/CreateSystem.h"
#undef XMLDocument

using namespace Useless;
using namespace XMLFactory;
using namespace XMLProgram;

namespace XMLProgram {

    struct XMLFSMObject : XMLFiniteStateMachine
    {
        SignalScope _scope;

        XMLFSMObject()
        {
            _scope.add( Tie2Signal( Application::OnUpdate, this, &XMLFSMObject::OnAppUpdate ));
            _scope.add( Tie2Signal( Application::OnIdle, this, &XMLFSMObject::OnAppIdle ));

            add_methods_block( "__application__", this, this )
                .def("Terminate", &XMLFSMObject::Terminate)
                .def("SetEncoding", &XMLFSMObject::SetEncoding, "name")
                .def("SetIdleDT", &XMLFSMObject::SetIdleDT, "timeDelta")
                ;

            Window &wnd = **ScreenResource("system","screen");

            add_methods_block( "__window__", &wnd, this )
                .def("ShowCursor", &Window::ShowCursor )
                .def("HideCursor", &Window::HideCursor )
                .def("InterceptCursor", &Window::InterceptCursor )
                .def("ReleaseCursor", &Window::ReleaseCursor )
                .def("SetCursorPos", &Window::SetCursorPos, "x", "y" )
                .def("Reposition", &Window::Reposition, "x", "y" )
                .def("ShowDecorations", &Window::ShowDecorations )
                .def("HideDecorations", &Window::HideDecorations )
                .def("AllowSizing", &Window::AllowSizing, "allow" )
                .def("AllowClear", &Window::AllowClear, "allow" )
                .def("AllowCursorWrapping", &Window::AllowCursorWrapping, "allow" )
                ;

            _scope.add( Tie2Signal( wnd.OnIdle, this, &XMLFSMObject::OnWindowIdle ));
            _scope.add( Tie2Signal( wnd.OnActivate, this, &XMLFSMObject::OnWindowActivate ));
            _scope.add( Tie2Signal( wnd.OnDeactivate, this, &XMLFSMObject::OnWindowDeactivate ));
            _scope.add( Tie2Signal( wnd.OnKeyPress, this, &XMLFSMObject::OnWindowKeyPress ));
            _scope.add( Tie2Signal( wnd.OnKeyRelease, this, &XMLFSMObject::OnWindowKeyRelease ));
            _scope.add( Tie2Signal( wnd.OnMouseWheel, this, &XMLFSMObject::OnWindowMouseWheel ));
            _scope.add( Tie2Signal( wnd.OnResize, this, &XMLFSMObject::OnWindowResize ));
        }

        ~XMLFSMObject()
        {
            if ( MayCall("OnFsmKill") )
            {   
                ExecuteString( XMLTag("call").setattr("id", "OnFsmKill").to_string() );
            }
        }

        void Terminate()
        {
            Application::InstancePtr()->Terminate();
        }

        void SetEncoding( std::string name )
        {
            Application::SetEncoding( name.c_str() );
        }

        void SetIdleDT( int timeDelta )
        {
            Application::SetIdleDT( timeDelta );
        }

        bool MayCall( std::string name )
        {
            return ( 0 != XMLProgram::GetChunk( name, GetCurrentExecutionState() ));
        }

        void OnAppIdle( int currentTime, int timeDelta )
        {
            if ( MayCall("OnAppIdle") )
            {   
                ExecuteString( XMLTag("call")
                        .setattr("id", "OnAppIdle")
                        .setattr("currentTime", currentTime)
                        .setattr("timeDelta", timeDelta )
                        .to_string() );
            }
        }

        void OnAppUpdate()
        {
            if ( MayCall("OnAppUpdate") )
            {   
                ExecuteString( XMLTag("call")
                        .setattr("id", "OnAppUpdate")
                        .to_string() );
            }
        }

        void OnWindowIdle( int currentTime, int timeDelta )
        {
            if ( MayCall("OnWindowIdle") )
            {   
                ExecuteString( XMLTag("call")
                        .setattr("id", "OnWindowIdle")
                        .setattr("currentTime", currentTime)
                        .setattr("timeDelta", timeDelta )
                        .to_string() );
            }
        }

        void OnWindowActivate()
        {
            if ( MayCall("OnWindowActivate") )
            {
                ExecuteString( XMLTag("call")
                        .setattr("id", "OnWindowActivate")
                        .to_string() );
            }
        }

        void OnWindowDeactivate()
        {
            if ( MayCall("OnWindowDeactivate") )
            {
                ExecuteString( XMLTag("call")
                        .setattr("id", "OnWindowDeactivate")
                        .to_string() );
            }
        }

        void OnWindowKeyPress( int key )
        {
            if ( MayCall("OnWindowKeyPress") )
            {
                ExecuteString( XMLTag("call")
                        .setattr("id", "OnWindowKeyPress")
                        .setattr("key", key)
                        .to_string() );
            }
        }

        void OnWindowKeyRelease( int key )
        {
            if ( MayCall("OnWindowKeyRelease") )
            {
                ExecuteString( XMLTag("call")
                        .setattr("id", "OnWindowKeyRelease")
                        .setattr("key", key)
                        .to_string() );
            }
        }

        void OnWindowMouseWheel( int z, int dz )
        {
            if ( MayCall("OnWindowMouseWheel") )
            {
                ExecuteString( XMLTag("call")
                        .setattr("id", "OnWindowMouseWheel")
                        .setattr("z", z)
                        .setattr("dz", dz)
                        .to_string() );
            }
        }

        void OnWindowResize( int w, int h )
        {
            if ( MayCall("OnWindowResize") )
            {
                ExecuteString( XMLTag("call")
                        .setattr("id", "OnWindowResize")
                        .setattr("w", w)
                        .setattr("h", h)
                        .to_string() );
            }
        }
    };
    
};//XMLProgram



KOOLIB_API Useless::Resource* CreateRunFSM( Useless::XMLIterator i, Useless::XMLCreatorEnv *env )
{
    XMLFactory::Node _node( i );

    Attr< std::string > fsm("fsm");
    Attr< std::string > object("object");

    _node >> fsm;
    _node >> object;

    FSMResource::Type fsmObject;

    Resource *r = Resources::Instance().QueryResource("programm-objects", object.str() );
    if ( !r )
    {
        r = new AnyResource< FSMResource::Type >( new XMLFSMObject );
        Resources::Instance().Insert("programm-objects", object.str(), r );
    }
    
    fsmObject = *FSMResource( r );

    XMLDocumentResource::Type prg = *XMLDocumentResource( "programms", *fsm );
    prg.Root() >> (*fsmObject);

    return 0;
}


namespace XMLProgram {
    namespace XML
    {
        struct RUN {};
        struct KILL {};
        struct SEND {};
        struct IMPORT {};
    };
};

namespace XMLFactory {

    using namespace XMLProgram;
    using namespace Useless;
#define LOCAL_TAG_HANDLER( _name )  USELESS_XML_TAG_HANDLER( _name, ExecutionState )

    void SendToFSM( FSMResource::Type fsmObject, Node _node, ExecutionState &_state )
    {
        _state.SetResult(0);
        FSMStackGuard guard( &*fsmObject );
        ExecutionState &s0 = guard.GetState();

        if ( &GetGlobals() == s0._prevState )
        {
            ExecutionState z = s0;
            z._prevState = &_state;
            z.ExecuteChildren( _node );
            _state.SetResult( z.GetResult() );
        }
        else
        {
            ExecutionState *s = &s0;
            for ( ; &GetGlobals() != s->_prevState->_prevState; s = s->_prevState )
            {}
            ExecutionState z = (*s->_prevState);
            z._prevState = &_state;
            s->_prevState = &z;
            s0.ExecuteChildren( _node );
            _state.SetResult( s0.GetResult() );
        }

    }

    LOCAL_TAG_HANDLER( XML::RUN )
    {
        Attr< std::string > file("file");
        Attr< std::string > object("object");

        GetAttr( file, _node, _state );
        GetAttr( object, _node, _state );
        
        FSMResource::Type fsmObject;

        Resource *r = Resources::Instance().QueryResource("programm-objects", object.str() );
        if ( !r )
        {
            r = new AnyResource< FSMResource::Type >( new XMLFSMObject );
            Resources::Instance().Insert("programm-objects", object.str(), r );
        }

        fsmObject = *FSMResource( r );

        SendToFSM( fsmObject, _node, _state );
        
        Useless::SPointer< IFile > fileInput = Useless::IFS::Instance().OpenFile( file.str() );
        IFileStream stream( fileInput );
        std::string ln;
        std::string data;
        while ( std::getline( stream, ln ))
        {
            data += ln;
            data += "\n";
        }

        Useless::XMLDocument doc( data, file.str() );
        (*fsmObject).GetCurrentExecutionState().ExecuteChildren( doc.Root() );
    }

    LOCAL_TAG_HANDLER( XML::KILL )
    {
        Attr< std::string > object("object");
        GetAttr( object, _node, _state );
        Resources::Instance().Remove("programm-objects", object.str() );
    }

    LOCAL_TAG_HANDLER( XML::SEND )
    {
        Attr< std::string > _to("to");
        GetAttr( _to, _node, _state );
        FSMResource::Type fsmObject = *FSMResource("programm-objects", _to.str() );
        
        SendToFSM( fsmObject, _node, _state );
    }
    
    LOCAL_TAG_HANDLER( XML::IMPORT )
    {
        Attr< std::string > _id("id");
        GetAttr( _id, _node, _state );
        Useless::XMLDocument tag("<" + (*_id) + "/>");        
        try {
            xtdImport.Execute( tag.Root(), _state );
        }
        catch(...) {
            throw Error("<import> Module '%s' could not be imported.", (*_id).c_str());
        }
    }

};//XMLFactory

