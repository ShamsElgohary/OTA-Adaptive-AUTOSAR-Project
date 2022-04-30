#include "someipSecurity.hpp"

namespace someip
{
    namespace security {
        
        using namespace std;
        using stream_base = boost::asio::ssl::stream_base;
        using tcp = boost::asio::ip::tcp;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////// SESSION //////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


            session::session(tcp::socket socket, boost::asio::ssl::context& ssl_context, boost::asio::io_context& io_context)
             : ssl_socket(std::move(socket), ssl_context), session_io_context(io_context)
            {

            }

            session::session(boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context)
            : ssl_socket(io_context, ssl_context), session_io_context(io_context)
            {
                
            }         

            void session::handshake(securityConfg_t securityType, 
                                    stream_base::handshake_type baseType ,
                                    syncType_t handshakeType)
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
            bool session::SendMessage(someip_Message &msg)
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
            someip_Message session::ReceiveMessage()
            {
                someip_Message someipMsg;

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
            bool session::SendMessageAsynch(someip_Message &msg )
            {
                try {
                    Serializer serializer;
                    std::stringstream ss;
                    // SERIALIZE SOMEIP MESSAGE STRUCT  
                    serializer.Serialize(ss, &msg);
                    std::string mssgBuf = ss.str();
                    ssl_socket.async_write_some( boost::asio::buffer(mssgBuf), &OnSendCompleted );
                    // IMPORTANT FOR ASYNCHRONOUS OPERATIONS 
                    session_io_context.run();

                }catch ( boost::system::system_error e) {
                    std::cout << "[someip] " << e.what() << "\n";
                    return false; // MESSAGE NOT SENT
                }

                return true;

            }

            /* FUNCTION TO READ A SOMEIP MESSAGE ASYNCH */
            someip_Message session::ReceiveMessageAsynch()
            {
                someip_Message someipMsg;

                try {

                    char buff[512];
                    ssl_socket.async_read_some(boost::asio::buffer(buff), &OnReceiveCompleted);
                    
                    // IMPORTANT FOR ASYNCHRONOUS OPERATIONS 
                    session_io_context.run();
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


            someip_Message session::SendRequest(someip_Message &req)
            {
                req.header.setMessageType( MessageType::REQUEST );
                
                /* SEND MESSAGE */
                this->SendMessage(req);

                /* RESPONSE */
                someip_Message responseMsg = this->ReceiveMessage();

                if( responseMsg.header.getMessageType() != MessageType::RESPONSE )
                {
                    std::cout<< "[someip] MESSAGE TYPE ISN'T RESPONSE MESSAGE " << std::endl;
                    /* CONTAINS HEADER WITH ERROR CODE (DEFAULT CONSTRUCTOR) */
                    //someip_Message msg;
                    //return msg;
                }

                return responseMsg;
            }


            bool session::SendResponse(someip_Message &responseMsg)
            {
                responseMsg.header.setMessageType( MessageType::RESPONSE );
                bool operationStatus = this->SendMessage(responseMsg);
                return operationStatus;
            }


            /* REQUEST NO RESPONSE */
            bool session::SendFireAndForget(someip_Message &msg)
            {
                msg.header.setMessageType( MessageType::REQUEST_NO_RETURN );
                this->SendMessageAsynch(msg);
                return true;
            }

            bool session::SendNotification(someip_Message &msg)
            {
                msg.header.setMessageType( MessageType::NOTIFICATION );
                this->SendMessageAsynch(msg);
                return true;
            }

            string session::receiveData(syncType_t readType)
            {
                
            int x = 0;
                switch(readType)
                {
                    case (syncType_t::sync_t):
                        x = (ssl_socket).read_some(boost::asio::buffer(data_));
                    break;

                    case(syncType_t::async_t):
                    auto self(shared_from_this());
                    (ssl_socket).async_read_some(boost::asio::buffer(data_),
                        [this, self](const boost::system::error_code& ec, std::size_t length)
                        {
                            if (!ec)
                            {
                            std::cout<< "[someip] RECEIVE DATA DONE \n ";
                            }
                        });
                    break;

                }

                data_[x] = '\0';
                return data_;

            }

            void session::sendData(syncType_t writeType,string data)
            {
                switch(writeType)
                {
                    case (syncType_t::sync_t):
                        
                        (ssl_socket).write_some(boost::asio::buffer(data,data.length()));
                        break;

                    case(syncType_t::async_t):
                    
                        auto self(shared_from_this());
                        (ssl_socket).async_write_some(boost::asio::buffer(data,data.length()),
                        [this, self](const boost::system::error_code& ec, std::size_t length)
                        {
                            if (!ec)
                            {
                            std::cout<< "[someip] Sending DATA DONE \n ";
                            }
                        });   
                        break;
                }

            }         

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// SERVER //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




        server::server(boost::asio::io_context& io_context, unsigned short port,securityConfg_t securityType,syncType_t acceptorType)
            : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
            context_(boost::asio::ssl::context::tls)
        {
            context_.set_options(
                boost::asio::ssl::context::default_workarounds
                |boost::asio::ssl::context::no_sslv2
                | boost::asio::ssl::context::single_dh_use);

            //context_.set_password_callback(boost::bind(&server::get_password, this));
            context_.use_certificate_chain_file("../Certificates/server.crt");
            context_.use_private_key_file("../Certificates/server.key", boost::asio::ssl::context::pem);
            context_.use_tmp_dh_file("../Certificates/dh2048.pem");

            ServerListen();
        }
        
        void server::ServerListen()
        {
            /*GET SOCKET FROM ACCEPT TO CONSTRACUT SESSION*/
            sessionInstance = std::make_shared<session>(std::move(acceptor_.accept()), context_);
            sessionInstance->handshake(tls, stream_base::server, sync_t);           
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// CLIENT ///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            client::client(boost::asio::io_context& io_context,
                boost::asio::ssl::context& context,
                const tcp::resolver::results_type& endpoints,securityConfg_t securityType,syncType_t handshakeType)
                : session(io_context, context)
            {
                ssl_socket.set_verify_mode(boost::asio::ssl::verify_peer);

                // SET CALL BACK, CALL VERIFY CERTIFICATE WHICH IS A FUNCTION OF THIS GIVING IT 2 PARAMETERS
                ssl_socket.set_verify_callback([this](bool p, boost::asio::ssl::verify_context& ctx) { return verify_certificate(p, ctx); });

                connect(sync_t,endpoints,securityType,handshakeType);
            }

            bool client::verify_certificate(bool preverified,
                boost::asio::ssl::verify_context& ctx)
            {
                char subject_name[256];
                X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
                X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
                std::cout << "[someip] Verifying " << subject_name << "\n";

                return preverified;
            }

            void client::connect(syncType_t connectType,const tcp::resolver::results_type& endpoints,securityConfg_t securityType,syncType_t handshakeType)
            {  

            switch(connectType)
            {
                case sync_t:
                
                    boost::asio::connect(ssl_socket.lowest_layer(), endpoints);
                    this->handshake(securityType,stream_base::client,handshakeType);
                    break;

                case syncType_t::async_t:

                    boost::asio::async_connect(ssl_socket.lowest_layer(), endpoints,
                    [this,securityType,handshakeType](const boost::system::error_code& error,
                    const tcp::endpoint& )
                    {
                    if (!error)
                    {
                        this->handshake(securityType,stream_base::client,handshakeType);
                      }
                    else
                    {
                        std::cout << "[someip] Connect failed: " << error.message() << "\n";
                    }
                });
                    break;
            }

            }

            bool client::CloseConnection()
            {
                ssl_socket.lowest_layer().cancel();
                //node.lowest_layer.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
		        ssl_socket.lowest_layer().close();
                return true;        
            }

   } // namespace security
}  // namespace someip


















