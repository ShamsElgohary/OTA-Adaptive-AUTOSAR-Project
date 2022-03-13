#include "../include/AccessManager.hpp"

int main (void)
{
    ara::iam::AccessManager::InitGrantStorage("../../etc/system/iam/access_control_lists.json");
    std::cout << "Done" << std::endl;

    // Create GrantObj 1
    ara::iam::Grant G1(1, 1, "ComGrant", "Provide");
    std::stringstream ss;

    // Serialize GrantObj 1 in stringstream ss
    boost::serialization::Serialize(G1, ss);

    // Create GrantObj 2
    ara::iam::Grant G2;

    // Deserialize stringstream ss in GrantObj 2
    boost::serialization::Deserialize(G2, ss);

    std::cout << "Done" << std::endl;


    return 0;
}