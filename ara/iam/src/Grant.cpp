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


void boost::serialization::Serialize(ara::iam::Grant & g, stringstream& ss)
{
    boost::archive::text_oarchive oa(ss);
    oa << g;
}

void boost::serialization::Deserialize(ara::iam::Grant & g, stringstream& ss)
{
    boost::archive::text_iarchive ia(ss);
    ia >> g;
} 

