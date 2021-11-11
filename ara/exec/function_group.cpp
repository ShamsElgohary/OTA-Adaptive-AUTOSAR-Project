#include "function_group.h"
namespace pt = boost::property_tree;
using namespace ara::exec;

FunctionGroup::CtorToken FunctionGroup::Preconstruct(string machine_manifest_path)
{
        CtorToken token ;
        pt::ptree machineTree;
        pt::read_json("/home/loay/Documents/graduation project/ara/manifest/MachineManifest.json", machineTree);
        for(auto &fng :machineTree.get_child("function_groups") )
        {
            token.fg_names.push_back(fng.first);
            for (auto &state : fng.second.get_child("states"))
            {
                token.fg_states[fng.first].push_back(state.second.data());
            }
        }
        token.fg_index =0;
        token.size = token.fg_names.size();
        return token;
}
FunctionGroup::FunctionGroup(FunctionGroup::CtorToken&& token)
{
    name = token.fg_names[token.fg_index];
    states = token.fg_states[name];
    current_state = states[0];
}


FunctionGroup::~FunctionGroup()
{

}