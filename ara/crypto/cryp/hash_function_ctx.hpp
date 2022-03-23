#pragma once
#include <vector>
#include <cstddef>
#include "crypto_context.h"
#include <cryptopp/sha.h>

using namespace CryptoPP;
namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class HashFunctionCtx : public CryptoContext
            {
            private: 
                
            public:
                virtual void Start () noexcept=0;
                virtual void Update (ReadOnlyMemRegion in)noexcept=0;
                virtual std::vector<std::Byte>  Finish() noexcept=0;                
                virtual std::vector<std::Byte>  GetDigest (std::size_t offset=0) const noexcept=0;
                //template <typename Alloc = <implementation-defined>>ara::core::Result<ByteVector<Alloc> > GetDigest (std::size_t offset=0)const noexcept; 
            };
        }
    }
}