#pragma once
#include "../include/find_process_server.hpp"

using namespace std ;
using namespace ara::em ;


FindProcessServer::FindProcessServer()
{
    server_socket  = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket <0)
    {
        cout<<"error establishing connection..." ;
        return ;
    }

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP.c_str());
    address.sin_port = htons(portNum);

    if(bind(server_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        cout<<"error while binding to connection...\n" ;
    }

    listen(server_socket, 1);
    client_socket = accept(server_socket, NULL, NULL);
    
    if(client_socket<0)
    {
        cout<<"error in client socket...\n" ;
    }
}

int FindProcessServer::receiveData()
{
    recv(client_socket , &pid , sizeof(int) , 0 ) ;
    return pid;
}

void FindProcessServer::sendData(string processName)
{
    send(client_socket , processName.c_str(), size(processName)+1 , 0 ) ;
}
FindProcessServer::~FindProcessServer()
{
    close(client_socket) ;
    close(server_socket) ;
}
