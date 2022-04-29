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



            session::session(tcp::socket socket, boost::asio::ssl::context& ssl_context)
             : ssl_socket(std::move(socket), ssl_context)
            {

            }

            session::session(boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context)
            : ssl_socket(io_context, ssl_context)
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
                            std::cout<< "HANDSHAKE DONE \n ";
                        }
                        });
                        break;
                }
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
                            std::cout<< "RECEIVE DATA DONE \n ";
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
                            std::cout<< "Sending DATA DONE \n ";
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

        string server::receiveData(syncType_t readType)
        {
            string x = sessionInstance->receiveData(readType);
            return x;
        }

        void server::sendData(syncType_t writeType, string data)
        {
            sessionInstance->sendData(writeType,data);
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

                //  ssl_socket.set_verify_callback(
                //      std::bind(&client::verify_certificate, this, _1, _2));

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
                std::cout << "Verifying " << subject_name << "\n";

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



















