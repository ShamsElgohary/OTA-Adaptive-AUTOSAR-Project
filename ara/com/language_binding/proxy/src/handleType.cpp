#include "../include/handleType.hpp"

namespace ara
{
    namespace com
    {
        namespace proxy
        {
            class HandleTypeFactory
            {
            public:
                static HandleType &create(BindingProtocol bP)
                {
                    //check syntax

                    switch (bP)
                    {
                    case BindingProtocol::someip:
                    {
                        HandleType *x = new SOMEIP;
                        HandleType &htype = *x;
                        return htype;
                    }
                    case BindingProtocol::dds:
                    {
                        HandleType *x = new DDS;
                        HandleType &htype = *x;
                        return htype;
                    }
                    }
                }
            };
            class HandleType
            {
            public:
                ara::com::BindingProtocol GetBindingProtocol()
                {
                    return bindingProtocol;
                }
            };
        }
    }
}