#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

using boost::asio::ip::tcp;
using std::placeholders::_1;
using std::placeholders::_2;

enum { max_length = 1024 };
using namespace std;
using stream_base = boost::asio::ssl::stream_base;
using tcp = boost::asio::ip::tcp;



typedef enum syncType_t{
    sync_t,
    async_t
}syncType_t;

typedef enum securityConfg_t{
    tls,
}securityConfg_t;



  class client:public std::enable_shared_from_this<client>
  {
  public:
    client(boost::asio::io_context& io_context,
        boost::asio::ssl::context& context,
        const tcp::resolver::results_type& endpoints,securityConfg_t securityType,syncType_t handshakeType)
      : node(io_context, context)
    {
      node.set_verify_mode(boost::asio::ssl::verify_peer);

      // SET CALL BACK, CALL VERIFY CERTIFICATE WHICH IS A FUNCTION OF THIS GIVING IT 2 PARAMETERS
      node.set_verify_callback(
          std::bind(&client::verify_certificate, this, _1, _2));

      connect(sync_t,endpoints,securityType,handshakeType);
    }

  public:
    bool verify_certificate(bool preverified,
        boost::asio::ssl::verify_context& ctx)
    {
      char subject_name[256];
      X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
      X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
      std::cout << "Verifying " << subject_name << "\n";

      return preverified;
    }

    void connect(syncType_t connectType,const tcp::resolver::results_type& endpoints,securityConfg_t securityType,syncType_t handshakeType)
    {

      switch(connectType)
      {
        case sync_t:
               /* should we handle error*/
              boost::asio::connect(node.lowest_layer(), endpoints);
              handshake(securityType,handshakeType);
          break;
        case syncType_t::async_t:
          boost::asio::async_connect(node.lowest_layer(), endpoints,
          [this,securityType,handshakeType](const boost::system::error_code& error,
            const tcp::endpoint& )
          {
            if (!error)
            {
              handshake(securityType,handshakeType);
            }
            else
            {
              std::cout << "Connect failed: " << error.message() << "\n";
            }
          });
          break;
      }

    }

    void handshake(securityConfg_t securityType,syncType_t handshakeType)
    {
        switch(handshakeType)
        {
            case(sync_t):
                node.handshake(stream_base::client);
                /* should we handle error*/
                break;
            case(syncType_t::async_t):

                node.async_handshake(stream_base::server, 
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

    void receiveData(syncType_t readType)
    {
        

        switch(readType)
        {
            case (syncType_t::sync_t):
                node.read_some(boost::asio::buffer(data_));
                /* should we handle error*/
                cout<<data_;
            break;

            case(syncType_t::async_t):
            auto self(shared_from_this());
            node.async_read_some(boost::asio::buffer(data_),
                [this, self](const boost::system::error_code& ec, std::size_t length)
                {
                    if (!ec)
                    {
                    std::cout<< "RECEIVE DATA DONE \n ";
                    }
                });
            break;

        }

    }

    /*send data as string*/

    void sendData(syncType_t writeType,string data)
    {
        switch(writeType)
        {
            case (syncType_t::sync_t):
                cout<<data<<endl;
                node.write_some(boost::asio::buffer(data,data.length()));
                /* should we handle error*/
            break;

            case(syncType_t::async_t):
            {
                auto self(shared_from_this());
                node.async_write_some(boost::asio::buffer(data,data.length()),
                [this, self](const boost::system::error_code& ec, std::size_t length)
                {
                    if (!ec)
                    {
                    std::cout<< "Sending DATA DONE \n ";
                    }
                });
            }
            break;

        }

    }

    boost::asio::ssl::stream<tcp::socket> node;
    char request_[max_length];
    char data_[max_length];
  };




  int main()
  {
    try
    {
      boost::asio::io_context io_context;

      boost::asio::ip::tcp::resolver resolver(io_context);

      boost::asio::ip::tcp::resolver::results_type endpoints =
        resolver.resolve("127.0.0.1", "1234");

      boost::asio::ssl::context ctx(boost::asio::ssl::context::tls);
      ctx.load_verify_file("../Certificates/server.crt");

      client c(io_context, ctx, endpoints,tls,sync_t);
      c.sendData(sync_t,"Shams & Tabakh");
      c.receiveData(sync_t);

      io_context.run();


    }
    catch (std::exception& e)
    {
      std::cerr << "Exception: 1" << e.what() << "\n";
    }

    return 0;
  }