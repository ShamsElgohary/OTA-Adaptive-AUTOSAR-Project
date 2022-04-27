#include "../include/find_process_server.hpp"
using namespace ara::exec;

FindProcessServer::FindProcessServer():io_service() ,acceptor_(io_service, tcp::endpoint(tcp::v4(), EM_PORT_NUMBER)),socket_(io_service)
{
    cout<<"em listening for iam requests....."<<endl;
    acceptor_.accept(socket_);
}


int FindProcessServer::receiveData()
{
    boost::asio::streambuf buf;
    boost::asio::read_until( socket_, buf, "\0" );
    stringstream geek( boost::asio::buffer_cast<const char*>(buf.data()));
    int data=0;
    geek>>data;
    return data;
}

void FindProcessServer::sendData(string processName)
{
    const string msg = processName;
    boost::asio::write( socket_, boost::asio::buffer(msg) );
}
FindProcessServer::~FindProcessServer()
{
    socket_.close();
}
