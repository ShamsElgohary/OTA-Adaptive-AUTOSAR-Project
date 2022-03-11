#include "IAM_types.h"
using namespace iam;


class Grant
{
private:
    ServiceID S_id;
    InstanceID In_id;
    friend class boost::serialization::access;
    template <typename Archive>
 
    
public:
    //Constructor to set the id parameters
    void Grant(ServiceID S, InstanceID I);
    static vector<Grant> ParseJson(string process_name);
    std::stringstream serialize_write(Archive& ar, const unsigned int version){
        // ar & S_id;
        // ar & In_id;
        // return ar;
    }
    static Grant serialize_read(std::stringstream input);
};
