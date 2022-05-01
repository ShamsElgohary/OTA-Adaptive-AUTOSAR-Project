#include "someipSecurity.hpp"

using namespace someip;
using namespace std;
using stream_base = boost::asio::ssl::stream_base;
using tcp = boost::asio::ip::tcp;


namespace someip
{
    namespace security {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// SESSION //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            SessionTLS::SessionTLS(boost::asio::ssl::context& ssl_context, boost::asio::io_context& io_context)
            : ssl_socket(io_context, ssl_context)
            { 

            }         

            SessionTLS::SessionTLS(tcp::socket socket, boost::asio::ssl::context& context)
            : ssl_socket(std::move(socket), context)
            {
                
            }

            void SessionTLS::handshake(stream_base::handshake_type baseType , syncType_t handshakeType)
            {
                switch(handshakeType)
                {
                    case(sync_t):

                        (ssl_socket).handshake(baseType);
                        break;

                    case(syncType_t::async_t):

                        (ssl_socket).async_handshake(baseType, 
                        [this](const boost::system::error_code& error)
                        {
                        if (!error)
                        {
                            std::cout<< "[someip] HANDSHAKE DONE \n ";
                        }
                        });
                        break;
                }
            }


            /* FUNCTION TO SEND A SOMEIP MESSAGE USING TCP */
            bool SessionTLS::SendMessage(someipMessage &msg)
            {
                try {
                    Serializer serializer;
                    std::stringstream ss;
                    // SERIALIZE SOMEIP MESSAGE STRUCT  
                    serializer.Serialize(ss, &msg);
                    std::string mssgBuf = ss.str();
                    ssl_socket.write_some( boost::asio::buffer(mssgBuf) );

                }catch ( boost::system::system_error e) {
                    std::cout << "[someip] " << e.what() << "\n";
                    return false; // MESSAGE NOT SENT
                }

                return true;
            }

            /* FUNCTION TO READ A SOMEIP MESSAGE USING TCP */
            someipMessage SessionTLS::ReceiveMessage()
            {
                someipMessage someipMsg;

                try {
                    char buff[512];
                    size_t read = ssl_socket.read_some(boost::asio::buffer(buff));
                    std::string mssgBuf = buff;
                    std::stringstream ss;
                    ss<< mssgBuf;
                    Deserializer deserializer;
                    // DESERIALIZE SOMEIP MESSAGE STRUCT  
                    deserializer.Deserialize(ss, &someipMsg);

                }catch ( boost::system::system_error e) {
                    std::cout << "[someip] " << e.what() << "\n";
                    someipMsg.header;
                }

                return someipMsg;
            }

            /* FUNCTION TO SEND A SOMEIP MESSAGE ASYNCH */
            bool SessionTLS::SendMessageAsynch(someipMessage &msg )
            {
                try {
                    Serializer serializer;
                    std::stringstream ss;
                    // SERIALIZE SOMEIP MESSAGE STRUCT  
                    serializer.Serialize(ss, &msg);
                    std::string mssgBuf = ss.str();
                    ssl_socket.async_write_some( boost::asio::buffer(mssgBuf), &OnSendCompleted );
                    // IMPORTANT FOR ASYNCHRONOUS OPERATIONS 
//                    session_io_context.run();

                }catch ( boost::system::system_error e) {
                    std::cout << "[someip] " << e.what() << "\n";
                    return false; // MESSAGE NOT SENT
                }

                return true;

            }

            /* FUNCTION TO READ A SOMEIP MESSAGE ASYNCH */
            someipMessage SessionTLS::ReceiveMessageAsynch()
            {
                someipMessage someipMsg;

                try {

                    char buff[512];
                    ssl_socket.async_read_some(boost::asio::buffer(buff), &OnReceiveCompleted);
                    
                    // IMPORTANT FOR ASYNCHRONOUS OPERATIONS 
//                    session_io_context.run();
                    std::string mssgBuf = buff;
                    std::stringstream ss;
                    ss<< mssgBuf;
                    Deserializer deserializer;
                    // DESERIALIZE SOMEIP MESSAGE STRUCT  
                    deserializer.Deserialize(ss, &someipMsg);

                }catch ( boost::system::system_error e) {
                    std::cout << "[someip] " << e.what() << "\n";
                    someipMsg.header;
                    // MESSAGE NOT READ
                }

            return someipMsg;
            }


            someipMessage SessionTLS::SendRequest(someipMessage &req)
            {
                req.header.setMessageType( MessageType::REQUEST );
                
                /* SEND MESSAGE */
                this->SendMessage(req);

                /* RESPONSE */
                someipMessage responseMsg = this->ReceiveMessage();

                if( responseMsg.header.getMessageType() != MessageType::RESPONSE )
                {
                    std::cout<< "[someip] MESSAGE TYPE ISN'T RESPONSE MESSAGE " << std::endl;
                    /* CONTAINS HEADER WITH ERROR CODE (DEFAULT CONSTRUCTOR) */
                    //someipMessage msg;
                    //return msg;
                }

                return responseMsg;
            }


            bool SessionTLS::SendResponse(someipMessage &responseMsg)
            {
                responseMsg.header.setMessageType( MessageType::RESPONSE );
                bool operationStatus = this->SendMessage(responseMsg);
                return operationStatus;
            }


            /* REQUEST NO RESPONSE */
            bool SessionTLS::SendFireAndForget(someipMessage &msg)
            {
                msg.header.setMessageType( MessageType::REQUEST_NO_RETURN );
                this->SendMessageAsynch(msg);
                return true;
            }

