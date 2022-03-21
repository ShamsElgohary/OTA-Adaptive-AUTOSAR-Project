#pragma once

#include "../../com/language_binding/proxy/include/serviceProxy.hpp"
#include "../../com/language_binding/proxy/include/method.hpp"

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            namespace proxy
            {
                /** TransferStart **/
                struct TransferStartInput
                {
                    uint64_t Size;
                };
                ///////////////////////////// PROBLEM //////////////////////////////////
                struct TransferStartOutput
                {
                    TransferIdType id;                       // Size (in bytes) of the Software Package to be transferred
                    uint32_t BlockSize;                      // Size of the blocks to be received with TransferData method
                    OperationResultType TransferStartResult; // Success or Failure
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &id;
                        ar &BlockSize;
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** TransferData **/
                struct TransferDataInput
                {
                    TransferIdType &id;
                    ByteVectorType data;
                    uint64_t blockCounter;
                };

                struct TransferDataOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** TransferExit **/
                struct TransferExitInput
                {
                    TransferIdType &id;
                };

                struct TransferExitOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** TransferDelete **/
                struct TransferDeleteInput
                {
                    TransferIdType &id;
                };

                struct TransferDeleteOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** ProcessSwPackage **/
                struct ProcessSwPackageInput
                {
                    TransferIdType &id;
                };

                struct ProcessSwPackageOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** Activate **/
                struct ActivateOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };
                ///////////////////////////// PROBLEM //////////////////////////////////
                /** GetSwClusterInfo **/
                struct GetSwClusterInfoOutput
                {
                    vector<ara::ucm::SwClusterInfoType> vectorOfClusterInfo;
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &vectorOfClusterInfo;
                    }
                    friend class boost::serialization::access;
                };

                /** Rollback **/
                struct RollbackOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** RevertProcessedSwPackages **/
                struct RevertProcessedSwPackagesOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** Finish **/
                struct FinishOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                struct GetCurrentStatusField
                {
                    PackageManagerStatusType Status;
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &Status;
                    }
                    friend class boost::serialization::access;
                };

                namespace methods
                {
                    class TransferStart : public ara::com::proxy::method::MethodBase
                    {
                    };

                    class TransferData : public ara::com::proxy::method::MethodBase
                    {
                    };

                    class TransferExit : public ara::com::proxy::method::MethodBase
                    {
                    };

                    class TransferDelete : public ara::com::proxy::method::MethodBase
                    {
                    };

                    class ProcessSwPackage : public ara::com::proxy::method::MethodBase
                    {
                    };

                    class Activate : public ara::com::proxy::method::MethodBase
                    {
                    };

                    class GetSwClusterInfo : public ara::com::proxy::method::MethodBase
                    {
                    };

                    class Rollback : public ara::com::proxy::method::MethodBase
                    {
                    };

                    class RevertProcessedSwPackages : public ara::com::proxy::method::MethodBase
                    {
                    };

                    class Finish : public ara::com::proxy::method::MethodBase
                    {
                    };
                }

                namespace fields
                {
                    class GetCurrentStatus : public ara::com::proxy::field::FieldBase <GetCurrentStatusField>
                }

                class PackageManagerProxy : public ara::com::proxy::ProxyBase
                    {
                    public:
                        inline ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService()
                        {
                            ara::com::proxy::ProxyBase::FindService(1);
                        }

                        inline ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(ara::com::InstanceIdentifier InstanceID)
                        {
                            ara::com::proxy::ProxyBase::FindService(1, InstanceID);
                        }

                        methods::TransferStart TransferStart;
                        methods::TransferData TransferData;
                        methods::TransferExit TransferExit;
                        methods::TransferDelete TransferDelete;
                        methods::ProcessSwPackage ProcessSwPackage;
                        methods::Activate Activate;
                        methods::GetSwClusterInfo GetSwClusterInfo;
                        methods::Rollback Rollback;
                        methods::RevertProcessedSwPackages RevertProcessedSwPackages;
                        methods::Finish Finish;

                        fields::GetCurrentStatus;
                    };
                }

            }

        }
    }
