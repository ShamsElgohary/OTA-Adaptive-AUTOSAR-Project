#ifndef SM_H
#define SM_H
#include <vector>
namespace ara
{
    namespace sm
    {
        class UpdateRequest
        {
            public :
            bool StartUpdateSession();
            bool PrepareUpdate(std::vector<FunctionGroup>FunctionGroups); // type from em
            bool VerifyUpdate(std::vector<FunctionGroup>FunctionGroups); // type from em
            bool StopUpdateSession();
        };
    }
}
#endif