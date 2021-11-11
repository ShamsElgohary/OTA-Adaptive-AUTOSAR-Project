#include "process.h"
#include <stdint.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

using namespace std ;
using namespace ara::exec ;

Process::CtorToken Process::preconstruct(Executable &ex,string fng,string state)
{
    Process::CtorToken token ;
    token.function_group_name=fng;
    token.function_group_state=state;
    token.name = ex.name;
    token.executable_path =ex.executable_path ;
    token.configration  = ex.configration[std::pair(fng, state)];
    return token;
}

Process::Process(Process::CtorToken &&token)
{
    current_state =processState::Kidel ;
    name  =  token.name;
    executable_path =  token.executable_path ;
    configration= token.configration  ;

}
int Process::run()
{
    this->id =fork();
    if(this->id ==0)
    {
        execl(executable_path.c_str(),to_string(id).c_str(),configration.arguments[0].c_str(),nullptr);
    }
    mkfifo(name.c_str(), 0666);
    return id ;
}

void Process::update_state()
{
    int fd = open(name.c_str(), O_RDWR); 
    read(fd, &current_state, sizeof(uint8_t));
    close(fd);
    unlink(name.c_str());
}
void Process::terminate()
{

}