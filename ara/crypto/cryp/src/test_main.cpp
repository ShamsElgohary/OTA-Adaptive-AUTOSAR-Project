#include "../include/crypto_provider.hpp"
#include <iostream>
#include <string>
using namespace CryptoPP;
using namespace ara::crypto::cryp;

int main()
{

    std::string msg = "Hello";
    std::string digest;

    std::vector <byte> Digest;

    CryptoProvider cryp_provider;
    HashFunctionCtx::Uptr hash = cryp_provider.CreateHashFunctionCtx(HashCtx_AlgID::SHA224_ID);
    hash->Start();
    hash->Update((const byte*)msg.data(), msg.length());
    
    std::cout << "Message: " << msg << std::endl;

    std::cout << "Digest: ";
    std::cout << std::endl;

    Digest=hash->Finish();
    
    for (int i = 0; i < Digest.size(); i++)
    {
        std::cout << static_cast<unsigned>(Digest[i]) << "    ";
    }
}