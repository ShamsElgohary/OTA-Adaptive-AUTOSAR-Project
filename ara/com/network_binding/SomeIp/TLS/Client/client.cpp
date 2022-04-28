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



  class client
  {
  public:
    client(boost::asio::io_context& io_context,
        boost::asio::ssl::context& context,
        const tcp::resolver::results_type& endpoints)
      : socket_(io_context, context)
    {
      socket_.set_verify_mode(boost::asio::ssl::verify_peer);

      // SET CALL BACK, CALL VERIFY CERTIFICATE WHICH IS A FUNCTION OF THIS GIVING IT 2 PARAMETERS
      socket_.set_verify_callback(
          std::bind(&client::verify_certificate, this, _1, _2));

      connect(endpoints);
    }

  public:
    bool verify_certificate(bool preverified,
        boost::asio::ssl::verify_context& ctx)
    {
      // The verify callback can be used to check whether the certificate that is
      // being presented is valid for the peer. For example, RFC 2818 describes
      // the steps involved in doing this for HTTPS. Consult the OpenSSL
      // documentation for more details. Note that the callback is called once
      // for each certificate in the certificate chain, starting from the root
      // certificate authority.

      char subject_name[256];
      X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
      X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
      std::cout << "Verifying " << subject_name << "\n";

      return preverified;
    }

    void connect(const tcp::resolver::results_type& endpoints)
    {
      boost::asio::async_connect(socket_.lowest_layer(), endpoints,
          [this](const boost::system::error_code& error,
            const tcp::endpoint& )
          {
            if (!error)
            {
              handshake();
            }
            else
            {
              std::cout << "Connect failed: " << error.message() << "\n";
            }
          });
    }

    void handshake()
    {
      socket_.async_handshake(boost::asio::ssl::stream_base::client,
          [this](const boost::system::error_code& error)
          {
            if (!error)
            {
              std::cout << "Handshake finished \n";
            }
            else
            {
              std::cout << "Handshake failed: " << error.message() << "\n";
            }

          });
    }

    void send_request(std::string s)
    {
      try
      {
			  socket_.async_write_some( boost::asio::buffer(s), &OnSendCompleted );
        std::cout << "DATA SENT \n";
      
      }
      catch(const std::exception& e)
      {
        std::cerr << e.what() << '\n';
      }
      
    }

    void receive_response(std::size_t length)
    {
      boost::asio::async_read(socket_,
          boost::asio::buffer(reply_, length),
          [this](const boost::system::error_code& error, std::size_t length)
          {
            if (!error)
            {
              OnReceiveCompleted(error, length);
            }
            else
            {
              std::cout << "Read failed: " << error.message() << "\n";
            }
          });
    }

    boost::asio::ssl::stream<tcp::socket> socket_;
    char request_[max_length];
    char reply_[max_length];
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

      client c(io_context, ctx, endpoints);

      c.send_request("HELLO");

      io_context.run();


    }
    catch (std::exception& e)
    {
      std::cerr << "Exception: 1" << e.what() << "\n";
    }

    return 0;
  }