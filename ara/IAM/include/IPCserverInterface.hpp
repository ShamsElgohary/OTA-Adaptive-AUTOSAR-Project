#include "IAM_types.h"

class IPCserverInterface
{
public:
    int getPeerId(int clientSocketDecriptor);
    void connect();
    void send(std::stringstream data);
    std::stringstream receive();
};