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

public:
    typedef enum {exec,sm,iam,ucm,ota,sd} exe_name;
    simulation(int port);
    void creat_socket();
    void connect_to_socket();
    void send_file(char *file_path);
    void send_exe_name(simulation::exe_name name);
    exe_name recive_exe_name(int client_socket);
    char* recive_file(int client_socket);
    int listen_l();
    ~simulation();
};