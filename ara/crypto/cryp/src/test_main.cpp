#include "../include/crypto_provider.hpp"
#include <fstream>
#include <cmath>
#include <vector>
#include <iostream>
#include <string>
#include "sstream"
using namespace CryptoPP;
using namespace ara::crypto;
using namespace std;
int main()
{
    std::string path =  "/home/youssef/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/ara/crypto/cryp/src/ex1.txt";
    
    ifstream ifs(path, ios::binary | ios::ate);
    ifstream::pos_type pos = ifs.tellg();
    std::vector<char> result(pos);
    ifs.seekg(0, ios::beg);
    ifs.read(&result[0], pos);
    
    char* result_arr=nullptr;
    result_arr=&result[0];

    std::vector <unsigned char> Digest;

    cryp::CryptoProvider cryp_provider;
    cryp::HashFunctionCtx::Uptr hash = cryp_provider.CreateHashFunctionCtx(cryp::HashCtx_AlgID::SHA1_ID);
    hash->Start();
    hash->Update(ReadOnlyMemRegion(result_arr), result.size());
    
    //std::cout << "Message: " << msg << std::endl;

    std::cout << "Digest: ";
    std::cout << std::endl;

    Digest=hash->Finish();
    
    std::stringstream ss;
    
    for (int i = 0; i < Digest.size(); i++)
    {
        std::cout << static_cast<unsigned>(Digest[i]) << "    ";
        ss<< std::hex<<int(Digest[i]);
    }
    std::string res(ss.str());
    std::cout<<res<<endl;
}