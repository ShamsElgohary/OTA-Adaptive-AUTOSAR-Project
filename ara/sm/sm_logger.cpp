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
using namespace ara::sm;

sm_logger::sm_logger(int server_port)
{
//freopen("sm.txt","w+",stdout); //write cout and printf to txt file     
this->sim=new simulation(server_port);
this->functions_state={};
}
void sm_logger::update_logger(sm_functions functions,map<string,string>fg_states)
{
 /*create updated json file*/

 Json::Value sm_json;
 sm_json["Cluster_name"]="sm_json";
 
 sm_json["sm_json"]["UpdateRequest"]["StartUpdateSession"]=Json::Value(functions.sm_StartUpdateSession);
 sm_json["sm_json"]["UpdateRequest"]["PrepareUpdate"]=Json::Value(functions.sm_PrepareUpdate);
 sm_json["sm_json"]["UpdateRequest"]["VerifyUpdate"]=Json::Value(functions.sm_VerifyUpdate);
 sm_json["sm_json"]["UpdateRequest"]["StopUpdateSession"]=Json::Value(functions.sm_StopUpdateSession);


 for(auto x :fg_states)
 sm_json["sm_json"]["function_group_states"][x.first]=x.second;
 
 std::ofstream json_file("sm.json");
 json_file<<sm_json;
 json_file.close();
 reset();
 
 /*connect to server*/
 this->sim->connect_to_socket();
 char current_dir[256];
 getcwd(current_dir,256);
 std::string path(current_dir);
 path+="/sm.json";
 this->sim->send_file((char *)(path.c_str()));
}
void sm_logger::reset()
{
this->functions_state={};
}
void sm_logger::gui_receive()
{
      this->gui_thread =std::thread([this]{
          UpdateRequestImpl * u=new UpdateRequestImpl(1);
          while(1)
          {
          fd=open("gui_sm",O_RDONLY);
          int cluster;
          read(fd,&cluster,sizeof(int));
          u->run_cluster(cluster);
          close(fd);
          }
      });
      gui_thread.detach();
   
}
