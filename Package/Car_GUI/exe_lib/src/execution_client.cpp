#include "../include/execution_client.hpp"
#include <boost/filesystem.hpp>

using namespace std ;
using namespace boost::filesystem;
using namespace ara::exec ;

ExecutionClient::ExecutionClient()
{
    std::cout << "Execution Client Created" << std:: endl;

}

void ExecutionClient::ReportExecutionStaste(ExecutionState state)
{
    write(0, &state, sizeof(state));
}

ExecutionClient::~ExecutionClient(){
    close(0);
}

