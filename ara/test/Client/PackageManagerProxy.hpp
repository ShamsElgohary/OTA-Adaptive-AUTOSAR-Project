#pragma once

#include <vector>
#include <array>
#include "serviceProxy.hpp"
#include "method.hpp"

using namespace std;

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            namespace proxy
            {
                using TransferIdType = array<uint8_t, 16>;
                using ByteVectorType = vector<uint8_t>;
                using SwNameType = string;
                using StrongRevisionLabelString = string;

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

                enum class SwClusterStateType : uint8_t
                {
                    kPresent = 0U,
                    kAdded = 1U,
                    kUpdated = 2U,
                    kRemoved = 3U,

                };

                enum class PackageManagerStatusType : uint8_t
                {
                    kIdle = 0U,
                    kReady = 1U,
                    kProcessing = 2U,
                    kActivating = 3U,
                    kActivated = 4U,
                    kRollingBack = 5U,
                    kRolledBack = 6U,
                    kCleaningUp = 7U,
                    kVerifying = 8U,

                };

                struct SwClusterInfoType
                {
                    SwNameType Name;
                    StrongRevisionLabelString Version;
                    SwClusterStateType State;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &Name;
                        ar &Version;
                        ar &State;
                    }
                    friend class boost::serialization::access;
                };

                /** TransferStart **/
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
                        ar &TransferStartResult;
                    }
                    friend class boost::serialization::access;
                };

                /** TransferData **/
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

                /** TransferExit **/
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

                /** TransferDelete **/
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

                /** ProcessSwPackage **/
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

                /** Activate **/
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
                ///////////////////////////// PROBLEM //////////////////////////////////
                /** GetSwClusterInfo **/
                struct GetSwClusterInfoOutput
                {
                    vector<SwClusterInfoType> vectorOfClusterInfo;

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
                    OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };

                /** RevertProcessedSwPackages **/
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

                /** Finish **/
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
                    public:
                        TransferStart(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 1) {}
                        TransferStartOutput operator()(uint64_t Size)
                        {

                            TransferStartInput in;
                            in.Size = Size;
                            TransferStartOutput out;
                            process_method_call<TransferStartOutput, TransferStartInput>(in, out);
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
                            cout << "Before Process" << endl;
                            process_method_call<ActivateOutput>(out);
                            cout << "After Process" << endl;
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
                            return out;
                        }
                    };

                    class Finish : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        Finish(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 10) {}
                        FinishOutput operator()()
                        {
                            FinishOutput out;
                            process_method_call<FinishOutput>(out);
                            return out;
                        }
                    };
                };
                namespace fields
                {
                    class GetCurrentStatus : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        GetCurrentStatus(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 10) {}
                        GetCurrentStatusField operator()()
                        {
                            GetCurrentStatusField out;
                            process_method_call<GetCurrentStatusField>(out);
                            return out;
                        }
                    };
                }

                class PackageManagerProxy : public ara::com::proxy::ProxyBase
                {
                public:
                    PackageManagerProxy(HandleType handle) : ProxyBase(handle),
                                                             TransferStart(handle.ptr2bindingProtocol),
                                                             TransferData(handle.ptr2bindingProtocol),
                                                             TransferExit(handle.ptr2bindingProtocol),
                                                             TransferDelete(handle.ptr2bindingProtocol),
                                                             ProcessSwPackage(handle.ptr2bindingProtocol),
                                                             Activate(handle.ptr2bindingProtocol),
                                                             GetSwClusterInfo(handle.ptr2bindingProtocol),
                                                             Rollback(handle.ptr2bindingProtocol),
                                                             RevertProcessedSwPackages(handle.ptr2bindingProtocol),
                                                             Finish(handle.ptr2bindingProtocol),
                                                             GetCurrentStatus(handle.ptr2bindingProtocol)
                    {
                    }
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService()
                    {
                        return ara::com::proxy::ProxyBase::FindService("../etc/service_manifest.json",1);
                    }
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(ara::com::InstanceIdentifier InstanceID)
                    {
                        return ara::com::proxy::ProxyBase::FindService("../etc/service_manifest.json",1, InstanceID);
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

                    fields::GetCurrentStatus GetCurrentStatus;
                };
            }
        }
    }
}
