#include "sm_logger.hpp"
#include "../../jsoncpp/header/json.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include "SM.hpp"
#include <stdint.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <bits/stdc++.h>
#include "../../utility/general.hpp"
using namespace ara::sm;
int sm_logger::fd=-1;
std::thread sm_logger::gui_thread;
sm_logger::sm_logger(int server_port)
{
freopen("sm.txt","w+",stdout); //write cout and printf to txt file
this->sim=new simulation(server_port);
this->sim->connect_to_socket();
this->sim->send_exe_name(simulation::exe_name::sm);
this->functions_state={};
std::cout<<"sm logger connected to gui\n";
}
void sm_logger::update_logger(sm_functions functions,map<string,string>fg_states)
{
 /*create updated json file*/
// Read JSON File
Json::Value event;
string file_name = "sm_Report.json";
ifstream f(file_name);
Json::Reader R;
R.parse(f, event);

 Json::Value sm_json = event;
 sm_json["Cluster_name"]="sm";
 
 sm_json["sm_json"]["UpdateRequest"]["StartUpdateSession"]=Json::Value(functions.sm_StartUpdateSession);
 sm_json["sm_json"]["UpdateRequest"]["PrepareUpdate"]=Json::Value(functions.sm_PrepareUpdate);
 sm_json["sm_json"]["UpdateRequest"]["VerifyUpdate"]=Json::Value(functions.sm_VerifyUpdate);
 sm_json["sm_json"]["UpdateRequest"]["StopUpdateSession"]=Json::Value(functions.sm_StopUpdateSession);


 for(auto x :fg_states)
 sm_json["sm_json"]["function_group_states"][x.first]=x.second;
 
 std::ofstream json_file("sm_Report.json");
 json_file<<sm_json;
 json_file.close();
 reset();
 
 /*connect to server*/
 if (SIMULATION_ACTIVE)
 {
    char current_dir[256];
    getcwd(current_dir,256);
    std::string path(current_dir);
    path+="/sm_Report.json";
    this->sim->send_file((char *)(path.c_str()));
 }
 
}
void sm_logger::reset()
{
this->functions_state={};
}
void sm_logger::gui_receive(ara::sm::UpdateRequestImpl* ptr)
{
          gui_thread =std::thread([ptr]{
          string path(CUSTOMIZED_PROJECT_PATH+"gui_sm");
          while(1)
          {
          fd=open(path.c_str(),O_RDONLY);
          //std::cout<<"received cluster\n";
          int cluster;
          read(fd,&cluster,sizeof(int));
          ptr->run_cluster(cluster);
          }
      });
      gui_thread.detach();
   
}
