#include "../include/hash_function_ctx.hpp"
#include <iostream>
#include <string>
using namespace CryptoPP;

int main()
{

    std::string msg = "Hello";
    std::string digest;

    std::vector <byte> Digest;

    ara::crypto::cryp::CryptoProvider cryp_provider;
    ara::crypto::cryp::HashFunctionCtx::Uptr hash = cryp_provider.CreateHashFunctionCtx(0);
    hash->Start();
    hash->Update((const byte*)msg.data(), msg.length());
    //(byte*)&digest[0] = hash.Finish();

    std::cout << "Message: " << msg << std::endl;

    std::cout << "Digest: ";
    //StringSource(digest, true, new Redirector(encoder));
    std::cout << std::endl;

    Digest = hash->Finish();
    for (int i = 0; i < Digest.size(); i++)
    {
        std::cout << static_cast<unsigned>(Digest[i]) << "    ";
    }

    /*
    ara::crypto::cryp::HashFunctionCtx H;
    H.Start();
    std::cout << "1" << std::endl;
    byte input[5] = {1,2,3,4,5};
    byte* output = NULL;
    //byte* x = input;

    H.Update(input);
    std::cout << "2" << std::endl;
    output = H.Finish();
    std::cout << "3" << std::endl;
    for (int i = 0; i < 32; i++)
    {
        std::cout << static_cast<unsigned>(output[i]) << "   ";
    }
*/
}