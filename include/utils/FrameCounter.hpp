#ifndef FRAMECOUNTER_HPP
#define FRAMECOUNTER_HPP

class FrameCounter
{
public:
    FrameCounter() = default;

    float GetFPS() const;

    void Update(float deltaTime);

private:
    float GetAverage() const;

    void StoreSample(float sample);

    float FrameTimes[5] = {};
    size_t Index = 0;
    size_t Count = 0;
};

#endif // FRAMECOUNTER_HPP
