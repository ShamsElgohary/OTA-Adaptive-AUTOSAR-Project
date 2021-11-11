#include "em.h"
#include "function_group.h"
#include "function_group_state.h"
#include "executable.h"
#include "process.h"
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

using namespace ara::exec;
using namespace std;
using namespace boost::filesystem;

vector <FunctionGroup>  function_groups;

vector <Executable>  executables;

map<string,vector<Process>> current_processes; 



void getFunctionGroups()
{
    FunctionGroup::CtorToken t= FunctionGroup::Preconstruct("");

    for(;t.fg_index<t.size ;t.fg_index++)
    {
        function_groups.push_back( 
            FunctionGroup(move(t))
            );
    }

}


void getExcutables()
{
    path p("/home/loay/Documents/graduation project/executables");
    try
    {
        if (exists(p))
        {
            if (is_directory(p))
            {
                cout << p << " is a directory containing:\n";
                    
                for (directory_entry& x : directory_iterator(p))
                {
                    string p = x.path().string() +"/" + x.path().filename().string()+".json";
                    executables.push_back(Executable(p));
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
    for(auto &exe: executables )
    {   
       if(exe.configration.find(pair<string,string>(fg_name,fg_newState)) != exe.configration.end() )
       {
            Process::CtorToken token= Process::preconstruct(exe, fg_name, fg_newState);
            current_processes[fg_name].push_back(Process(move(token))) ;
       }
    }
}


void process_handel(Process &process)
{
    for (auto &x : process.configration.dependency)
    {
      for (auto &y:current_processes[process.function_group_name])
      {
         if(y.name==x)
         {
             y.check_for_state(Process::processState::Kidel);
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


//a => machinefg  ,, a=> fg1 

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

    getFunctionGroups();
    print_fng();

    getExcutables();
    print_exe();

    ////////////////////open sm///////////////////////////////

    for(auto exe: executables)
    {
        if(exe.name=="sm")
        {
            Process::CtorToken token= Process::preconstruct(exe, "machineFG", "startup");
            Process sm =Process(move(token));
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
        //machinfg =>strartup 
        //fng1 =>runing 
        terminate_all_process(fg_name);
        getprocesses(fg_name,fg_newState);
        run_processes(fg_name);
    }
    close(fd);
    unlink("smFifo");
    ////////////////////close sm//////////////////////////////
}
