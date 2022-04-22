#pragma once

#include "../../com/language_binding/skeleton/include/serviceskeleton.hpp"
#include "../../com/include/types.hpp"
#include "serialization_simple.hpp"
using namespace std;
namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            class PackageManagerSkeleton : public ara::com::skeleton::skeletonBase
            {
            public:
                /** Add **/
                struct AddInput
                {
                    uint64_t a;
                    uint64_t b;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &a;
                        ar &b;
                    }
                    friend class boost::serialization::access;
                };
                ///////////////////////////// PROBLEM //////////////////////////////////
                struct AddOutput
                {
                    // Size (in bytes) of the Software Package to be transferred
                    uint64_t Sum; // Size of the blocks to be received with TransferData method // Success or Failure
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &Sum;
                    }
                    friend class boost::serialization::access;
                };

                /* PACKAGE MANAGER METHODS */

                virtual AddOutput Add(uint64_t a, uint64_t b) = 0;

                void handleMethod() override
                {
                    int methodID;
                    this->ptr2bindingProtocol->ServerListen();
                    stringstream payload = this->ptr2bindingProtocol->ReceiveMessage(methodID);

                    switch (methodID)
                    {
                    case 1:
                    {
                        AddInput ip;
                        Deserializer2 D;
                        D.deserialize(payload, ip);

                        AddOutput op = Add(ip.a, ip.b);
                        cout << "SUM: " << op.Sum << endl;
                        Serializer2 S;
                        stringstream result;
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendRequest(1, result);
                        
                        break;
                    }
                    }
                    this->ptr2bindingProtocol->CloseConnection();
                }

                PackageManagerSkeleton(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : skeletonBase(1, I_id, mode)
                {
                }
            };
        }
    }
}

class Imp : public ara::ucm::pkgmgr::PackageManagerSkeleton
{
public:
    Imp(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : PackageManagerSkeleton(I_id, mode)
    {
    }

    inline AddOutput Add(uint64_t a, uint64_t b) override
    {
        AddOutput out;
        out.Sum = a + b;
        return out;
    }
};
