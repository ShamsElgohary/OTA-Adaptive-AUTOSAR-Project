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
    //     execl(executable_path.c_str(),to_string(id).c_str(),*configuration_.arguments[0].c_str(),nullptr);
    // }
    // mkfifo(name.c_str(), 0666);
    // return id ;
}

void Application::update_state()
{
    // int fd = open(name.c_str(), O_RDWR); 
    
    // {
    //     unique_lock <mutex> gaurd {*n};
    //     read(fd, &current_state, sizeof(uint8_t));
    // }

    // c_v->notify_one();
    // close(fd);
    // unlink(name.c_str());
}
void Application::check_for_state(Application::ProcessState s)
{
    // unique_lock <mutex> gaurd {*n};
    // c_v->wait(gaurd,[&]{return current_state!= s;});
}
void Application::terminate()
{

}


