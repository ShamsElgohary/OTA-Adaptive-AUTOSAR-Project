#pragma once

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>


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


        class session :public std::enable_shared_from_this<session>
        {
            public:

            session(tcp::socket socket, boost::asio::ssl::context& ssl_context);

            session(boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context);


            void handshake(securityConfg_t securityType,  
                            stream_base::handshake_type baseType ,
                            syncType_t handshakeType); 

            string receiveData(syncType_t readType);

            void sendData(syncType_t writeType, string data);

            protected:
            boost::asio::ssl::stream<tcp::socket> ssl_socket;
            char data_[max_length];
        };


        class server 
        {
        public:
            server(boost::asio::io_context& io_context, unsigned short port,securityConfg_t securityType,syncType_t acceptorType);

            string receiveData(syncType_t readType);

            void sendData(syncType_t writeType, string data);
                
                /* SERVER LISTENING */
            void ServerListen();

            /* OPEN SOCKET CONNECTION*/
            bool OpenConnection();

            /* CLOSE SOCKET CONNECTION */
            bool CloseConnection();

            //     /* FUNCTION TO SEND A SOMEIP MESSAGE USING TCP */
            // bool SendMessage(someip_Message &msg);

            // /* FUNCTION TO READ A SOMEIP MESSAGE USING TCP */
            // someip_Message ReceiveMessage();		

            // /* FUNCTION TO SEND A SOMEIP MESSAGE ASYNCH */
            // bool SendMessageAsynch(someip_Message &msg );

            // /* FUNCTION TO READ A SOMEIP MESSAGE ASYNCH */
            // someip_Message ReceiveMessageAsynch();	


            // /* MESSAGE TYPES */

            // someip_Message SendRequest(someip_Message &msg);

            // bool SendResponse(someip_Message &msg);

            // /* REQUEST NO RESPONSE */
            // bool SendFireAndForget(someip_Message &msg);

            // bool SendNotification(someip_Message &msg);    
            

        private:

            shared_ptr<session> sessionInstance = nullptr;
            boost::asio::ip::tcp::acceptor acceptor_;
            boost::asio::ssl::context context_;
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

            /* CLOSE SOCKET CONNECTION */
            bool CloseConnection();

            
            private:
            char data_[max_length];
        };

    } // namespace security
} // namespace someip
