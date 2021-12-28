#ifndef SM_H
#define SM_H
#include <vector>
#include <string.h>
#include <bits/stdc++.h>
using Functiongroup=std::string;
namespace ara
{
    namespace sm
    {
        class UpdateRequest
        {
            public :
            bool StartUpdateSession();
            bool PrepareUpdate(std::vector<Functiongroup>FunctionGroups); // type from em
            bool VerifyUpdate(std::vector<Functiongroup>FunctionGroups); // type from em
            bool StopUpdateSession();
        };
    }
}
#endif