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
void Application::start()
{
    mkfifo(this->name.c_str(), 0777);
    fd = open(this->name.c_str(), O_RDWR);
    // fcntl(fd, F_SETFL, O_NONBLOCK);

    this->id = fork();
    if (this->id == 0)
    {
        dup2(fd, 0);
        execl(executable_path.c_str(), nullptr);
    }
}
void Application::terminate()
{
    if (kill(id, SIGTERM))
    {
        cout << "[em] couldn't terminate process.... with id = " << id << " and named " << name << "\n\n\n";
    }
}

future<void> Application::Update_status()
{
    return async(launch::async, [this]()
                                {
                    ExecutionState newstate = ExecutionState::Kidle;
                    read(fd, &newstate, sizeof(current_state));
                    if (newstate == ExecutionState::Krunning)
                    {
                        current_state = newstate;
                        cout << "[em] " << name << " new state is Krunning "<<id<<"\n\n\n";
                    }
                    newstate = ExecutionState::Kidle;
                    read(fd, &newstate, sizeof(current_state));
                    if (newstate == ExecutionState::Kterminate)
                    {
                        current_state = newstate;
                        cout << "[em] " << name << " new state is Kterminate "<<id<<"\n\n\n";
                        close(fd);
                        id=0;
                    } });
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

