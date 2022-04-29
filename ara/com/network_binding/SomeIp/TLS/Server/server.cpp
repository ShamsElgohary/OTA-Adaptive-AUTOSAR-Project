#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

using namespace std;
using stream_base = boost::asio::ssl::stream_base;
using tcp = boost::asio::ip::tcp;
enum { max_length = 1024 };



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
    session(tcp::socket socket, boost::asio::ssl::context& context)
        : node(std::move(socket), context)
        {
        }



    void handshake(securityConfg_t securityType,syncType_t handshakeType)
    {
        auto self(shared_from_this());
        switch(handshakeType)
        {
            case(syncType_t::sync_t):
                node.handshake(stream_base::server);
                break;
            case(syncType_t::async_t):

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

    string receiveData(syncType_t readType)
    {
        
        std::size_t x = 0;
        switch(readType)
        {
            case (syncType_t::sync_t):

                x = node.read_some(boost::asio::buffer(data_));
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
        data_[x] = '\0';

        return data_;

    }

    /*send data as string*/

    void sendData(syncType_t writeType, string data)
    {
        switch(writeType)
        {
            case (syncType_t::sync_t):
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
    private:
    boost::asio::ssl::stream<tcp::socket> node;
    char data_[max_length];
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

      string receiveData(syncType_t readType)
    {
        string x = session_instance->receiveData(readType);
        return x;
    }

    /*send data as string*/

    void sendData(syncType_t writeType, string data)
    {
         session_instance->sendData(writeType,data);
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
        case (syncType_t::sync_t):

        /*GET SOCKET FROM ACCEPT TO CONSTRACUT SESSION*/
            session_instance = std::make_shared<session>(std::move(acceptor_.accept()), context_);
            session_instance->handshake(tls,acceptorType);
            break;

        case (syncType_t::async_t):
            acceptor_.async_accept(
            [this,acceptorType](const boost::system::error_code& error, boost::asio::ip::tcp::socket socket)
            {
            if (!error)
            {
            session_instance = std::make_shared<session>(std::move(socket), context_);
            session_instance->handshake(tls,acceptorType);
            }

            
            });
          break;

      }
      //acceptConnecation(acceptorType);
  }

  
    shared_ptr<session> session_instance = nullptr;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ssl::context context_;
};



int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_context io_context;

    server s(io_context, 1234,tls,sync_t);

    string x = s.receiveData(sync_t);
    cout<<x<<endl;
    s.sendData(sync_t,"recivedasssssssssssssssssssssssssssssssssssssssssssss");
    string y = s.receiveData(sync_t);
    cout<<y<<endl;
    s.sendData(sync_t,"recivedagain");

    io_context.run();
  }
  
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
