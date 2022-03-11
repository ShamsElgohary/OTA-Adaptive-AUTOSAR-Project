#include "IAM_types.h"
using namespace iam;


class IPCclientInterface
{
public:
    int socket_descriptor;

    void connect();
    void send(std::stringstream data);
    bool receive();
};