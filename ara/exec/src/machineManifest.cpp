#include "../include/machineManifest.hpp"

using namespace std ;
using namespace boost::filesystem;
using namespace ara::exec ;

MachineManifest::MachineManifest(string path)
{
    FunctionGroup::CtorToken t  = FunctionGroup::Preconstruct(path);
    for(;t.fg_index<t.size ;t.fg_index++)
    {
        if(t.fg_names[t.fg_index] != "machineState")
            function_groups.push_back( FunctionGroup( move( t ) ) );
        else
            function_groups.push_back( FunctionGroup( move( t ) ) );
            machine_states =t.fg_states["machineState"];
    }
}
