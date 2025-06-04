#include "stdafx.hpp"
#include "states/State.hpp"

State::State(StateData* stateData)
    : window(stateData->window), supportedKeys(*stateData->supportedKeys), font(*stateData->font), stateData(stateData), quit(false) {}

State::~State() {}
