#include "../include/crypto_provider.hpp"

HashFunctionCtx::Uptr ara::crypto::cryp::CryptoProvider::CreateHashFunctionCtx(ara::crypto::AlgId algId)
{
    HashFunctionCtx H(algId);
    HashFunctionCtx::Uptr HashPtr = std::make_unique<HashFunctionCtx>(H);
    return HashPtr;
}