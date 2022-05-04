#include "../include/sm_logger.hpp"
#include "../../jsoncpp/header/json.h"
#include <fstream>
#include <iostream>

sm_logger::sm_logger(int server_port)
{
this->sim=new simulation(server_port);
this->functions_state={};
}
void sm_logger::update_logger(sm_functions functions,map<string,string>fg_states)
{
 /*create updated json file*/
 Json::Value sm_json;
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
 this->sim->creat_socket();
 this->sim->connect_to_socket();
 this->sim->send_file("sm.json");
}
void sm_logger::reset()
{
this->functions_state={};
}