#include "../include/hash_function_ctx.hpp"
using namespace ara::crypto::cryp;
HashFunctionCtx::HashFunctionCtx(ara::crypto::AlgId AlgID)
{
    if (AlgID == SHA1_ID)
    {
        h = new SHA1();
        digest_length = h->DigestSize();
        digest = new byte[digest_length];
    }

    else if (AlgID == SHA224_ID)
    {
        h = new SHA224();
        digest_length =h->DigestSize();
        digest = new byte[digest_length];
    }
}

void HashFunctionCtx::Start ()
{   
    h->Restart();
}
void HashFunctionCtx::Update (ReadOnlyMemRegion in, int length)
{
    h->Update(in, length);
}
std::vector<unsigned char> HashFunctionCtx::Finish()
{
    h->Final(digest);
    unsigned long long length=digest_length;
    std::vector <unsigned char> result(digest,digest+length);
    return result;
}            
std::vector<byte> HashFunctionCtx::GetDigest (std::size_t offset)
{
    byte* temp=new byte[digest_length];
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
