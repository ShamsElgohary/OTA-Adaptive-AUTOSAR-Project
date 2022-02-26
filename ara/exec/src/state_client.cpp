#include "../include/state_client.hpp"
using namespace std ;
using namespace ara::exec ;

bool StateClient::setState(const FunctionGroupState &state){
    
    int size_fg_name =state.fg_name.size();
    int size_newState =state.fg_newState.size();


    cout<<"writing to pipe from sm\n";
    write(fd, &size_fg_name, sizeof(int));
    write(fd, state.fg_name.c_str(), size_fg_name+1);

    write(fd, &size_newState, sizeof(int));
    write(fd, state.fg_newState.c_str(), size_newState+1);
}


StateClient::StateClient()
{
    cout<<"trying to open pipe from sm \n";
    this->fd = open("smFifo", O_WRONLY);
    cout<<"pipe opened from sm\n";
}
StateClient::~StateClient()
{
    close(this->fd);        
}