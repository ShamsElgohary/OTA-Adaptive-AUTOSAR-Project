#pragma once
#include <bits/stdc++.h>
#include "application.hpp"
#include "applicationManifest.hpp"
#include "function_group.hpp"
#include <boost/filesystem.hpp>

namespace ara {
    namespace exec
    {
        class MachineManifest final
        {
            public:
                vector<FunctionGroup> function_groups;
                vector<string> machine_states;
                MachineManifest(string path);
        };
    }
}
