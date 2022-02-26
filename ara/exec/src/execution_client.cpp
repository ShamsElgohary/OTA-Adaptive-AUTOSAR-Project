#include "../include/execution_client.hpp"
#include <boost/filesystem.hpp>

using namespace std ;
using namespace boost::filesystem;
using namespace ara::exec ;

ExecutionClient::ExecutionClient()
{
    int id =getpid();
    string  s = to_string(id) ;
    cout<<"trying to open pipe from exuctable\n" ;
    this->fd = open(s.c_str(), O_WRONLY);
    cout<<"opened pipe from exuctable\n" ;

}

void ExecutionClient::ReportExecutionStaste(ExecutionState state)
{
    cout<<"writing to pipe from exuctable\n" ;
    write(fd, &state, sizeof(state));
}

ExecutionClient::~ExecutionClient(){
        close(this->fd);
}

