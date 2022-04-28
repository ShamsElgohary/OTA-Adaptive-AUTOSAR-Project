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
    this->fd = open("executablesFifo", O_WRONLY);
    cout<< "[em] " <<"reporting state from exuctable\n" ;
    write(fd, &state, sizeof(state));
    close(this->fd);
}

ExecutionClient::~ExecutionClient(){
}

