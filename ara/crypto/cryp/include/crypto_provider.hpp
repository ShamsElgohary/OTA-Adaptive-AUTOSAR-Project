#pragma once
#include "hash_function_ctx.hpp"
#include "types.hpp"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class CryptoProvider
            {
            public:
            
                HashFunctionCtx::Uptr CreateHashFunctionCtx(AlgId algId);

            private:
            };
        }
    }
}