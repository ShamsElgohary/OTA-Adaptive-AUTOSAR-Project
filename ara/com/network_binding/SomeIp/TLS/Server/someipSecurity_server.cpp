#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

using namespace std;
using stream_base = boost::asio::ssl::stream_base;
using tcp = boost::asio::ip::tcp;



typedef enum syncType_t{
    sync,
    async
}syncType_t;

typedef enum securityConfg_t{
    tls,
}securityConfg_t;


class session :public std::enable_shared_from_this<session>
{
    public:
    session(tcp::socket socket, boost::asio::ssl::context& context)
        : node(std::move(socket), context)
        {
        }



    void handshake(securityConfg_t securityType,syncType_t handshakeType)
    {
        auto self(shared_from_this());
        switch(handshakeType)
        {
            case(sync):
                node.handshake(stream_base::server);
                /* should we handle error*/
                break;
            case(syncType_t::async):

                node.async_handshake(stream_base::server, 
                [this, self](const boost::system::error_code& error)
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
        auto self(shared_from_this());

        switch(readType)
        {
            case (sync):
                node.read_some(boost::asio::buffer(data_));
                /* should we handle error*/
            break;

            case(syncType_t::async):
                node.async_read_some(boost::asio::buffer(data_),
                [this, self](const boost::system::error_code& error)
                {
                if (!error)
                {
                    std::cout<< "RECEIVE DATA DONE \n ";
                }
                });
            break;

        }

    }

    /*send data as string*/

    void sendData(syncType_t writeType, std::size_t length)
    {
        switch(writeType)
        {
            case (sync):
                node.write_some(boost::asio::buffer(data_,length));
                /* should we handle error*/
            break;

            case(syncType_t::async):
            {
                auto self(shared_from_this());
                node.async_write_some(boost::asio::buffer(data_, length),
                [this, self](const boost::system::error_code& error)
                {
                if (!error)
                {
                    std::cout<< "SENT DATA DONE \n ";
                }
                });
            }
            break;

        }

    }
    private:
    boost::asio::ssl::stream<tcp::socket> node;
    char data_[1024];
};


class server
{
public:
  server(boost::asio::io_context& io_context, unsigned short port,securityConfg_t securityType,syncType_t acceptorType)
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

    acceptConnecation(acceptorType);
  }

private:
//   std::string get_password() const
//   {
//     return "test";
//   }

  void acceptConnecation(syncType_t acceptorType)
  {

      switch (acceptorType)
      {
        case sync:

        /*GET SOCKET FROM ACCEPT TO CONSTRACUT SESSION*/
        std::make_shared<session>(std::move(acceptor_.accept()), context_)->handshake(tls,acceptorType);
          break;

        case (syncType_t::async):
            acceptor_.async_accept(
            [this,acceptorType](const boost::system::error_code& error, boost::asio::ip::tcp::socket socket)
            {
            if (!error)
            {
                std::make_shared<session>(std::move(socket), context_)->handshake(tls,acceptorType);
            }

            //do_accept();
            });
          break;

      }
    

  }

  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ssl::context context_;
};



int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_context io_context;

    server s(io_context, 1234,tls,sync);

    io_context.run();
  }
  
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}