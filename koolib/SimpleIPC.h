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

 Following module uses following library:

    CryptoPP <http://www.eskimo.com/~weidai/cryptlib.html>

 ********************************************************************/

#ifndef __INCLUDED_KOOLIB_SIMPLE_IPC_H__
#define __INCLUDED_KOOLIB_SIMPLE_IPC_H__

#ifndef USELESS_HAS_CRYPTOPP
#pragma message("In order to use SimpleIPC, get CryptoPP library, and set USELESS_HAS_CRYPTOPP flag")
#else

#include <dll.h> // - CryptoPP library
#include <socketft.h> // defined in CryptoPP library
#pragma comment(lib,"WS2_32.Lib") // required win32 library


namespace SimpleIPC
{
    // Global static class used to enable sockets
    struct SocketsService
    {
        static SocketsService & Instance()
        {
            static SocketsService sockets;
            return sockets;
        }

        private:
        SocketsService()
        {
            CryptoPP::Socket::StartSockets();
        }

        ~SocketsService()
        {
            CryptoPP::Socket::ShutdownSockets();
        }
    };

    // Socket wrapper, base of Server and Client classes
    struct NetworkEntity
    {
        NetworkEntity()
        {
            SocketsService::Instance();
        }

        virtual ~NetworkEntity()
        {
        }

        CryptoPP::Socket m_socket;
    };

    // Server socket
    struct Server : NetworkEntity
    {
        Server( int port )
        {
            m_servSocket.Create( SOCK_STREAM );        
            m_servSocket.Bind( port );
            m_servSocket.Listen();
            m_servSocket.Accept( m_socket );
        }

        private:
        CryptoPP::Socket m_servSocket;
    };

    // Client socket
    struct Client : NetworkEntity
    {
        Client( const char *addr, int port )
        {
            m_socket.Create( SOCK_STREAM );        
            if ( !m_socket.Connect( addr, port ))
            {
                throw std::exception("Cannot connect to server");
            }
        }
    };

    // Communication port
    struct Port
    {
        Port( NetworkEntity *entity )
            : m_entity( entity )
            , m_sink( entity->m_socket )
            , m_source( entity->m_socket, false, new CryptoPP::StringSink( m_buffer ))
            {
            }

        const std::string & Recv()
        {
            unsigned long byteCount = ULONG_MAX;
            m_buffer.resize(0);
            m_source.Pump2( byteCount, false );
            return m_buffer;
        }

        const std::string WaitRecv()
        {
            unsigned long byteCount = 1;
            m_buffer.resize(0);
            m_source.Pump2( byteCount, true );
            byteCount = ULONG_MAX;
            m_source.Pump2( byteCount, false );
            return m_buffer;
        }

        void Send( char c )
        {
            m_sink.Put( c );
        }

        void Send( const std::string &s )
        {
            m_sink.Put( (byte*)s.c_str(), s.size() );
        }

        void SendEnd()
        {
            m_sink.MessageEnd();
        }

        private:
        CryptoPP::member_ptr< NetworkEntity > m_entity;
        CryptoPP::SocketSource      m_source;
        CryptoPP::SocketSink        m_sink;
        std::string                 m_buffer;
    };
};

#endif
#endif
