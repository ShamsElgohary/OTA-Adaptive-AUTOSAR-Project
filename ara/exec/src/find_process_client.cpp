#include "../include/find_process_client.hpp"

using namespace ara::em ;

FindProcessClient::FindProcessClient()
{
    server_socket  = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket <0)
    {
        cout<<"client establishing connection..." ;
        return ;
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP.c_str());
    address.sin_port = htons(portNum);

    if(connect(server_socket ,(struct sockaddr *)&address, sizeof(address))<0)
    {
        cout<<"error while connecting to server...\n" ;
    }
}

void FindProcessClient::sendData(int pid)
{
    send(server_socket , &pid, sizeof(int) , 0 );
}

string FindProcessClient::receiveData()
{
    recv(server_socket , buff , BUFF_SIZE , 0 ) ;
    return string(buff);
}

FindProcessClient::~FindProcessClient()
{
    close(server_socket) ;
}