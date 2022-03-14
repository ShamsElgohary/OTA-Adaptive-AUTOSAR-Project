#include "../include/IAMGrantQuery.hpp"

int main (void)
{
    ara::iam::IAMGrantQuery IGQ;

    ara::iam::Grant G(2, 1, "ComGrant", "Provide");

    bool rtn = IGQ.HasGrant(G);

    std::cout << "Result: " << rtn << std::endl;

    std::cout << "Client End" << std::endl;

    return 0;
}
