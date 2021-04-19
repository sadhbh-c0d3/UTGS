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
 ********************************************************************/

#ifndef USELESS_HAS_CRYPTOPP
#pragma message("In order to use SimpleIPC, get CryptoPP library, and set USELESS_HAS_CRYPTOPP flag")
#else

#define MEAN_AND_LEAN
#include <koolib/xml/XMLModuleBuiltIn.h>
#include <koolib/xml/XMLChunksMethod.h>
#include <koolib/SimpleIPC.h>

namespace XMLProgram {

    struct SimpleIPCPortProxy : XMLCodeBlock, SimpleIPC::Port
    {
        SimpleIPCPortProxy( SimpleIPC::NetworkEntity *entity ): SimpleIPC::Port( entity )
        {
            add_methods( this )
                .def_remap_0< std::string >("Recv", &SimpleIPC::Port::Recv )
                .def_remap_0< std::string >("WaitRecv", &SimpleIPC::Port::WaitRecv )
                .def_remap_1< void, TextUtf8 >("Send",
                        (void (SimpleIPC::Port::*)( const std::string &))&SimpleIPC::Port::Send, "msg")
                .def("SendEnd", &SimpleIPC::Port::SendEnd )
                ;
        }
    };
    
    namespace XML_IMPORT
    {
        struct SIMPLE_IPC : XMLCodeBlock
        {
            SIMPLE_IPC()
            {
                // turn on socket support
                SimpleIPC::SocketsService::Instance();

                add_methods( this )
                    .def("CreatePort", CreatePort, "port")
                    .def("Connect", Connect, "port", "addr")
                    ;
            }

            IChunkPtr CreatePort( int port )
            {
                return new SimpleIPCPortProxy( new SimpleIPC::Server( port ));
            }

            IChunkPtr Connect( int port, std::string addr )
            {
                return new SimpleIPCPortProxy( new SimpleIPC::Client( addr.c_str(), port ));
            }
        };

    };// namespace XML_IMPORT
};//namespace XMLProgram

namespace XMLFactory {
    using namespace XMLProgram;

#define DEFINE_MODULE_IMPORT( _modName ) USELESS_XML_TAG_HANDLER( _modName, ExecutionState ) \
{\
    _state.SetResult( new _modName() ); \
}

DEFINE_MODULE_IMPORT( XML_IMPORT::SIMPLE_IPC );

};//XMLFactory

#endif
