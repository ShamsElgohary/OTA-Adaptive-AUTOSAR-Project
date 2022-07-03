#include "../include/simulation.hpp"
#include "thread"
#include "iostream"

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
    exe_name name;
    recv(client_socket, &name, sizeof(int), 0);
    return name;
}
void simulation::send_exe_name(simulation::exe_name name)
{
    send(sockfd_c, &name, sizeof(int), 0);
}
char *simulation::recive_file(int client_socket)
{
    char *buffer = new char[SIZE];
    recv(client_socket, buffer, SIZE, 0);
    std::cout << "file recieved " << std::endl;
    string file = buffer;
    return buffer;
}

void simulation::connect_to_socket()
{
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename = "execution_manifest.json";

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
    int size = 0;
    while (getline(fp, data))
    {
        data += "\0";
        size = data.size() + 1;
        send(sockfd_c, &size, sizeof(int), 0);
        send(sockfd_c, data.c_str(), sizeof(char) * size, 0);
    }
    fp.close();
    size = 0;
    send(sockfd_c, &size, sizeof(int), 0);
}
