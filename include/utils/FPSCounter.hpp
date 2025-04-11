#ifndef FPSCOUNTER_HPP
#define FPSCOUNTER_HPP

class FPSCounter
{
public:
    FPSCounter();
    ~FPSCounter() = default;

    int getFPS() const { return static_cast<int>(this->frames.size()); };

    void update(const float& dt);
private:
    std::deque<float> frames;
    float totalTime;
};

#endif // FPSCOUNTER_HPP