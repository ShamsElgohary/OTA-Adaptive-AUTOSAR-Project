#pragma once
#include <vector>
#include <cstddef>
#include <cryptopp/sha.h>
#include <memory>
#include "types.hpp"


using namespace CryptoPP;
namespace ara
{
    namespace crypto
    {
        namespace cryp
        {

            enum HashCtx_AlgID:uint64_t {SHA1_ID, SHA224_ID};

            class HashFunctionCtx
            {
            private: 
                friend class CryptoProvider;
                HashTransformation* h = nullptr;
                long digest_length;
                unsigned char *digest = nullptr;
                HashFunctionCtx(AlgId AlgID);

            public:

                using Uptr = std::unique_ptr<HashFunctionCtx>;
                void Start ();
                void Update (ReadOnlyMemRegion in, int length);
                std::vector <byte> Finish();           
                std::vector <byte> GetDigest (std::size_t offset=0);
            };
        }
    }
}