#pragma once

#include <cstdint>

#include "Delegate.h"

CS2CPP_NAMESPACE_BEGIN

class Timer final
{
public:
    explicit Timer(bool isAutoReset) noexcept;
    Timer() noexcept;
    Timer(int64_t interval, bool isAutoReset) noexcept;

public:
    void Update();
    void Start();
    void Stop();
    void SetAutoReset(bool isAutoReset);
    bool IsAutoReset() const noexcept;
    void SetInterval(int64_t interval);
    int64_t GetInterval() const noexcept;

public:
    Delegate<void()> OnTimeElapsed;

private:
    int64_t interval_;
    int64_t prevTime_;
    bool isAutoReset_;
    bool isEnabled_;
};

CS2CPP_NAMESPACE_END
