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
    if(status < 0)
    {
        std::cout << "Error connecting to socket!" << std::endl;
        return 1;
    }
    std::cout << "Connected to the server!" << std::endl;

    return 0;
}
void ara::iam::IPCclientInterface::Send(ara::iam::Grant G)
{
    std::stringstream ss;

    // Serialize GrantObj 1 in stringstream ss
    boost::serialization::Serialize(G, ss);

    std::string data = ss.str();

    char msg[1500]; 
    std::cout << ">";
    memset(&msg, 0, sizeof(msg));//clear the buffer
    strcpy(msg, data.c_str());
    if(data == "exit")
    {
        send(ara::iam::IPCclientInterface::clientSd, (char*)&msg, strlen(msg), 0);
    }
    send(ara::iam::IPCclientInterface::clientSd, (char*)&msg, strlen(msg), 0);
}

/*void ara::iam::IPCclientInterface::sendPID()
{

}*/

bool ara::iam::IPCclientInterface::Receive()
{
    char msg[1500]; 
    std::cout << "Awaiting server response..." << std::endl;
    memset(&msg, 0, sizeof(msg));//clear the buffer
    recv(ara::iam::IPCclientInterface::clientSd, (char*)&msg, sizeof(msg), 0);
    std::cout << "Result: " << msg << std::endl;
    return msg == "0" ? 0 : 1;
}