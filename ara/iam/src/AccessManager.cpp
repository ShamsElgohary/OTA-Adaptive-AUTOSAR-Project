#include "../include/AccessManager.hpp"

void ara::iam::AccessManager::InitGrantStorage(std::string basePath)
{
    #ifdef SIMULATION_ACTIVE
        sim_socket.connect_to_socket();
        sim_socket.send_exe_name(simulation::exe_name::iam);
    #endif
    ara::iam::GrantStorage::ParseJson(basePath, sim_socket);
}

std::uint8_t ara::iam::AccessManager::InitServerAdapter()
{
    // RUN SERVER
    return server.ServerSocketInit();
}

void ara::iam::AccessManager::RunEventLoop()
{
    Json::Value request;
    // m7taga ageb path el iam_json file hena
    ifstream f("iam_access.json");
    Json::Reader R;
    R.parse(f, request);
    Json::Value t;
    // Add To JSON FILE
    // request["Cluster_name"] = "iam_json";
    request["Requests"] = Json::arrayValue;
    t["Grant_Result"] = Json::stringValue;
    t["Process_id"] = Json::stringValue;
    t["Process_name"] = Json::stringValue;
    t["info"]["GrantType"] = Json::stringValue;
    t["info"]["PR_Type"] = Json::stringValue;
    t["info"]["Service_ID"] = Json::stringValue;
    t["info"]["Instance_ID"] = Json::stringValue;

    while (true)
    {

        // Receive PID
        server.Listen();
        ara::iam::Grant G = server.Receive(0);

        // std::cout << "[iamServer] "  << PID << std::endl;
        //  Send PID to EM
        FPC.sendData(G.PID);

        // Receive Proc
        std::string P_name = FPC.receiveData();
        // std::string P_name = "ucm";
        // cout<< "[iamServer] process name: "<<P_name<<endl;
        // RECIEVE GRANT FROM CLIENT

        // SEARCH GRANTSTORAGE MAP
        bool rtn = ara::iam::GrantStorage::SearchGrantStorage(P_name, G);

        // std::cout << "[iamServer] Result: " << rtn << std::endl;

        // RETURN RESULT TO CLIENT
        server.Send(rtn, 0);

        server.socket_.close();
        t["Grant_Result"] = rtn;
        t["Process_id"] = G.PID;
        t["Process_name"] = P_name;
        t["info"]["GrantType"] = G.GType;
        t["info"]["PR_Type"] = G.PR_T;
        t["info"]["Service_ID"] = G.S_id;
        t["info"]["Instance_ID"] = G.In_id;

        request["Requests"].append(t);

        // std::cout << request << std::endl;
        // matnsesh path el file
        std::ofstream json_f("iam_access.json");
        json_f << request;
        json_f.close();

    #ifdef SIMULATION_ACTIVE
        {
            char current_dir[256];
            getcwd(current_dir, 256);
            std::string path(current_dir);
            path += "/iam_access.json";
            sim_socket.send_file((char *)(path.c_str()));
        }
         #endif

    }
}