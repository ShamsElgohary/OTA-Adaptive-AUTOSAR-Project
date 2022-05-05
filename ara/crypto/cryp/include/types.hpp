#pragma once
#include <cstddef>
#include <cstdint>
#include <cryptopp/sha.h>


namespace ara
{
    namespace crypto
    {
        typedef uint64_t AlgId;
        typedef const unsigned char* ReadOnlyMemRegion;
    }
}