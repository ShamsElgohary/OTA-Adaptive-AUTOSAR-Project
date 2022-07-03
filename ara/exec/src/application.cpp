#include "../include/application.hpp"
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include "applicationExecutionMgr.hpp"
using namespace std;
using namespace ara::exec;
extern std::ofstream outdata;

Application::Application(Application::CtorToken &&token)
{
    outdata.open("em_report.txt");
    current_state = ExecutionState::Kidle;
    configuration_ = token.configration;
    name = token.name;
    executable_path = token.executable_path;
}
void Application::start()
{
    thread([this]()
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
            //chdir(executable_path.c_str());
            dup2(fd, 0);
            string path = executable_path+name;
            execl(path.c_str(), nullptr);
        }
        locker.unlock();
        ExecutionState newstate = ExecutionState::Kidle;
        read(fd, &newstate, sizeof(current_state));
        locker.lock();
        current_state = newstate;
        locker.unlock();
        condr.notify_all();
        if(SIMULATION_ACTIVE)
        {
            static_cast<ApplicationExecutionMgr*>(parent)->reportConfig_simulation();
        }
        outdata << "[EM] " << name << " state is Krunning " << id << "\n";
        outdata.flush();
        Update_status(); })
        .detach();
}
void Application::terminate()
{
    unique_lock<mutex> locker(mur);
    if (kill(id, SIGTERM) == -1)
    {
        outdata << "[EM] couldn't terminate process.... with id = " << id << " and named " << name << "\n";
        outdata.flush();
    }
    else
    {
        outdata << "[EM] terminateing " << name << "\n";
        outdata.flush();
        id = 0;
    }
    locker.unlock();
}

void Application::Update_status()
{
    ExecutionState newstate = ExecutionState::Kidle;
    read(fd, &newstate, sizeof(current_state));
    unique_lock<mutex> locker(mur);
    current_state = newstate;
    id = 0;
    close(fd);
    locker.unlock();
    if (SIMULATION_ACTIVE)
    {
        static_cast<ApplicationExecutionMgr *>(parent)->reportConfig_simulation();
    }
    outdata << "[EM] " << name << " state is Kterminate"
            << "\n";
    outdata.flush();

    condt.notify_all();
}
Application::Application(ApplicationManifest::startUpConfiguration con, string name, string path)
{
    configuration_ = con;
    this->name = name;
    executable_path = path;
    current_state = ExecutionState::Kidle;
}

Application::~Application()
{
    close(fd);
}
