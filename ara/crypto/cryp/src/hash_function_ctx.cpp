#include "../include/hash_function_ctx.hpp"

ara::crypto::cryp::HashFunctionCtx::HashFunctionCtx(ara::crypto::AlgId AlgID)
{
    if (AlgID == SHA1_ID)
    {
        h = new SHA1();
        digest_length = 20;
        digest = new byte[digest_length];
    }

    else if (AlgID == SHA224_ID)
    {
        h = new SHA224();
        digest_length = 28;
        digest = new byte[digest_length];
    }
}

void ara::crypto::cryp::HashFunctionCtx::Start ()
{   
    h->Restart();
}
void ara::crypto::cryp::HashFunctionCtx::Update (ReadOnlyMemRegion in, int length)
{
    h->Update(in, length);
}
std::vector <byte> ara::crypto::cryp::HashFunctionCtx::Finish()
{
    h->Final(digest);
    std::vector <byte> result(digest,digest+digest_length);
    return result;
}            
std::vector <byte> ara::crypto::cryp::HashFunctionCtx::GetDigest (std::size_t offset=0)
{
    byte* temp;
    if (digest_length <= offset)
    {
        memset(temp, 0, digest_length * sizeof (*temp));
        std::vector <byte> result(temp,temp+digest_length);
        return result;
    }
    else
    {
        for (int i=offset; i<digest_length; i++)
        {
            temp[i] = digest[i];
        }
        std::vector <byte> result(temp,temp+digest_length);
        return result;
    }
}

//Rem: This is implemented twice: mara f hash ctx w mara f crypto provider l7d manzboto
ara::crypto::cryp::CryptoProvider::CreateHashFunctionCtx(ara::crypto::AlgId algId)
{
    HashFunctionCtx H(algId);
    HashFunctionCtx::Uptr HashPtr = std::make_unique<HashFunctionCtx>(H);
    return HashPtr;
}