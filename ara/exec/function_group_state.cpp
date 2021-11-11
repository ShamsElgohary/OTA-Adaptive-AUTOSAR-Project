#include "function_group_state.h"

using namespace ara::exec ;


FunctionGroupState::CtorToken Preconstruct(string functionGroup, string newState)
{
	FunctionGroupState::CtorToken token ;
	token.fng_name =functionGroup;
    token.state = newState ;
	return token;
}

FunctionGroupState::FunctionGroupState(FunctionGroupState::CtorToken && token)
{
	fg_name = token.fng_name;
	fg_newState = token.state;
}


