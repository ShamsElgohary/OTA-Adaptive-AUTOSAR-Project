#include "../include/applicationManifest.hpp"
#include <boost/filesystem.hpp>

using namespace ara::exec ;
namespace pt = boost::property_tree;
using namespace boost::filesystem;

ApplicationManifest::ApplicationManifest(string executionManifestPath)
{   
    path p {executionManifestPath};
    name = p.parent_path().parent_path().filename().string() ;
    executable_path = p.parent_path().parent_path().string()+"/bin/";

    pt::ptree executionTree;
    pt::read_json(executionManifestPath, executionTree);
    
    platform_application =  executionTree.get<string>("platform_application")=="true";
    for(auto &confg :executionTree.get_child("startup_configs"))
    {   
        ApplicationManifest::startUpConfiguration con;
        for(auto &arguments :confg.second.get_child("arguments"))
        {   
            con.arguments.push_back(arguments.second.data());
        }
        for(auto &environments :confg.second.get_child("environments"))
        {   
            con.environments.push_back(environments.second.data());
        }
        con.scheduling_priority =confg.second.get<string>("scheduling_priority");
        con.scheduling_policy =confg.second.get<string>("scheduling_policy");

        for(auto &machine_states :confg.second.get_child("machine_states"))
        {   
            con.machine_states.push_back(machine_states.second.data());
            applicationStates.push_back(machine_states.second.data());
        }
        bool flag = true ;
        for(auto &function_group_states :confg.second.get_child("function_group_states"))
        {    
            
            for(auto &function_group :function_group_states.second)
            {
                if(flag)
                {
                    con.function_group_states[function_group_states.first.data()] = vector<string>();
                    flag =false ;
                }
                con.function_group_states[function_group_states.first.data()].push_back(function_group.second.data());
                applicationStates.push_back(function_group.second.data());
                
            }
        }
        startUpConfigurations.push_back(con);
    }
}