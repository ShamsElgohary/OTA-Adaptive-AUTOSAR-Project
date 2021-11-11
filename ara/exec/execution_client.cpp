#include "execution_client.h"


using namespace std ;
using namespace boost::filesystem;
using namespace ara::exec ;

ExecutionClient::ExecutionClient()
{
    path pro (current_path());    
    this->fd = open(pro.stem(), O_WRONLY);
}

void ExecutionClient::ReportExecutionStaste(ExecutionState state)
{
    write(fd, &state, sizeof(state));
    close(this->fd);

}

