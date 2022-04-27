#include "../include/application.hpp"
#include <stdint.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

using namespace std ;
using namespace ara::exec ;

Application::Application(Application::CtorToken && token)
{
    current_state = ExecutionState::Kidle;
    configuration_ = token.configration;
    name  = token.name;
    executable_path =token.executable_path;
}
void Application::start()
{
    this->id =fork();
    if(this->id ==0)
    {
        execl(executable_path.c_str(),nullptr);
    }
}
void Application::terminate()
{
    if(kill(id,SIGKILL) == 0)
    {
         cout<<"terminating "<<name<<"\n\n\n";
    }
    else
    {
         cout<<"couldn't terminate process.... with id = "<<id<<" and named "<<name<<"\n\n\n";
    }

    //Update_status();
    // if(this->current_state!=ExecutionState::Kterminate){
    //    //wait time in microseconds
    //     usleep(100000);
    //     kill(id,SIGKILL);
    // }
}


void Application::Update_status()
{   
    int fd = open("executablesFifo", O_RDONLY);
    read(fd,&current_state,sizeof(current_state));
    close(fd);
    if(current_state ==ExecutionState::Krunning)
        cout<<name << " new state is "<<"Krunning"<<"\n\n\n";
}
Application::Application(ApplicationManifest::startUpConfiguration con, string name , string path)
{
    configuration_ = con ;
    this->name = name ;
    executable_path = path+"/"+name ;
    current_state = ExecutionState::Kidle;
}