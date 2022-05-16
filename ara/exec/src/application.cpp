#include "../include/application.hpp"
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

using namespace std;
using namespace ara::exec;

Application::Application(Application::CtorToken &&token)
{
    current_state = ExecutionState::Kidle;
    configuration_ = token.configration;
    name = token.name;
    executable_path = token.executable_path;
}
future<void> Application::start()
{
    return async(launch::async, [this]()
                 {
        for (auto &app_name : configuration_.dependency)
        {
            Application* app =depend[app_name.first] ;
            unique_lock<mutex>  locker(app->mur);

            if(app_name.second == "Krunning")
            {
                if(app->current_state !=ExecutionState::Krunning)
                    app->condr.wait(locker);
            }
            else{
                if(app->current_state !=ExecutionState::Kterminate)
                    app->condt.wait(locker);
            }

        }
        unique_lock<mutex>  locker(mur);
        mkfifo(this->name.c_str(), 0777);
        fd = open(this->name.c_str(), O_RDWR);
        this->id = fork();
        if (this->id == 0)
        {
            dup2(fd, 0);
            execl(executable_path.c_str(), nullptr);
        }
        locker.unlock();
        Update_status(); });
}
void Application::terminate()
{
    unique_lock<mutex> locker(mur);
    if (kill(id, SIGTERM)==-1)
    {
        cout << "[em] couldn't terminate process.... with id = " << id << " and named " << name << "\n\n\n";
    }
    else{
        cout << "[em] terminateing "<< name << "\n";
        id=0;
    }
    locker.unlock();
}

void Application::Update_status()
{
    thread([this]()
           {
        ExecutionState newstate = ExecutionState::Kidle;
        read(fd, &newstate, sizeof(current_state));
        unique_lock<mutex>  locker(mur);
        current_state = newstate;
        locker.unlock();
        condr.notify_one();
        cout << "[em] " << name << " new state is Krunning " << id << "\n\n\n";
    
        read(fd, &newstate, sizeof(current_state));
        locker.lock();
        current_state = newstate;
        close(fd);
        locker.unlock();
        cout << "[em] " << name << " new state is Kterminate"<<"\n\n\n";
        condt.notify_one();
         }).detach();
}
Application::Application(ApplicationManifest::startUpConfiguration con, string name, string path)
{
    configuration_ = con;
    this->name = name;
    executable_path = path + "/" + name;
    current_state = ExecutionState::Kidle;
}

Application::~Application()
{
    close(fd);
}
