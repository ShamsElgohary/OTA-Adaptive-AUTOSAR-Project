# include "../include/IPCserverInterface.hpp"

std::uint8_t ara::iam::IPCserverInterface::ServerSocketInit()
{
    //setup a socket and connection tools
    bzero((char*)&servAddr, sizeof(servAddr));
    ara::iam::IPCserverInterface::servAddr.sin_family = AF_INET;
    ara::iam::IPCserverInterface::servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ara::iam::IPCserverInterface::servAddr.sin_port = htons(IAM_PORT_NUMBER);
 
    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    ara::iam::IPCserverInterface::ServerSD = socket(AF_INET, SOCK_STREAM, 0);
    if(ara::iam::IPCserverInterface::ServerSD < 0)
    {
        cerr << "Error establishing the server socket" << std::endl;
        return 1;
    }
    //bind the socket to its local address
    int bindStatus = bind(ara::iam::IPCserverInterface::ServerSD , (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << std::endl;
        return 2;
    }
    return 0;
}

int ara::iam::IPCserverInterface::getPeerId(int sd)
{
    recv(sd , &clientPID , sizeof(int) , 0 ) ;
    return clientPID;
}

int ara::iam::IPCserverInterface::Listen()
{
    std::cout << "[iamServer] Waiting for a client to connect..." << std::endl;
    // listen for up to 5 requests at a time
    listen(ara::iam::IPCserverInterface::ServerSD, 5);
    // receive a request from client using accept
    // we need a new address to connect with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    // accept, create a new socket descriptor to 
    // handle the new connection with client
    int newSd = accept(ara::iam::IPCserverInterface::ServerSD , (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << std::endl;
        return -1;
    }
    std::cout << "[iamServer] Connected with client!" << std::endl;

    return newSd;
}

void ara::iam::IPCserverInterface::Send(bool is_granted, int sd)
{
    send(sd, &is_granted, sizeof(bool), 0);
}

ara::iam::Grant ara::iam::IPCserverInterface::Receive(int sd)
{
    char msg[1500];
    //receive a message from the client (listen)
    std::cout << "[iamServer] Awaiting client response..." << std::endl;
    memset(&msg, 0, sizeof(msg));//clear the buffer
    
    recv(sd, (char*)&msg, sizeof(msg), 0);
    std::string op = ara::iam::convertToString(msg, 1500);
    // Convert to string Stream
    std::stringstream ss;
    ss.str(op);

    // Create GrantObj
    ara::iam::Grant G;

    // Deserialize stringstream ss in GrantObj
    boost::serialization::Deserialize(G, ss);
    
    // std::cout << "[CLIENT]" << std::endl;
    // std::cout << "Service ID: " << G.S_id << std::endl;
    // std::cout << "Instance ID: " << G.In_id << std::endl;
    // std::cout << "Grant Type: " << G.GType << std::endl;
    // std::cout << "PR Type: " << G.PR_T << std::endl;

    return G;
}
