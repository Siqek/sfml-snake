#include "stdafx.hpp"
#include "utils/FPSCounter.hpp"

FPSCounter::FPSCounter()
    : frames{}, totalTime(0.f)
{}

void FPSCounter::update(const float &dt)
{
    this->frames.push_front(dt);
    this->totalTime += dt;

    while (this->totalTime > 1.f && !this->frames.empty())
    {
        this->totalTime -= this->frames.back();
        this->frames.pop_back();
    }
}
