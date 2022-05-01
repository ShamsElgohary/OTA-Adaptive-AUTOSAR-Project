#include "../include/state_client.hpp"
using namespace std ;
using namespace ara::exec ;

bool StateClient::setState(const FunctionGroupState &state){
    
    this->fd = open("smFifo", O_WRONLY);
    int size_fg_name =state.fg_name.size();
    int size_newState =state.fg_newState.size();
    bool result;

    cout << "[sm] " << "setting new state from sm\n";
    std::cout<<state.fg_name<<std::endl;
    std::cout<<state.fg_newState<<std::endl;

    write(fd, &size_fg_name, sizeof(int));
    write(fd, state.fg_name.c_str(), size_fg_name+1);

    write(fd, &size_newState, sizeof(int));
    write(fd, state.fg_newState.c_str(), size_newState+1);

    read(fd,&result,sizeof(result));
    close(this->fd); 

    return result;
}


StateClient::StateClient()
{
    
}
StateClient::~StateClient()
{
}