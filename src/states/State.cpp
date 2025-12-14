#include "stdafx.hpp"
#include "states/State.hpp"

IState::IState(StateContext& context)
    : Context(context), bWantsToBeDetached(false)
{}

void IState::MarkToBeDetached()
{
    bWantsToBeDetached = true;
}
