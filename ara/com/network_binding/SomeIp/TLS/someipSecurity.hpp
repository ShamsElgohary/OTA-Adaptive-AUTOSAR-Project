#pragma once

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "../someip.hpp"

using namespace std;
using stream_base = boost::asio::ssl::stream_base;
using tcp = boost::asio::ip::tcp;

namespace someip { 
    namespace security {

        enum{ max_length = 1024 };

        typedef enum syncType_t{
            sync_t,
            async_t
        }syncType_t;


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////// SESSION ///////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        class SessionTLS : public std::enable_shared_from_this<SessionTLS> , public someipConnection
        {
            public:
            SessionTLS(tcp::socket socket, boost::asio::ssl::context& context);

            SessionTLS(boost::asio::ssl::context& ssl_context, boost::asio::io_context& io_context);

            void handshake(stream_base::handshake_type baseType , syncType_t handshakeType); 

            /* FUNCTION TO SEND A SOMEIP MESSAGE  */
            bool SendMessage(someipMessage &msg);

            /* FUNCTION TO READ A SOMEIP MESSAGE  */
            someipMessage ReceiveMessage();		

            /* FUNCTION TO SEND A SOMEIP MESSAGE ASYNCH */
            bool SendMessageAsynch(someipMessage &msg );

            /* FUNCTION TO READ A SOMEIP MESSAGE ASYNCH */
            someipMessage ReceiveMessageAsynch();	

            /* MESSAGE TYPES */

            someipMessage SendRequest(someipMessage &msg);

            bool SendResponse(someipMessage &msg);

            /* REQUEST NO RESPONSE */
            bool SendFireAndForget(someipMessage &msg);
            
            bool SendNotification(someipMessage &msg);   

            /* CLOSE SOCKET CONNECTION */
            bool CloseConnection();
    
            protected:
            boost::asio::ssl::stream<tcp::socket> ssl_socket;

        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////// SERVER ///////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        class ServerTLS : public someipConnection
        {
        public:
        
            ServerTLS(boost::asio::io_context& io_context, uint16_t port, std::string CertificateDir , std::string IPv4 = LOOPBACK_IP);
                
            /* SERVER LISTENING */
            void ServerListen();  

            /* FUNCTION TO SEND A SOMEIP MESSAGE  */
            bool SendMessage(someipMessage &msg);

            /* FUNCTION TO READ A SOMEIP MESSAGE  */
            someipMessage ReceiveMessage();		

            /* FUNCTION TO SEND A SOMEIP MESSAGE ASYNCH */
            bool SendMessageAsynch(someipMessage &msg );

            /* FUNCTION TO READ A SOMEIP MESSAGE ASYNCH */
            someipMessage ReceiveMessageAsynch();	


            /* MESSAGE TYPES */

            someipMessage SendRequest(someipMessage &msg);

            bool SendResponse(someipMessage &msg);

            /* REQUEST NO RESPONSE */
            bool SendFireAndForget(someipMessage &msg);
            
            bool SendNotification(someipMessage &msg);   

            /* CLOSE SOCKET CONNECTION */
            bool CloseConnection();
     

        private:
        
            shared_ptr<SessionTLS> session_instance = nullptr;
            boost::asio::ssl::context ssl_context;
            boost::asio::ip::tcp::acceptor acceptor_;
        };


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////// CLIENT //////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



        class ClientTLS:public std::enable_shared_from_this<ClientTLS>, public SessionTLS
        {
            public:
            ClientTLS(boost::asio::io_context& io_context,
                        boost::asio::ssl::context& context,
                        uint16_t port,
                        std::string clientCertificate, 
                        std::string IPv4 = LOOPBACK_IP);
                        

            bool verify_certificate(bool preverified,
                boost::asio::ssl::verify_context& ctx);

            void connect(syncType_t connectType,const tcp::resolver::results_type& endpoints);

        };

    } // namespace security
} // namespace someip
