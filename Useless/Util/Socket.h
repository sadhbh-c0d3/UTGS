#include "Useless/Util/SPointer.h"
#include <string>

namespace Useless {

    struct USELESS_API Port
    {
        virtual ~Port() {}
        virtual void Send( char ) = 0;
        virtual void Send( const std::string & ) = 0; 
        virtual void SendEnd() = 0;
        virtual void Recv( std::string & ) = 0;
        virtual void WaitRecv( std::string & ) = 0;
    };

    struct Client
    {
        virtual ~Client() {}
        virtual void Disconnect() = 0;
        virtual void Wait() = 0;
    };
    
    struct Server
    {
        virtual ~Server() {}
        virtual void Shutdown() = 0;
        virtual void Wait() = 0;
    };
    
    struct Service
    {
        virtual ~Service() {}
        virtual SPointer< Service > CreateInstance() = 0;
        virtual void Run () = 0;
        
        void Send( char c )               { m_port->Send( c ); }
        void Send( const std::string &s ) { m_port->Send( s ); }
        void SendEnd()                    { m_port->SendEnd(); }
        void Recv( std::string &s )       { m_port->Recv( s ); }
        void WaitRecv( std::string &s )   { m_port->WaitRecv( s ); }
        
        SPointer< Port > m_port;
    };

    USELESS_API SPointer< Server > CreateServer( SPointer< Service >, int port );
    USELESS_API SPointer< Client > ConnectToService( SPointer< Client >, int port, const std::string &addr = "localhost" );



    struct MyService : Service
    {
        SPointer< Service > CreateInstance()
        {
            return new MyService;
        }

        void Run()
        {
            try
            {
                std::string resp;
                Send("Who are you?");
                WaitRecv( resp );
                if ( "MyClient" != resp )
                {
                    throw false;
                }
            }
            catch()
            {
            }
            
            SendEnd();
        }
    };

    struct MyClient : Client
    {
        void Run()
        {
            try {
                std::string resp;
                WaitRecv( resp );
                if ( "Who are you?" != resp )
                {
                    throw false
                }
                Send("MyClient");
            }
            catch()
            {
            }
            
            SendEnd();
        }
    };

    SPointer< Server > srv = CreateServer( new MyService, 666 );
    srv->Shutdown();
    srv->Wait();

    
    SPointer< Client > cli = ConnectToService( new MyClient, 666 );
    cli->Disconnect();
    cli->Wait();


};//Useless
