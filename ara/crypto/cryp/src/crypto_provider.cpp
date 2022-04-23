#include "../include/crypto_provider.hpp"
using namespace ara::crypto::cryp;
HashFunctionCtx::Uptr CryptoProvider::CreateHashFunctionCtx(ara::crypto::AlgId algId)
{
    HashFunctionCtx H(algId);
    HashFunctionCtx::Uptr HashPtr = std::make_unique<HashFunctionCtx>(H);
    return HashPtr;
}