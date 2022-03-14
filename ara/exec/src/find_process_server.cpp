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
    cout<<"server socket created..." ;

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portNum);

    bind(server_socket, (struct sockaddr *)&address, sizeof(address));

    listen(server_socket, 1);
    client_socket = accept(server_socket, NULL, NULL);
}

int FindProcessServer::receiveData()
{
    recv(client_socket , &pid , size(int) , 0 ) ;
    return pid;
}

void FindProcessServer::sendData(string processName)
{
    send(client_socket , processName.c_str(), size(processName)+1 , 0 ) ;
    return pid;
}