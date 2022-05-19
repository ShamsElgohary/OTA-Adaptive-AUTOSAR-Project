#include "simulation.hpp"
#include "thread"
#include "iostream"
#include <QApplication>
#include "mainwindow.h"

simulation::simulation(int port)
{
    ip = "127.0.0.1";
    port_num = port;
}

simulation::~simulation()
{
    close(sockfd_c);
    close(sockfd_s);

}

void simulation::creat_socket()
{
    struct sockaddr_in server_addr;
    char buffer[SIZE];

    sockfd_s = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_s < 0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created. \n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port_num;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = bind(sockfd_s, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e < 0)
    {
        perror("[-]Error in Binding");
        exit(1);
    }
    printf("[+]Binding Successfull.\n");
}

int simulation::listen_l()
{
    struct sockaddr_in new_addr;
    socklen_t addr_size;

    e = listen(sockfd_s, 10);
    if (e == 0)
    {
        printf("[+]Listening...\n");
    }
    else
    {
        perror("[-]Error in Binding");
        exit(1);
    }
    addr_size = sizeof(new_addr);

    return accept(sockfd_s, (struct sockaddr *)&new_addr, &addr_size);
}
simulation::exe_name simulation::recive_exe_name(int client_socket)
{
    exe_name name ;
    recv(client_socket, &name, sizeof(exe_name),0);
    return name;
}
bool simulation::recive_file(int client_socket,exe_name name)
{
    int n;
    string file_name;
    bool succes;
    switch(name)
    {
        case (simulation::exe_name::exec) : file_name = "em.json";
    }
    if (recv(client_socket, &n, sizeof(n), 0)<=0){
        close(client_socket);
        return false;
    }
    ofstream file;
    file.open(file_name);
    file.clear();
    char  buffer [SIZE];
    while(1)
    {
        if(n==0)break;
        if (recv(client_socket, buffer, n * sizeof(char) , 0)<=0)
        {
            file.close();
            close(client_socket);
            return false;
        }
        file<<buffer;
        if (recv(client_socket, &n, sizeof(n), 0)<=0)
        {
            file.close();
            close(client_socket);
            return false;
        }
    }
    file.flush();
    file.close();
    return true;
}

void simulation::connect_to_socket()
{
    struct sockaddr_in server_addr;
    sockfd_c = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_c < 0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created. \n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port_num;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd_c, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e == -1)
    {
        perror("[-]Error in Connecting");
        exit(1);
    }
}

void simulation::send_file(char *file_path)
{
    ifstream fp;
        fp.open(file_path);
        string data;
        int size =0;
        while (!fp.eof())
        {
            data="";
            fp >> data;
            size =data.size();
            send(sockfd_c, &size, sizeof(int), 0);
            send(sockfd_c, data.c_str(), sizeof(char)*size, 0);
        }
        size =0;
        send(sockfd_c, &size, sizeof(int), 0);
}
