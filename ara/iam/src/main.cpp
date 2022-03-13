#include "../include/AccessManager.hpp"

int main (void)
{
    ara::iam::AccessManager::InitGrantStorage("../GrantStorage.json");
    std::cout<<"Done";
    return 0;
}