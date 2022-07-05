#include "simulation.hpp"
#include "thread"
#include "iostream"

simulation::simulation(int port)
{
    ip = "127.0.0.1";
    port_num = port;
}

simulation::~simulation()
{
    if(socket_!=nullptr && socket_->is_open())
    {
        socket_->close();
    }
    delete socket_;
    delete io_service;
    delete acceptor_;
}

void simulation::connect_to_socket()
{
    io_service = new boost::asio::io_service();
    socket_ = new tcp::socket(*io_service);
    try {
        socket_->connect(tcp::endpoint(boost::asio::ip::address::from_string(ip), port_num));
    }
    catch ( boost::system::system_error e)
    {
        std::cout<<"connect to socket : " <<e.what()<<endl;
    }
}
void simulation::create_server()
{
    io_service = new boost::asio::io_service();
    try{
        acceptor_ = new tcp::acceptor(*io_service, tcp::endpoint(tcp::v4(), port_num));
    }
    catch ( boost::system::system_error e)
    {
        std::cout<<"create server :"<<e.what()<<endl;
        system("pkill -x em");
        system("pkill -x sm");
        system("pkill -x iam");
        system("pkill -x ota");
        system("pkill -x someip_sd");
        system("pkill -x ucm");
        system("pkill -x ex1");
        system("pkill -x ex2");
        acceptor_ = new tcp::acceptor(*io_service, tcp::endpoint(tcp::v4(), port_num));
    }
}

tcp::socket *simulation::listen_l()
{
    tcp::socket *socket_ = new tcp::socket{*io_service};
    try{
        acceptor_->accept(*socket_);
    }
    catch (boost::system::system_error e)
    {
        std::cout<<"listen for requests: " <<e.what()<<endl;
    }
    return socket_;
}
simulation::exe_name simulation::recive_exe_name(tcp::socket *client_socket)
{
    boost::asio::streambuf buf;
    try{
        boost::asio::read_until(*client_socket, buf, "\0");
    }
    catch (boost::system::system_error e)
    {
        std::cout <<"recive exe name : "<<e.what()<<endl;
    }
    stringstream s(boost::asio::buffer_cast<const char *>(buf.data()));
    int x;
    s >> x;
    return simulation::exe_name(x);
}
void simulation::send_exe_name(simulation::exe_name name)
{
    try{
        boost::asio::write(*socket_, boost::asio::buffer(to_string(name)));
    }
    catch (boost::system::system_error e)
    {
        std::cout<<"send exe name :"<<e.what()<<endl;
    }
}
bool simulation::recive_file(tcp::socket *client_socket, exe_name name)
{
        string s;
        char buf[1];
        try{
            while(1)
            {
                client_socket->read_some(boost::asio::buffer( buf ));
                if(buf[0] == '\0')  {
                    try{
                        client_socket->read_some(boost::asio::buffer( buf ));
                    }
                    catch (boost::system::system_error e)
                    {
                        std::cout <<e.what()<<endl;
                    }
                    break;}
                s += buf[0];
            }
        }
        catch (boost::system::system_error e)
        {
            std::cout<<"recive_file: "<<e.what()<<endl;
            return false;
        }
        string file_name = to_string(name);
        ofstream file;
        file.open(file_name);
        file.clear();
        file << s;
        file.flush();
        file.close();
        return true;

}

void simulation::send_file(char *file_path)
{
    ifstream fp;
    fp.open(file_path);
    string data, file;
    while (getline(fp, data))
    {
        file += data+"\n";
    }
    string x;
    try{
        for(auto c :file )
        {
            x="";
            x+=c;
            socket_->write_some(boost::asio::buffer(x));

        }
        socket_->write_some(boost::asio::buffer("\0"));
    }
    catch (boost::system::system_error e)
    {
        std::cout<<"send file : " <<e.what()<<endl;
    }
    fp.close();
}