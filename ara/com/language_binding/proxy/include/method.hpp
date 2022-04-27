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
                        this->Delegate->SendRequest(this->ID, payload);
                        stringstream out_ss = this->Delegate->ReceiveMessage();
                        Deserializer2 D;
                        D.deserialize(out_ss, out);
                        this -> Delegate ->CloseConnection();
                    }
                    template <typename output>
                    void process_method_call(output &out)
                    {
                        stringstream payload;
                        cout << "Before Send" << endl;
                        this->Delegate->SendRequest(this->ID, payload);
                        cout << "After Send" << endl;
                        stringstream out_ss = this->Delegate->ReceiveMessage();
                        cout << "After Rec" << endl;
                        Deserializer2 D;
                        D.deserialize(out_ss, out);
                        this -> Delegate ->CloseConnection();
                    }
                };
            }
        }
    }
}