            bool SessionTLS::SendNotification(someipMessage &msg)
            {
                msg.header.setMessageType( MessageType::NOTIFICATION );
                this->SendMessageAsynch(msg);
                return true;
            }

            bool SessionTLS::CloseConnection()
            {
                ssl_socket.lowest_layer().cancel();
                //node.lowest_layer.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
		        ssl_socket.lowest_layer().close();
                return true;        
            }



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// SERVER //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        ServerTLS::ServerTLS(boost::asio::io_context& io_context, uint16_t port,  std::string CertificateDir, std::string IPv4)
            : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
              ssl_context(boost::asio::ssl::context::tls)
         {
            ssl_context.set_options(
                boost::asio::ssl::context::default_workarounds
                |boost::asio::ssl::context::no_sslv2
                | boost::asio::ssl::context::single_dh_use);

            
            // std::string serverCertificate = ;
            // std::string serverKey = ;
            // std::string dh = ;

            //context_.set_password_callback(boost::bind(&server::get_password, this));
            ssl_context.use_certificate_chain_file("Certificates/server.crt");
            ssl_context.use_private_key_file("Certificates/server.key", boost::asio::ssl::context::pem);
            ssl_context.use_tmp_dh_file("Certificates/dh2048.pem");
        }
        
        void ServerTLS::ServerListen()
        {
            session_instance = std::make_shared<SessionTLS>(std::move(acceptor_.accept()), ssl_context);
            session_instance->handshake(stream_base::server, sync_t);           
        }


        /* FUNCTION TO SEND A SOMEIP MESSAGE  */
        bool ServerTLS::SendMessage(someipMessage &msg)
        {
            return session_instance->SendMessage(msg);
        }

        /* FUNCTION TO READ A SOMEIP MESSAGE  */
        someipMessage ServerTLS::ReceiveMessage()
        {
            return session_instance->ReceiveMessage();
        }

        /* FUNCTION TO SEND A SOMEIP MESSAGE ASYNCH */
        bool ServerTLS::SendMessageAsynch(someipMessage &msg )
        {
            return session_instance->SendMessageAsynch(msg);
        }

        /* FUNCTION TO READ A SOMEIP MESSAGE ASYNCH */
        someipMessage ServerTLS::ReceiveMessageAsynch()
        {
            return session_instance->ReceiveMessageAsynch();
        }


        /* MESSAGE TYPES */

        someipMessage ServerTLS::SendRequest(someipMessage &msg)
        {
            return session_instance->SendRequest(msg);
        }

        bool ServerTLS::SendResponse(someipMessage &msg)
        {
            return session_instance->SendResponse(msg);
        }

        /* REQUEST NO RESPONSE */
        bool ServerTLS::SendFireAndForget(someipMessage &msg)
        {
            return session_instance->SendFireAndForget(msg);
        }
        
        bool ServerTLS::SendNotification(someipMessage &msg)
        {
            return session_instance->SendNotification(msg);
        }

        /* CLOSE SOCKET CONNECTION */
                /* CLOSE SOCKET CONNECTION */
        bool ServerTLS::CloseConnection()
        {
            return session_instance->CloseConnection();
        }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// CLIENT ///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ClientTLS::ClientTLS(boost::asio::io_context& io_context,
                        boost::asio::ssl::context& context,
                        uint16_t port,
                        std::string clientCertificate,
                        std::string IPv4)
                : SessionTLS(context, io_context)
        {

            boost::asio::ip::tcp::resolver resolver(io_context);

            boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(IPv4 ,std::to_string(port));
            
            // "Certificates/server.crt"
            context.load_verify_file(clientCertificate);

            ssl_socket.set_verify_mode(boost::asio::ssl::verify_peer);

            // SET CALL BACK, CALL VERIFY CERTIFICATE WHICH IS A FUNCTION OF THIS GIVING IT 2 PARAMETERS
            ssl_socket.set_verify_callback([this](bool p, boost::asio::ssl::verify_context& ctx) { return verify_certificate(p, ctx); });

            connect(sync_t,endpoints);
        }

        bool ClientTLS::verify_certificate(bool preverified,
            boost::asio::ssl::verify_context& ctx)
        {
            char subject_name[256];
            X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
            X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
            std::cout << "[someip] Verifying " << subject_name << "\n";

            return preverified;
        }

        void ClientTLS::connect(syncType_t connectType,const tcp::resolver::results_type& endpoints)
        {  

            // FUNCTION IS CONFIGURABLE AS WE CAN TAKE INPUT ( CONNECT TYPE WHICH CHOOSES SYNC OR ASYNC )
            // SAME WITH HANDSHAKE

        switch(connectType)
        {
            case sync_t:
            
                boost::asio::connect(ssl_socket.lowest_layer(), endpoints);
                this->handshake(stream_base::client, syncType_t::sync_t);
                break;

            case syncType_t::async_t:

                boost::asio::async_connect(ssl_socket.lowest_layer(), endpoints,
                [this](const boost::system::error_code& error,
                const tcp::endpoint& )
                {
                if (!error)
                {
                    this->handshake(stream_base::client,syncType_t::async_t);
                    }
                else
                {
                    std::cout << "[someip] Connect failed: " << error.message() << "\n";
                }
            });
                break;
        }

        }


   } // namespace security
}  // namespace someip
