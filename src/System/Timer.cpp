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
    _interval(interval),
    _prevTime(0),
    _isAutoReset(isAutoReset),
    _isEnabled(false)
{
}

void Timer::Start()
{
    _isEnabled = true;
    _prevTime = Environment::GetTickCount();
}

void Timer::Stop()
{
    _isEnabled = false;
}

void Timer::SetAutoReset(bool isAutoReset)
{
    _isAutoReset = isAutoReset;
}

bool Timer::IsAutoReset() const noexcept
{
    return _isAutoReset;
}

void Timer::SetInterval(int64_t interval)
{
    _interval = interval;
}

int64_t Timer::GetInterval() const noexcept
{
    return _interval;
}

void Timer::Update()
{
    if (!_isEnabled)
    {
        return;
    }

    const auto currentTime = Environment::GetTickCount();
    if ((currentTime - _prevTime) > _interval)
    {
        if (OnTimeElapsed != nullptr)
        {
            OnTimeElapsed();
        }

        if (_isAutoReset)
        {
            _prevTime = currentTime;
        }
        else
        {
            this->Stop();
        }
    }
}

CS2CPP_NAMESPACE_END