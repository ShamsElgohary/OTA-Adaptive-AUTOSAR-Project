#include "../include/execution_client.hpp"
#include <boost/filesystem.hpp>

using namespace std ;
using namespace boost::filesystem;
using namespace ara::exec ;

ExecutionClient::ExecutionClient()
{
    

}

void ExecutionClient::ReportExecutionStaste(ExecutionState state)
{
    cout<<"trying to open pipe from exuctable\n" ;
    this->fd = open("executablesFifo", O_WRONLY);
    cout<<"opened pipe from exuctable\n" ;
    cout<<"writing to pipe from exuctable\n" ;
    write(fd, &state, sizeof(state));
    close(this->fd);

}

ExecutionClient::~ExecutionClient(){
}

