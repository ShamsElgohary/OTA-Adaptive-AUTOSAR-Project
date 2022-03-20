#pragma once
#include <iostream>
#include "../../../language_binding/Services/include/serviceProxy.hpp"
using namespace std;

namespace ara
{
    namespace com
    {
        namespace skeleton
        {
            namespace method
            {
                class input
                {
                };

                class output
                {
                };

                
                class methodBase
                {
                public:
                    virtual output processMethod(input &ip);
                };
            }
        }
    }
}


class startTransfer : public methodBase
{
    class Adjustinput : public input{
        public:
        Position p;
    }

    class Adjustoutput : public output{
        public:
        Position p;
    }

    Output processMethod(INput ) override{
        startTransfer()
    }
  }
