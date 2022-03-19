#pragma once

#include "types.hpp"
#include "../../com/language_binding/proxy/include/serviceProxy.hpp"
#include "../../com/language_binding/proxy/include/method.hpp"
#include "../../com/language_binding/proxy/include/field.hpp"

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

                struct TransferStartOutput
                {
                    TransferIdType id;                       // Size (in bytes) of the Software Package to be transferred
                    uint32_t BlockSize;                      // Size of the blocks to be received with TransferData method
                    OperationResultType TransferStartResult; // Success or Failure
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
                };

                /** TransferExit **/
                struct TransferExitInput
                {
                    TransferIdType &id;
                };

                struct TransferExitOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** TransferDelete **/
                struct TransferDeleteInput
                {
                    TransferIdType &id;
                };

                struct TransferDeleteOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** ProcessSwPackage **/
                struct ProcessSwPackageInput
                {
                    TransferIdType &id;
                };

                struct ProcessSwPackageOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** Activate **/
                struct ActivateOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** GetSwClusterInfo **/
                struct GetSwClusterInfoOutput
                {
                    vector<ara::ucm::SwClusterInfoType> vectorOfClusterInfo;
                };

                /** Rollback **/
                struct RollbackOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** RevertProcessedSwPackages **/
                struct RevertProcessedSwPackagesOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** Finish **/
                struct FinishOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                struct GetCurrentStatusField
                {
                    PackageManagerStatusType Status;
                };

                namespace methods
                {
                    class TransferStart : public ara::com::proxy::method::MethodBase<TransferStartOutput>
                    {
                    };

                    class TransferData : public ara::com::proxy::method::MethodBase<TransferDataOutput>
                    {
                    };

                    class TransferExit : public ara::com::proxy::method::MethodBase<TransferExitOutput>
                    {
                    };

                    class TransferDelete : public ara::com::proxy::method::MethodBase<TransferDeleteOutput>
                    {
                    };

                    class ProcessSwPackage : public ara::com::proxy::method::MethodBase<ProcessSwPackageOutput>
                    {
                    };

                    class Activate : public ara::com::proxy::method::MethodBase<ActivateOutput>
                    {
                    };

                    class GetSwClusterInfo : public ara::com::proxy::method::MethodBase<GetSwClusterInfoOutput>
                    {
                    };

                    class Rollback : public ara::com::proxy::method::MethodBase<RollbackOutput>
                    {
                    };

                    class RevertProcessedSwPackages : public ara::com::proxy::method::MethodBase<RevertProcessedSwPackagesOutput>
                    {
                    };

                    class Finish : public ara::com::proxy::method::MethodBase<FinishOutput>
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
                            InternalFindService(1);
                        }

                        inline ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(ara::com::InstanceIdentifier InstanceID)
                        {
                            InternalFindService(1, InstanceID);
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
