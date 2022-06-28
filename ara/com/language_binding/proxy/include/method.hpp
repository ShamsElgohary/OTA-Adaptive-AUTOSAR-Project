#pragma once
#include <iostream>
#include "serviceProxy.hpp"
#include "serialization_simple.hpp"

using namespace std;
namespace ara
{
    namespace com
    {
        namespace proxy
        {
            namespace method
            {
                class MethodBase
                {
                private:
                    

                public:
                    std::shared_ptr<ara::com::NetworkBindingBase> Delegate;
                    
                    uint16_t ID;
                    MethodBase(std::shared_ptr<ara::com::NetworkBindingBase> ptr, uint16_t id) : Delegate(ptr), ID(id) {}
                    
                    template <typename output, typename T>
                    void process_method_call(T in, output &out)
                    {
                        stringstream payload;
                        Serializer2 S;
                        S.serialize(payload, in);
                        usleep(8000);
                        // REQUEST SENDS A MESSAGES AND RECEIVES RESPONSE
                        stringstream out_ss = this->Delegate->SendRequest(this->ID, payload);
                        Deserializer2 D;
                        D.deserialize(out_ss, out);
                        this -> Delegate ->CloseConnection();
                    }
                    
                    template <typename output>
                    void process_method_call(output &out)
                    {
                        stringstream payload;
                        usleep(8000);
                        // REQUEST SENDS A MESSAGES AND RECEIVES RESPONSE (NO INPUT PAYLOAD IS EMPTY)
                        stringstream out_ss = this->Delegate->SendRequest(this->ID, payload);
                        Deserializer2 D;
                        D.deserialize(out_ss, out);
                        this -> Delegate ->CloseConnection();
                    }
                    
                    void process_method_call()
                    {
                        stringstream payload;
                        usleep(8000);
                        this->Delegate->SendFireAndForget(this->ID, payload);
                    }
                };
            }
        }
    }
}