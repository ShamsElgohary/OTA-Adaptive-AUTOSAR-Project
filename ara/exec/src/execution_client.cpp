#include "../include/execution_client.hpp"
#include <boost/filesystem.hpp>

using namespace std ;
using namespace boost::filesystem;
using namespace ara::exec ;

ExecutionClient::ExecutionClient()
{
    path pro (current_path());    
    this->fd = open(pro.stem().c_str(), O_WRONLY);
}

void ExecutionClient::ReportExecutionStaste(ExecutionState state)
{
    write(fd, &state, sizeof(state));
    close(this->fd);
}

ExecutionClient::~ExecutionClient(){}

