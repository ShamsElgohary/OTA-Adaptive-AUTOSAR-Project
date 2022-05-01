#pragma once

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "someip.hpp"

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

        typedef enum securityConfg_t{
            tls,
        }securityConfg_t;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////// SESSION ///////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        class session : public std::enable_shared_from_this<session> , public someipConnection
        {
            public:
            session(tcp::socket socket, boost::asio::ssl::context& context);

            session(boost::asio::ssl::context& ssl_context, boost::asio::io_context& io_context);

            void handshake(securityConfg_t securityType,  
                            stream_base::handshake_type baseType ,
                            syncType_t handshakeType); 


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

    
            string receiveData(syncType_t readType);

            void sendData(syncType_t writeType, string data);

            protected:
            boost::asio::ssl::stream<tcp::socket> ssl_socket;
            char data_[max_length];

        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////// SERVER ///////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        class server : public someipConnection
        {
        public:
        
            server(boost::asio::io_context& io_context, unsigned short port,securityConfg_t securityType,syncType_t acceptorType);
                
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


            string receiveData_external(syncType_t readType);

            void sendData_external(syncType_t writeType, string data);      

        private:
        
            shared_ptr<session> session_instance = nullptr;
            boost::asio::ssl::context ssl_context;
            boost::asio::ip::tcp::acceptor acceptor_;
        };


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////// CLIENT //////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        class client:public std::enable_shared_from_this<client>, public session
        {
            public:
            client(boost::asio::io_context& io_context,
                boost::asio::ssl::context& context,
                const tcp::resolver::results_type& endpoints,
                securityConfg_t securityType,
                syncType_t handshakeType);

            bool verify_certificate(bool preverified,
                boost::asio::ssl::verify_context& ctx);

            void connect(syncType_t connectType,
                        const tcp::resolver::results_type& endpoints,
                        securityConfg_t securityType,
                        syncType_t handshakeType);

            
            private:
            char data_[max_length];
        };

    } // namespace security
} // namespace someip
