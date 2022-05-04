#include "../include/simulation.hpp"

simulation::simulation(int port)
{
    ip = "127.0.0.1";
    port_num = port;
}

simulation::~simulation()
{
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

void simulation::listen_l()
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
    new_sock = accept(sockfd_s, (struct sockaddr *)&new_addr, &addr_size);
}

void simulation::recive_file()
{
    ofstream f("file2.json");
    char buffer[SIZE];

    int n;
    while (1)
    {
        n = recv(new_sock, buffer, SIZE, 0);
        if (n <= 0)
        {
            break;
            return;
        }
        f << buffer;
        bzero(buffer, SIZE);
    }
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
    FILE *fp;
    fp = fopen(file_path, "r");
    if (fp == NULL)
    {
        perror("[-]Error in reading file.");
        exit(1);
    }
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(sockfd_c, data, sizeof(data), 0) == -1)
        {
            perror("[-] Error in sendung data");
            exit(1);
        }
        bzero(data, SIZE);
    }
}
