#include "../include/Grant.hpp"

bool ara::iam::Grant::IsEqual(Grant& other)
{
    if((this ->S_id == other.S_id) && (this ->In_id == other.In_id) && (this ->GType == other.GType) && (this ->PR_T == other.PR_T))
    {
        return true;
    }
    return false;
} 

ara::iam::Grant::Grant(ara::iam::ServiceID S, ara::iam::InstanceID I, ara::iam::Grant_Type GT, ara::iam::PR_Type PR) : S_id(S), 
                        In_id(I), GType(GT), PR_T(PR)
    {}

ara::iam::Grant::Grant() : S_id(0), In_id(0), GType(""), PR_T("")
    {}


ara::iam::Grant::~Grant()
    {}

/*
void ara::iam::Grant::Serialize(stringstream& oss)
{
    boost::archive::binary_oarchive oa(oss);
    oa&* (this);
}

void ara::iam::Grant::Deserialize(stringstream& oss)
{
    std::string str_data = oss.str();
    std::istringstream iss(str_data);
    boost::archive::binary_iarchive ia(iss);
    ia&* (this);
} 
*/
