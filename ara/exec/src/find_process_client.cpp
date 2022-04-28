#include "../include/find_process_client.hpp"

using namespace ara::exec;


FindProcessClient::FindProcessClient() : io_service(), socket_(io_service)
{
    socket_.connect(tcp::endpoint(boost::asio::ip::address::from_string(EM_IP_ADDRESS), EM_PORT_NUMBER));
    cout<< "[iam] " << "iam connected to em....."<<endl;

}

void FindProcessClient::sendData(int pid_)
{
    const string pid = to_string(pid_);
    boost::asio::write(socket_, boost::asio::buffer(pid));
}

string FindProcessClient::receiveData()
{
    boost::asio::streambuf buf;
    boost::asio::read_until( socket_, buf, "\0" );
    return boost::asio::buffer_cast<const char*>(buf.data());
}

FindProcessClient::~FindProcessClient()
{
    socket_.close();
}