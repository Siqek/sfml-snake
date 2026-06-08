#include "stdafx.hpp"
#include "utils/FrameCounter.hpp"

float FrameCounter::GetFPS() const
{
    return 1.f / GetAverage();
}

void FrameCounter::Update(float deltaTime)
{
    StoreSample(deltaTime);
}

float FrameCounter::GetAverage() const
{
    if (Count == 0) // Don't devide by 0.
    {
        return 0.f;
    }

    float sum = 0.f;
    for (size_t i = 0; i < Count; ++i)
    {
        sum += FrameTimes[i];
    }
    return sum / static_cast<float>(Count);
}

void FrameCounter::StoreSample(float sample)
{
    FrameTimes[Index] = sample;
    Index = (Index + 1) % 5;
    if (Count < 5)
    {
        ++Count;
    }
}
