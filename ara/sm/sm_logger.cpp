#include "sm_logger.hpp"
#include "../../jsoncpp/header/json.h"
#include <fstream>
#include <iostream>
#include <unistd.h>

sm_logger::sm_logger(int server_port)
{
    this->sim = new simulation(server_port);
    this->functions_state = {};
}
void sm_logger::update_logger(sm_functions functions, map<string, string> fg_states)
{
    /*create updated json file*/
    Json::Value fileData;
    string file_name = "GUI_Report.json";
    ifstream f(file_name);
    Json::Reader R;
    R.parse(f, fileData);
    Json::Value Tempcom = fileData["com_json"];

    Json::Value sm_json;
    sm_json["Cluster_name"] = "sm";

    sm_json["sm_json"]["UpdateRequest"]["StartUpdateSession"] = Json::Value(functions.sm_StartUpdateSession);
    sm_json["sm_json"]["UpdateRequest"]["PrepareUpdate"] = Json::Value(functions.sm_PrepareUpdate);
    sm_json["sm_json"]["UpdateRequest"]["VerifyUpdate"] = Json::Value(functions.sm_VerifyUpdate);
    sm_json["sm_json"]["UpdateRequest"]["StopUpdateSession"] = Json::Value(functions.sm_StopUpdateSession);

    for (auto x : fg_states)
        sm_json["sm_json"]["function_group_states"][x.first] = x.second;

    sm_json["com_json"] = Tempcom;

    std::ofstream json_file("GUI_Report.json");
    json_file << sm_json;
    json_file.close();
    reset();

    /*connect to server*/
    this->sim->connect_to_socket();
    char current_dir[256];
    getcwd(current_dir, 256);
    std::string path(current_dir);
    path += "/GUI_Report.json";
    this->sim->send_file((char *)(path.c_str()));
    sleep(3);
}
void sm_logger::reset()
{
    this->functions_state = {};
}