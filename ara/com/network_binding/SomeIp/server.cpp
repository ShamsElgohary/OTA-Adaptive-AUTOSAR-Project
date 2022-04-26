
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

//////////////////////////


  /* USED AS A HANDLE IN THE SOCKET FOR ASYNCHRONOUS MESSAGES */
void OnSendCompleted(boost::system::error_code ec, size_t bytes_transferred) {
  if (ec)
      std::cout << "Send failed: " << ec.message() << "\n";
  else
      std::cout << "Send succesful (" << bytes_transferred << " bytes)\n";
}

    /* USED AS A HANDLE IN THE SOCKET FOR ASYNCHRONOUS MESSAGES */
void OnReceiveCompleted(boost::system::error_code ec, size_t bytes_transferred) {
  if (ec)
      std::cout << "Receive Failed: " << ec.message() << "\n";
  else
      std::cout << "Receive Successful (" << bytes_transferred << " bytes)\n";
}


/////////////////////


using boost::asio::ip::tcp;
typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

////////////////////////////


class session : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket, boost::asio::ssl::context& context)
    : socket_(std::move(socket), context)
  {
  }

  void start()
  {
    Handshake();
  }


private:

  void Handshake()
  {
    auto self(shared_from_this());
    socket_.async_handshake(boost::asio::ssl::stream_base::server, 
        [this, self](const boost::system::error_code& error)
        {
          if (!error)
          {
            do_read();
          }
        });
  }

  // void do_read()
  // {
  //   auto self(shared_from_this());
  //   socket_.async_read_some(boost::asio::buffer(data_), OnReceiveCompleted);
    
  // }

  void do_read()
  {
    // shared ptr of this (to prevent destructor)
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_),
        [this,self](const boost::system::error_code& ec, std::size_t length)
        {
          if (!ec)
          {
            do_write(length);
          }
        });
  }


  void do_write(std::size_t length)
  {
    // used for async operations to give lifetime inside lambda
    auto self(shared_from_this());

    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
        [this, self](const boost::system::error_code& ec,
          std::size_t /*length*/)
        {
          if (!ec)
          {
            do_read();
          }
        });
  }

  boost::asio::ssl::stream<tcp::socket> socket_;
  char data_[1024];
};

  
class server
{
public:
  server(boost::asio::io_context& io_context, unsigned short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      context_(boost::asio::ssl::context::tls)
  {
    context_.set_options(
        boost::asio::ssl::context::default_workarounds
        |boost::asio::ssl::context::no_sslv2
        | boost::asio::ssl::context::single_dh_use);
    context_.set_password_callback(boost::bind(&server::get_password, this));
    context_.use_certificate_chain_file("../Certificates/server.crt");
    context_.use_private_key_file("../Certificates/server.key", boost::asio::ssl::context::pem);
    context_.use_tmp_dh_file("../Certificates/dh2048.pem");

    do_accept();
  }

private:
  std::string get_password() const
  {
    return "test";
  }

  void do_accept()
  {
    acceptor_.async_accept(
        [this](const boost::system::error_code& error, boost::asio::ip::tcp::socket socket)
        {
          if (!error)
          {
            std::make_shared<session>(std::move(socket), context_)->start();
          }

          do_accept();
        });
  }

  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ssl::context context_;
};


int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_context io_context;

    server s(io_context, 1234);

    io_context.run();
  }
  
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
