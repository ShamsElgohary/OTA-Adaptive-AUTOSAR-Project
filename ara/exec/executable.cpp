#include "executable.h"
#include <boost/filesystem.hpp>

using namespace ara::exec ;
namespace pt = boost::property_tree;
using namespace boost::filesystem;

Executable::Executable(string executionManifestPath)
{   
    path p {executionManifestPath};
    name = p.parent_path().filename().string() ;
    executable_path = p.parent_path().filename().string()+"/"+name+".out";

    pt::ptree executionTree;
    pt::read_json(executionManifestPath, executionTree);
    
    platform_application =  executionTree.get<string>("platform_application")=="true";
    for(auto &confg :executionTree.get_child("startup_configs"))
    {   
        startUpConfigration con ;
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
        {   pair <string ,string> p ;
            p = {"machineFg",machine_states.second.data()};
            configration[p] = con;
        }
        for(auto &function_group_states :confg.second.get_child("function_group_states"))
        {   pair <string ,string> p ;
            p = {function_group_states.first,function_group_states.second.data()};
            configration[p] = con;
        }
        
    }
}