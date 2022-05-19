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
    std::mutex mtx;

public:
    typedef enum {exec,sm,iam,ucm,ota,sd} exe_name;
    simulation(int port);
    void creat_socket();
    void connect_to_socket();
    void send_file(char *file_path);
    exe_name recive_exe_name(int client_socket);
    bool recive_file(int client_socket,exe_name name);
    int listen_l();
    ~simulation();
};
