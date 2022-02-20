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
    this->current_state = ProcessState::Kidle;
    configuration_ = token.configration;
    name  = token.name;
    executable_path =token.executable_path;
}

int Application::start()
{
    this->id =fork();
    if(id !=0 && mkfifo(to_string(id).c_str(), 0666)==-1)
    {
        cout<<"couldn't create fifo for excutable "<< this->name;
    }
    if(this->id ==0)
    {
        execl(executable_path.c_str(),nullptr);
    }
     return id ;
}

void Application::terminate()
{

    kill(id,SIGTERM);
    Update_status();
    if(this->current_state!=ProcessState::Kterminate){
       //wait time in microseconds
        usleep(100000);
        kill(id,SIGKILL);
    }
    unlink(this->fifo_path_name.c_str());

}

Application::CtorToken Application::preconstruct(ApplicationManifest &ex,string fg_name,string fg_state)
{
    Application::CtorToken token;
         for (auto &x : ex.startUpConfigurations)
            {
                if(fg_name=="machineState")
                {
                   for (auto &y:x.machine_states)
                        {
                            if(y==fg_state)
                            {
                                token.name = ex.name ;
                                token.executable_path = ex.executable_path ;
                                token.configration=x;
                                return token;
                            }
                        }
                }
               else{ 
                    auto it = x.function_group_states.find(fg_name);
                    if(it==x.function_group_states.end())
                    { }
                    else
                    {
                        for (auto &y:x.function_group_states[fg_name])
                        {
                            if(y==fg_state)
                            {
                                token.name = ex.name ;
                                token.executable_path = ex.executable_path ;
                                token.configration=x;
                                return token;

                            }
                        }
                    }
               }
            }
}


void Application::Update_status()
{   
    int fd = open(to_string(id).c_str(),O_RDONLY);
    char state ;
    read(fd,&state,sizeof(state));
    current_state = Application::ProcessState(state) ;
    close(fd);
}
Application::Application(ApplicationManifest::startUpConfiguration con, string name , string path)
{
    configuration_ = con ;
    this->name = name ;
    executable_path = path+"/"+name ;
    current_state = ProcessState::Kidle;
}