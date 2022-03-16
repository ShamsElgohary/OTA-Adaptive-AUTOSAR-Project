#include "AppProxy.hpp"


void main (void)
{
    auto handles = Name1::Name2::Proxy::AppProxy::FindService();

    Name1::Name2::Proxy::AppProxy P1(handles[0]);

    P1.DoMethod(A1, A2);

    // HOW TO GET OUTPUTS?
}