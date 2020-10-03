#include "System/Environment.h"
#include "System/Timer.h"

CS2CPP_NAMESPACE_BEGIN

Timer::Timer() noexcept :
    Timer(false)
{
}

Timer::Timer(bool isAutoReset) noexcept :
    Timer(0, isAutoReset)
{
}

Timer::Timer(int64_t interval, bool isAutoReset) noexcept :
    interval_(interval),
    prevTime_(0),
    isAutoReset_(isAutoReset),
    isEnabled_(false)
{
}

void Timer::Start()
{
    isEnabled_ = true;
    prevTime_ = Environment::TickCount64();
}

void Timer::Stop()
{
    isEnabled_ = false;
}

void Timer::SetAutoReset(bool isAutoReset)
{
    isAutoReset_ = isAutoReset;
}

bool Timer::IsAutoReset() const noexcept
{
    return isAutoReset_;
}

void Timer::SetInterval(int64_t interval)
{
    interval_ = interval;
}

int64_t Timer::GetInterval() const noexcept
{
    return interval_;
}

void Timer::Update()
{
    if (!isEnabled_)
    {
        return;
    }

    auto currentTime = Environment::TickCount64();
    if ((currentTime - prevTime_) <= interval_)
    {
        return;
    }

    if (OnTimeElapsed != nullptr)
    {
        OnTimeElapsed();
    }

    if (isAutoReset_)
    {
        prevTime_ = currentTime;
    }
    else
    {
        Stop();
    }
}

CS2CPP_NAMESPACE_END
