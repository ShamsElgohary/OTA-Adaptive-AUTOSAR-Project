#pragma once
#include <vector>
#include <array>
#include "serviceProxy.hpp"
#include "method.hpp"
#include "types.hpp"
using namespace std;
namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            namespace proxy
            {
                enum class PackageManagerStatusType : uint8_t
                {
                    kCleaningUp = 7U,
                    kRolledBack = 6U,
                    kRollingBack = 5U,
                    kActivated = 4U,
                    kActivating = 3U,
                    kProcessing = 2U,
                    kReady = 1U,
                    kIdle = 0U,
                    kVerifying = 8U,
                };
                enum class OperationResultType : uint8_t
                {
                    kSuccess = 0U,
                    kInsufficientMemory = 1U,
                    kIncorrectBlock = 2U,
                    kIncorrectSize = 3U,
                    kInvalidTransferId = 4U,
                    kOperationNotPermitted = 5U,
                    kIncorrectBlockSize = 30U,
                    kInsufficientData = 6U,
                };
                using TransferIdType = std::array<uint8_t, 16>;
                using ByteVectorType = std::vector<uint8_t>;
                using SwClusterInfoVectorType = std::vector<SwClusterInfoType>;

                struct FinishOutput
                {
                    OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };

                struct TransferStartInput
                {
                    uint64_t Size;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &Size;
                    }
                    friend class boost::serialization::access;
                };

                struct TransferStartOutput
                {
                    TransferIdType id;

                    uint32_t BlockSize;

                    OperationResultType TransferStartResult;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &id;
                        ar &BlockSize;
                        ar &TransferStartResult;
                    }
                    friend class boost::serialization::access;
                };

                struct TransferDataInput
                {
                    TransferIdType id;
                    ByteVectorType data;
                    uint64_t blockCounter;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &id;
                        ar &data;
                        ar &blockCounter;
                    }
                    friend class boost::serialization::access;
                };

                struct TransferDataOutput
                {
                    OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };

                struct TransferExitInput
                {
                    TransferIdType id;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &id;
                    }
                    friend class boost::serialization::access;
                };

                struct TransferExitOutput
                {
                    OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };

                struct TransferDeleteInput
                {
                    TransferIdType id;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &id;
                    }
                    friend class boost::serialization::access;
                };

                struct TransferDeleteOutput
                {
                    OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };

                struct ProcessSwPackageInput
                {
                    TransferIdType id;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &id;
                    }
                    friend class boost::serialization::access;
                };

                struct ProcessSwPackageOutput
                {
                    OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };

                struct ActivateOutput
                {
                    OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };

                struct GetSwClusterInfoOutput
                {
                    SwClusterInfoVectorType vectorOfClusterInfo;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &vectorOfClusterInfo;
                    }
                    friend class boost::serialization::access;
                };

                struct RollbackOutput
                {
                    OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };

                struct RevertProcessedSwPackagesOutput
                {
                    OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };

                struct CurrentStatusField
                {
                    PackageManagerStatusType CurrentStatus;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &CurrentStatus;
                    }
                    friend class boost::serialization::access;
                };

                namespace methods
                {
                    class Finish : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        Finish(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 10) {}
                        FinishOutput operator()()
                        {
                            FinishOutput out;
                            process_method_call<FinishOutput>(out);
                            ara::com::AddMethodCall(10, "Finish", ara::com::MethodType::Proxy_Method, 2, Cluster_Name);
                            return out;
                        }
                    };
                    class TransferStart : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        TransferStart(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 1) {}
                        TransferStartOutput operator()(uint64_t Size)
                        {
                            TransferStartInput in;
                            in.Size = Size;
                            TransferStartOutput out;
                            process_method_call<TransferStartOutput, TransferStartInput>(in, out);
                            ara::com::AddMethodCall(1, "TransferStart", ara::com::MethodType::Proxy_Method, 2, Cluster_Name);
                            return out;
                        }
                    };
                    class TransferData : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        TransferData(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 2) {}
                        TransferDataOutput operator()(TransferIdType id, ByteVectorType data, uint64_t blockCounter)
                        {
                            TransferDataInput in;
                            in.id = id;
                            in.data = data;
                            in.blockCounter = blockCounter;
                            TransferDataOutput out;
                            process_method_call<TransferDataOutput, TransferDataInput>(in, out);
                            ara::com::AddMethodCall(2, "TransferData", ara::com::MethodType::Proxy_Method, 2, Cluster_Name);
                            return out;
                        }
                    };
                    class TransferExit : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        TransferExit(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 3) {}
                        TransferExitOutput operator()(TransferIdType id)
                        {
                            TransferExitInput in;
                            in.id = id;
                            TransferExitOutput out;
                            process_method_call<TransferExitOutput, TransferExitInput>(in, out);
                            ara::com::AddMethodCall(3, "TransferExit", ara::com::MethodType::Proxy_Method, 2, Cluster_Name);
                            return out;
                        }
                    };
                    class TransferDelete : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        TransferDelete(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 4) {}
                        TransferDeleteOutput operator()(TransferIdType id)
                        {
                            TransferDeleteInput in;
                            in.id = id;
                            TransferDeleteOutput out;
                            process_method_call<TransferDeleteOutput, TransferDeleteInput>(in, out);
                            ara::com::AddMethodCall(4, "TransferDelete", ara::com::MethodType::Proxy_Method, 2, Cluster_Name);
                            return out;
                        }
                    };
                    class ProcessSwPackage : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        ProcessSwPackage(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 5) {}
                        ProcessSwPackageOutput operator()(TransferIdType id)
                        {
                            ProcessSwPackageInput in;
                            in.id = id;
                            ProcessSwPackageOutput out;
                            process_method_call<ProcessSwPackageOutput, ProcessSwPackageInput>(in, out);
                            ara::com::AddMethodCall(5, "ProcessSwPackage", ara::com::MethodType::Proxy_Method, 2, Cluster_Name);
                            return out;
                        }
                    };
                    class Activate : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        Activate(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 6) {}
                        ActivateOutput operator()()
                        {
                            ActivateOutput out;
                            process_method_call<ActivateOutput>(out);
                            ara::com::AddMethodCall(6, "Activate", ara::com::MethodType::Proxy_Method, 2, Cluster_Name);
                            return out;
                        }
                    };
                    class GetSwClusterInfo : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        GetSwClusterInfo(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 7) {}
                        GetSwClusterInfoOutput operator()()
                        {
                            GetSwClusterInfoOutput out;
                            process_method_call<GetSwClusterInfoOutput>(out);
                            ara::com::AddMethodCall(7, "GetSwClusterInfo", ara::com::MethodType::Proxy_Method, 2, Cluster_Name);
                            return out;
                        }
                    };
                    class Rollback : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        Rollback(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 8) {}
                        RollbackOutput operator()()
                        {
                            RollbackOutput out;
                            process_method_call<RollbackOutput>(out);
                            ara::com::AddMethodCall(8, "Rollback", ara::com::MethodType::Proxy_Method, 2, Cluster_Name);
                            return out;
                        }
                    };
                    class RevertProcessedSwPackages : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        RevertProcessedSwPackages(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 9) {}
                        RevertProcessedSwPackagesOutput operator()()
                        {
                            RevertProcessedSwPackagesOutput out;
                            process_method_call<RevertProcessedSwPackagesOutput>(out);
                            ara::com::AddMethodCall(9, "RevertProcessedSwPackages", ara::com::MethodType::Proxy_Method, 2, Cluster_Name);
                            return out;
                        }
                    };
                }
                namespace fields
                {
                    class GetCurrentStatus : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        GetCurrentStatus(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 11) {}
                        CurrentStatusField operator()()
                        {
                            CurrentStatusField out;
                            process_method_call<CurrentStatusField>(out);
                            ara::com::AddMethodCall(11, "GetCurrentStatus", ara::com::MethodType::Proxy_Method, 2, Cluster_Name);
                            return out;
                        }
                    };

                }
                class PackageManagerProxy : public ara::com::proxy::ProxyBase
                {
                public:
                    PackageManagerProxy(HandleType handle) : ProxyBase(handle), GetCurrentStatus(handle.ptr2bindingProtocol), Finish(handle.ptr2bindingProtocol), TransferStart(handle.ptr2bindingProtocol), TransferData(handle.ptr2bindingProtocol), TransferExit(handle.ptr2bindingProtocol), TransferDelete(handle.ptr2bindingProtocol), ProcessSwPackage(handle.ptr2bindingProtocol), Activate(handle.ptr2bindingProtocol), GetSwClusterInfo(handle.ptr2bindingProtocol), Rollback(handle.ptr2bindingProtocol), RevertProcessedSwPackages(handle.ptr2bindingProtocol)
                    {
                    }
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService()
                    {
                        return ara::com::proxy::ProxyBase::FindService(CUSTOMIZED_PROJECT_PATH + "executables/ucm/0.1/etc/service_manifest.json", Cluster_Name, 2);
                    }
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(ara::com::InstanceIdentifier InstanceID)
                    {
                        return ara::com::proxy::ProxyBase::FindService(CUSTOMIZED_PROJECT_PATH + "executables/ucm/0.1/etc/service_manifest.json", Cluster_Name, 2, InstanceID);
                    }
                    methods::Finish Finish;
                    methods::TransferStart TransferStart;
                    methods::TransferData TransferData;
                    methods::TransferExit TransferExit;
                    methods::TransferDelete TransferDelete;
                    methods::ProcessSwPackage ProcessSwPackage;
                    methods::Activate Activate;
                    methods::GetSwClusterInfo GetSwClusterInfo;
                    methods::Rollback Rollback;
                    methods::RevertProcessedSwPackages RevertProcessedSwPackages;
                    fields::GetCurrentStatus GetCurrentStatus;
                };
            }
        }
    }
}
