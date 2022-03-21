#pragma once
#include "types.hpp"
//#include "../com/language_binding/skeleton/include/serviceskeleton.hpp"

namespace ara
{
    namespace sm
    {
        class UpdateRequest //: public ara::com::skeleton::Serviceskeleton
        {
        public:
            /** StartUpdateSession **/
            struct StartUpdateSessionOutput
            {
                uint8_t AppError;
            };

            /** PrepareUpdate **/
            struct PrepareUpdateInput
            {
                FunctionGroupList FunctionGroups;
            };

            struct PrepareUpdateOutput
            {
                uint8_t AppError;
            };

            /** VerifyUpdate **/
            struct VerifyUpdateInput
            {
                FunctionGroupList FunctionGroups;
            };

            struct VerifyUpdateOutput
            {
                uint8_t AppError;
            };

            /** StopUpdateSession **/
            struct StopUpdateSessionOutput
            {
                uint8_t AppError;
            };

            virtual uint8_t StartUpdateSession() = 0;
            virtual uint8_t PrepareUpdate(FunctionGroupList FunctionGroups) = 0;
            virtual uint8_t VerifyUpdate(FunctionGroupList FunctionGroups) = 0;
            virtual void StopUpdateSession() = 0;

            /*inline void handleMethod(int methodID) override
            {
                switch (methodID)
                {
                case 1:
                    StartUpdateSessionOutput op = StartUpdateSession();
                    this->ptr2bindingProtocol->send(op);
                    break;
                case 2:
                    PrepareUpdateInput ip;
                    this->ptr2bindingProtocol->receive(ip);
                    PrepareUpdateOutput op = PrepareUpdate(ip.FunctionGroups);
                    this->ptr2bindingProtocol->send(op);
                    break;
                case 3:
                    VerifyUpdateInput ip;
                    this->ptr2bindingProtocol->receive(ip);
                    VerifyUpdateOutput op = VerifyUpdate(ip.FunctionGroups);
                    this->ptr2bindingProtocol->send(op);
                    break;
                case 4:
                    StopUpdateSessionOutput op = StopUpdateSession();
                    this->ptr2bindingProtocol->send(op);
                    break;
                }
            }
            */
        };
    }
}