#pragma once
#include "types.hpp"

using namespace std;
using namespace ara::iam;
namespace ara
{
    namespace iam
    {
        class Grant
        {
            private:

            public:
            	ara::iam::ServiceID S_id = 0;
                ara::iam::InstanceID In_id = 0;
                ara::iam::Grant_Type GType = "";
                ara::iam::PR_Type PR_T = "";

                bool IsEqual(Grant& other);        

                Grant();

                Grant(ara::iam::ServiceID S, ara::iam::InstanceID I, ara::iam::Grant_Type GT, ara::iam::PR_Type PR);

                ~Grant();

        };
    }
}
namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, ara::iam::Grant & g, const unsigned int version)
{
    ar & g.S_id;
    ar & g.In_id;
    ar & g.GType;
    ar & g.PR_T;
}
void Serialize(ara::iam::Grant & g, stringstream& oss);

void Deserialize(ara::iam::Grant & g, stringstream& oss);

} // namespace serialization
} // namespace boost



