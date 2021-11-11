#include "state_client.h"

using namespace std ;
using namespace ara::exec ;

bool StateClient::setState(const FunctionGroupState &state){
    
    int size_fg_name =state.fg_name.size();
    int size_newState =state.fg_newState.size();

    write(fd, &size_fg_name, sizeof(size_fg_name));
    write(fd, &state.fg_name, sizeof(state.fg_name));

    write(fd, &size_newState, sizeof(size_newState));
    write(fd, &state.fg_newState, sizeof(state.fg_newState));

    int reply;
    read(fd, &reply, sizeof(int));

    close(this->fd);
    return reply;
       
}


StateClient::StateClient(){
        this->fd = open("smFifo", O_RDWR);  //mkfifo??
}
StateClient::~StateClient()
{}