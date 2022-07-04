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
#include <boost/asio.hpp>

using namespace std;
#define SIZE 5000
using namespace boost::asio;
using ip::tcp;
class simulation
{
private:
    char *ip;
    int port_num;
    boost::asio::io_service *io_service;
    tcp::acceptor *acceptor_;
    tcp::socket* socket_;

public:
    typedef enum
    {
        exec,
        sm,
        iam,
        ucm,
        ota,
        sd
    } exe_name;
    simulation(int port);
    void connect_to_socket();
    void send_file(char *file_path);
    void send_exe_name(simulation::exe_name name);
    void create_server();
    simulation::exe_name recive_exe_name(tcp::socket* client_socket);
    bool recive_file(tcp::socket* client_socket,exe_name name);
    tcp::socket* listen_l();
    ~simulation();
};
