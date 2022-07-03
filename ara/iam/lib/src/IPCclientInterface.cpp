#include "../include/IPCclientInterface.hpp"

std::uint8_t ara::iam::IPCclientInterface::Connect()
{
    //setup a socket and connection tools 
    struct hostent* host = gethostbyname(IAM_IP_ADDRESS); 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = 
        inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(IAM_PORT_NUMBER);
    ara::iam::IPCclientInterface::clientSd = socket(AF_INET, SOCK_STREAM, 0);
    //try to connect...
    int status = connect(ara::iam::IPCclientInterface::clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    int failcount = 0;
    while (status < 0 && failcount < 3)
    {
        usleep(20);
        status = connect(ara::iam::IPCclientInterface::clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
        failcount += 1;
        if (status >= 0)
        {
            break;
        }
    }
    if (status < 0)
    {
        std::cout << "[iamClient] " << "Error connecting to socket!" << std::endl;
        return 1;
    }
    std::cout << "[iamClient] " << "Connected to the server!" << std::endl;

    return 0;
}
void ara::iam::IPCclientInterface::Send(ara::iam::Grant G)
{
    std::stringstream ss;

    // Serialize GrantObj 1 in stringstream ss
    boost::serialization::Serialize(G, ss);

    std::string data = ss.str();

    char msg[1500]; 
    memset(&msg, 0, sizeof(msg));//clear the buffer
    strcpy(msg, data.c_str());
    if(data == "exit")
    {
        send(ara::iam::IPCclientInterface::clientSd, (char*)&msg, strlen(msg), 0);
    }
    send(ara::iam::IPCclientInterface::clientSd, (char*)&msg, strlen(msg), 0);
}

void ara::iam::IPCclientInterface::sendPID(int pid)
{
    send(ara::iam::IPCclientInterface::clientSd , &pid, sizeof(int) , 0 );
}

bool ara::iam::IPCclientInterface::Receive()
{
    std::cout << "[iamClient] " << "Awaiting server response..." << std::endl;
    recv(ara::iam::IPCclientInterface::clientSd, &is_granted, sizeof(bool), 0);
    std::cout << "[iamClient] " << "Result: " << is_granted << std::endl;
    return is_granted;
}