#include "../include/state_client.hpp"

using namespace std ;
using namespace ara::exec ;

bool StateClient::setState(const FunctionGroupState &state){
    
    int size_fg_name =state.fg_name.size();
    int size_newState =state.fg_newState.size();

    write(fd, &size_fg_name, sizeof(int));
    write(fd, &state.fg_name, size_fg_name);

    write(fd, &size_newState, sizeof(int));
    write(fd, &state.fg_newState, size_newState);

    close(this->fd);       
}


StateClient::StateClient()
{
    int id =getpid();
    string  s = to_string(id) ;
    this->fd = open(s.c_str(), O_WRONLY);
}
StateClient::~StateClient()
{

}