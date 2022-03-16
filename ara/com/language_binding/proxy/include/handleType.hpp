#include "../../../include/types.hpp"

namespace ara
{
    namespace com
    {
        namespace proxy
        {
            class HandleType
            {
            public:
                //uint32_t VersionMajor;
                //uint32_t VersionMinor;

                ara::com::serviceIdentifierType serviceID;
                ara::com::InstanceIdentifier InstanceID;
                uint32_t ip;
                uint32_t port;
                ara::com::NetworkBindingBase* networkBindingPtr;
                //ara::com::BindingProtocol bindingProtocol;

            
                //  inline bool operator==(const HandleType &other) const;

                const ara::com::InstanceIdentifier &GetInstanceId() const;
                ara::com::BindingProtocol GetBindingProtocol();
                /* COPY CONSTRUCTORS */
                HandleType(const HandleType &);
                HandleType &operator=(const HandleType &);

                /* MOVE CONSTRUCTORS */
                HandleType(HandleType &&);
                HandleType &operator=(HandleType &&);

                ~HandleType() noexcept;
                template<typename T>
                virtual SendRequest(uint32_t,T) = 0;

                friend class proxy::ProxyBase;
            };

            class SOMEIP : public HandleType
            {
            public:
                struct output{};
                SOMEIP() : bindingProtocol(BindingProtocol::someip) {}
                /*********************
                 * someIP functions  *
                 * *******************/
              output FindService(InstanceIdentifier instanceID);
            };

            class DDS : public HandleType
            {
            public:
                DDS() : bindingProtocol(BindingProtocol::dds) {}
                /*********************
                 * DDS functions     *
                 * *******************/
                struct output{};
            };

            class HandleTypeFactory
            {
            public:
                static HandleType &create(BindingProtocol bP);
            };
        }
    }
}