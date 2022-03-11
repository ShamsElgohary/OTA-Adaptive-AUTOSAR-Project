#include "IAM_types.h"
using namespace iam;


class GrantStorage
{
public:
    std::map<process_name, vector<int>> AccessMap;
};