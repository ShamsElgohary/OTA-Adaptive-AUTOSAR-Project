#include "../../../include/types.hpp"

namespace ara
{
    namespace com
    {
        namespace proxy
        {
            class HandleType
            {
            protected:
                //uint32_t VersionMajor;
                //uint32_t VersionMinor;

                //  ara::com::InterfaceIdentifier InterfaceID;
                ara::com::serviceIdentifierType serviceID;
                ara::com::InstanceIdentifier InstanceID;
                uint32_t ip;
                uint32_t port;
                //std::shared_ptr<ara::com::NetworkBindingBase> network_binding;
                ara::com::BindingProtocol bindingProtocol;

            public:
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

                friend class proxy::ProxyBase;
            };

            class SOMEIP : public HandleType
            {
            public:
                SOMEIP() : bindingProtocol(BindingProtocol::someip) {}
                /*********************
                 * someIP functions  *
                 * *******************/
            };

            class DDS : public HandleType
            {
            public:
                DDS() : bindingProtocol(BindingProtocol::dds) {}
                /*********************
                 * DDS functions     *
                 * *******************/
            };

            class HandleTypeFactory
            {
            public:
                static HandleType &create(BindingProtocol bP);
            };
        }
    }
}