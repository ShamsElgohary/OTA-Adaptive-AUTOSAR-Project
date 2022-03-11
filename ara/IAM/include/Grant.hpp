#include "IAM_types.h"
using namespace iam;


class Grant
{
private:
    ServiceID S_id;
    InstanceID In_id;
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar & S_id;
        ar & In_id;
    }
    
public:
    //Constructor to set the id parameters
    void Grant(ServiceID S, InstanceID I);
    void ParseJson();
};
