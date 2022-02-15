#include "../include/function_group.hpp"
namespace pt = boost::property_tree;
using namespace ara::exec;

FunctionGroup::CtorToken FunctionGroup::Preconstruct(string machine_manifest_path)
{
        CtorToken token ;
        pt::ptree machineTree;
        pt::read_json(machine_manifest_path, machineTree);
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
FunctionGroup::FunctionGroup(FunctionGroup::CtorToken&& token):name_{token.fg_names[token.fg_index]},allStates_{token.fg_states[name_]},currentState_{&allStates_[0]}
{
    //TODO  map<string,set<Application*>> startupConfigurations_;
}
FunctionGroup::~FunctionGroup()
{}