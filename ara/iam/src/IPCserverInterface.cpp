#include "../include/IPCserverInterface.hpp"

ara::iam::IPCserverInterface::IPCserverInterface() : io_service(), acceptor_(io_service, tcp::endpoint(tcp::v4(), IAM_PORT_NUMBER)), socket_(io_service) {}
std::uint8_t ara::iam::IPCserverInterface::ServerSocketInit()
{
    cout << "iam listening for requests....." << endl;
    try
    {
        acceptor_.accept(socket_);
    }
    catch (boost::system::system_error e)
    {
        std::cout << "iam ServerSocketInit : " << e.what() << endl;
    }
}


int ara::iam::IPCserverInterface::Listen()
{
    std::cout << "[iamServer] Waiting for a client to connect..." << std::endl;
    try
    {
        acceptor_.accept(socket_);
    }
    catch (boost::system::system_error e)
    {
        std::cout << "iam Listen : " << e.what() << endl;
    }
}

void ara::iam::IPCserverInterface::Send(bool is_granted, int sd)
{
    string s = (is_granted ? "1" : "0");
    try
    {
        cout<<"iam Send : "<<s<<endl;
        socket_.write_some(boost::asio::buffer(s,1));
    }
    catch (boost::system::system_error e)
    {
        std::cout << "iam Send error  : " << e.what() << endl;
    }
}

ara::iam::Grant ara::iam::IPCserverInterface::Receive(int sd)
{
    ara::iam::Grant G;
    try
    {
        boost::asio::streambuf buf;
        
        boost::asio::read_until(socket_, buf, "\0");
        stringstream ss(boost::asio::buffer_cast<const char *>(buf.data()));
        string s(boost::asio::buffer_cast<const char *>(buf.data()));

        cout<< "iam Receive: " << s <<endl;
        // Create GrantObj
        // Deserialize stringstream ss in GrantObj
        boost::serialization::Deserialize(G, ss);
    }
    catch (boost::system::system_error e)
    {
        std::cout << "iam Receive error: " << e.what() << endl;
    }
    return G;
}
ara::iam::IPCserverInterface::~IPCserverInterface()
{
    socket_.close();
}