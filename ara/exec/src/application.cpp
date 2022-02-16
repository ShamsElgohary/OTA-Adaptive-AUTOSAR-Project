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
    configuration_ = &token.configration;
    name  = token.name;
    executable_path =token.executable_path;
}

int Application::start()
{
    // this->id =fork();
    // if(this->id ==0)
    // {
    //     execl(executable_path.c_str(),nullptr);
    // }
    // mkfifo(name.c_str(), 0666);
    // return id ;
}

void Application::terminate()
{

}

Application::CtorToken Application::preconstruct(ApplicationManifest &ex,string fg_name,string fg_state)
{
    Application::CtorToken token;
         for (auto &x : ex.startUpConfigurations)
            {
                if(fg_name=="Machine_fg")
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

Application::Application(ApplicationManifest::startUpConfiguration &configration ,string nama ,string path)
{
    this->current_state = ProcessState::Kidle;
    configuration_ = &configration;
    name  = name;
    executable_path =executable_path;
}