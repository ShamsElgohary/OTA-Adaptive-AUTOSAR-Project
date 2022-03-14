#include "../include/IPCclientInterface.hpp"

std::string ara::iam::convertToString(char* a, int size)
{
    int i;
    std::string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

int ara::iam::IPCclientInterface::Connect()
{
    //setup a socket and connection tools 
    struct hostent* host = gethostbyname(IAM_IP_ADDRESS); 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = 
        inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(IAM_PORT_NUMBER);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    //try to connect...
    int status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        std::cout << "Error connecting to socket!" << std::endl; 
    }
    std::cout << "Connected to the server!" << std::endl;

    return clientSd;
}
void ara::iam::IPCclientInterface::Send(std::string data, int clientSd)
{
    char msg[1500]; 
    std::cout << ">";
    memset(&msg, 0, sizeof(msg));//clear the buffer
    strcpy(msg, data.c_str());
    if(data == "exit")
    {
        send(clientSd, (char*)&msg, strlen(msg), 0);
    }
    send(clientSd, (char*)&msg, strlen(msg), 0);
}
void ara::iam::IPCclientInterface::sendPID(int clientPID)
{

}
bool ara::iam::IPCclientInterface::Receive(int clientSd)
{
    char msg[1500]; 
    std::cout << "Awaiting server response..." << std::endl;
    memset(&msg, 0, sizeof(msg));//clear the buffer
    recv(clientSd, (char*)&msg, sizeof(msg), 0);
    return msg[0] = '0' ? 0 : 1;
}