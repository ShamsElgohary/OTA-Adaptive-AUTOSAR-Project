# include "../include/IPCserverInterface.hpp"

std::string ara::iam::convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

int ara::iam::IPCserverInterface::getPeerId(int clientSocketDecriptor)
{

}
int ara::iam::IPCserverInterface::Connect()
{
    //setup a socket and connection tools
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(IAM_PORT_NUMBER);
 
    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << std::endl;
        exit(0);
    }
    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << std::endl;
        exit(0);
    }
    std::cout << "Waiting for a client to connect..." << std::endl;
    //listen for up to 5 requests at a time
    listen(serverSd, 5);
    //receive a request from client using accept
    //we need a new address to connect with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to 
    //handle the new connection with client
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << std::endl;
        exit(1);
    }
    std::cout << "Connected with client!" << std::endl;

    return newSd;
}

void ara::iam::IPCserverInterface::Send(std::string data, int sd)
{
    char msg[1500];
    memset(&msg, 0, sizeof(msg)); //clear the buffer
    strcpy(msg, data.c_str());
    send(sd, (char*)&msg, strlen(msg), 0);
}

ara::iam::Grant ara::iam::IPCserverInterface::Receive(int sd)
{
    char msg[1500];
    //receive a message from the client (listen)
    std::cout << "Awaiting client response..." << std::endl;
    memset(&msg, 0, sizeof(msg));//clear the buffer
    
    recv(sd, (char*)&msg, sizeof(msg), 0);
    std::cout << "Client: " << msg << std::endl;
    std::string op = ara::iam::convertToString(msg, 1500);
    //return op;
    //call deserializer
}
