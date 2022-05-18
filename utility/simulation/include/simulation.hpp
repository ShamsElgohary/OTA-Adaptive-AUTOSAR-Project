#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>
#include <mutex>
#include "functional"
using namespace std;
#define SIZE 1024

class simulation
{
private:
    char *ip;
    int port_num;
    int e;
    int sockfd_s;
    int sockfd_c;
    int new_sock;
    std::mutex mtx;

public:
    simulation(int port);
    void creat_socket();
    void connect_to_socket();
    void send_file(char *file_path);
    void recive_file(int client_socket,std::function<void()>handler);
    void listen_l(std::function<void()>handler);
    ~simulation();
};

    void ClearJSONReport();
