#pragma once
#include "types.h"

namespace ara
{
    namespace iam
    {
        class Grant
        {   
            private:
                friend class boost::serialization::access;
                friend class ara::iam::GrantStorage;
                template <typename Archive>
                void serialize(Archive& ar, const unsigned int version)
                {
                    ar & S_id;
                    ar & In_id;
                    ar & GType;
                    ar & PR_T;
                }
            protected:
                ara::iam::ServiceID S_id;
                ara::iam::InstanceID In_id;
                ara::iam::Grant_Type GType;
                ara::iam::PR_Type PR_T;

            public:
                bool IsEqual(Grant& other);        

                //Constructor to set the id parameters
                Grant(ara::iam::ServiceID S, ara::iam::InstanceID I, ara::iam::Grant_Type GT);

                void Serialize(ostringstream& oss)
                {
                    boost::archive::binary_oarchive oa(oss);
                    oa&* (this);
                }

                void Deserialize(ostringstream& oss)
                {
                    std::string str_data = oss.str();
                    std::istringstream iss(str_data);
                    boost::archive::binary_iarchive ia(iss);
                    ia&* (this);
                }
        };
    }
}
