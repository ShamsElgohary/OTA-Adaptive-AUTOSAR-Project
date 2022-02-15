#include "../include/function_group.hpp"
#include "../include/function_group_state.hpp"
#include "../include/applicationManifest.hpp"
#include "../include/application.hpp"
#include <thread>
#include <future>
#include <stdint.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <boost/filesystem.hpp>
#include <bits/stdc++.h>
#include "../include/applicationExecutionMgr.hpp"

using namespace ara::exec;
using namespace std;
using namespace boost::filesystem;

#define ARA_ROOT "/home/loay/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/ara"

vector <FunctionGroup>  function_groups;
vector <ApplicationManifest>  executables;
map<string,vector<Application>> current_processes; 


// void getFunctionGroups()
// {
//     FunctionGroup::CtorToken t= FunctionGroup::Preconstruct(string(ARA_ROOT)+"/etc/system/machine_manifest.json");
//     for(;t.fg_index<t.size ;t.fg_index++)
//     {
//         function_groups.push_back( 
//             FunctionGroup(move(t))
//             );
//     }

// }


void getExcutables()
{
    path p(string(ARA_ROOT)+"/executables");
    try
    {
        if (exists(p))
        {
            if (is_directory(p))
            {
                cout << p << " is a directory containing:\n";
                    
                for (directory_entry& x : directory_iterator(p))
                {
                    string p = x.path().string() +"/etc" + x.path().filename().string()+".json";
                    Executable E = {ApplicationManifest(p)};
                    executables_.push_back(E);

                }
            }
            else
            {
                cout << p << " exists, but is not a regular file or directory\n";
            }
        }
        else
        {
               cout << p << " does not exist\n";
        }
    }
    catch (const filesystem_error& ex)
    {
        cout << ex.what() << '\n';
    }

}

void terminate_all_process(string fn_name )
{
for(auto& index: current_processes[fn_name]){
    kill(index.id,SIGTERM);
}
}


void getprocesses(string fg_name , string fg_newState)
{
    for(auto &exe: executables_ )
    {   
       if(exe.configration.find(pair<string,string>(fg_name,fg_newState)) != exe.configration.end() )
       {
            Application::CtorToken application= Application::preconstruct(exe, fg_name, fg_newState);
            current_processes[fg_name].push_back(Application(move(application))) ;
       }
       if(exe.manifest_.fu != exe.configration.end() )
       {
            Application::CtorToken application= Application::preconstruct(exe, fg_name, fg_newState);
            current_processes[fg_name].push_back(Application(move(application))) ;
       }
    }
}


void process_handel(Application &process)
{
    for (auto &x : process.configration.dependency)
    {
      for (auto &y:current_processes[process.function_group_name])
      {
         if(y.name==x)
         {
             y.check_for_state(Application::processState::Kidel);
         }
      } 
    }
    process.run();
    process.update_state();
}


void run_processes(string fg_name) 
{
    vector <future<void>> threads;

    for(auto &process: current_processes[fg_name])
    {
       threads.push_back(async(launch::async,process_handel,ref(process)));
    }
    for(auto& t :threads )
    {
        t.wait();
    }
}

void print_fng()
{
    for(auto & i : function_groups)
    {
        cout<<"function group name: "<<i.name <<" function group states ";
        for(auto x :i.states)
        {
            cout<<x <<" ";
        }
        cout<<"\n";
    }
}


void print_exe()
{
    for(auto & i : executables)
    {
        cout<<"executable name: "<<i.name <<" executable path: " <<i.executable_path;
        cout<<"\n";
    }
}


int main ()
{

    // getFunctionGroups();
    // print_fng();

    getExcutables();
    print_exe();


    ApplicationExecutionMgr app(ARA_ROOT);
    ////////////////////open sm///////////////////////////////



    for(auto exe: executables_)
    {
        if(exe.manifest_=="sm")
        {
            Application::CtorToken token= Application::preconstruct(exe, "machineFG", "startup");
            Application sm =Application(move(token));
            sm.run();
            sm.update_state();
        }
    }

    mkfifo("smFifo", 0666);
    int fd = open("smFifo", O_RDWR); 

    string fg_name ;
    string fg_newState ;
    int size;

    while(true)
    {
        

        read(fd, &size, sizeof(int));
        read(fd, &fg_name, size*sizeof(char));

        read(fd, &size, sizeof(int));
        read(fd, &fg_newState, size*sizeof(char));
    
        terminate_all_process(fg_name);
        getprocesses(fg_name,fg_newState);
        run_processes(fg_name);
    }
    close(fd);
    unlink("smFifo");
    ////////////////////close sm//////////////////////////////
}
