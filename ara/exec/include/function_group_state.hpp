#pragma once
#include <bits/stdc++.h>
using namespace std;
namespace ara {
   namespace exec
   {
       class FunctionGroupState
       {

       public:

        string fg_name;
        string fg_newState;

       typedef struct {
           string fng_name ;
           string state;
        } CtorToken;

        static FunctionGroupState::CtorToken Preconstruct(string functionGroup, string newState);
        FunctionGroupState(FunctionGroupState::CtorToken &&token);
        ~FunctionGroupState();
       };

   }
}
