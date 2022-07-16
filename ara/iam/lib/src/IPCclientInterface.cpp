#include "../include/IPCclientInterface.hpp"

ara::iam::IPCclientInterface::IPCclientInterface() : io_service(), socket_(io_service) {}
std::uint8_t ara::iam::IPCclientInterface::Connect()
{
    try
    {
        socket_.connect(tcp::endpoint(boost::asio::ip::address::from_string(IAM_IP_ADDRESS), IAM_PORT_NUMBER));
        
    }
    catch (boost::system::system_error e)
    {
        std::cout << "iam connect to socket : " << e.what() << endl;
        return 0;
    }
    return 1;
}
void ara::iam::IPCclientInterface::Send(ara::iam::Grant G)
{
    std::stringstream ss;
    try
    {
        // Serialize GrantObj 1 in stringstream ss
        boost::serialization::Serialize(G, ss);
        std::string data = ss.str();
        boost::asio::write(socket_, boost::asio::buffer(data));
    }
    catch (boost::system::system_error e)
    {
        std::cout << "iam client : " << e.what() << endl;
    }
}


bool ara::iam::IPCclientInterface::Receive()
{
    bool is_granted = false;
    try
    {
        std::cout << "[iamClient] "
                  << "Awaiting server response..." << std::endl;
        char buf[1];
        socket_.read_some(boost::asio::buffer(buf,1));
        cout<<"[iamClient] Result: "<<buf<<endl;
        is_granted = (buf[0] == '1') ? true : false;
        std::cout << "[iamClient] Result: " << is_granted << std::endl;
    }
    catch (boost::system::system_error e)
    {
        std::cout << "iam Receive error: " << e.what() << endl;
    }
    return is_granted;
}
ara::iam::IPCclientInterface::~IPCclientInterface()
{
    socket_.close();
}