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
    cout<< "[em] " <<"reporting state from exuctable\n" ;
    write(0, &state, sizeof(state));
}

ExecutionClient::~ExecutionClient(){
    close(0);
}

