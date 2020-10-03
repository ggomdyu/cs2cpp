#pragma once

#include <chrono>

CS2CPP_NAMESPACE_BEGIN

class Stopwatch final
{
public:
    Stopwatch() noexcept;

public:
    void Start();
    void Reset();
    void Stop() noexcept;
    bool IsRunning() const noexcept;
    int64_t GetElapsedSeconds() const;
    int64_t GetElapsedMilliseconds() const;
    int64_t GetElapsedNanoseconds() const;

private:
    int64_t oldTime_;
};

inline Stopwatch::Stopwatch() noexcept :
    oldTime_(0)
{
}

inline void Stopwatch::Start()
{
    oldTime_ = std::chrono::steady_clock::now().time_since_epoch().count();
}

inline void Stopwatch::Reset()
{
    oldTime_ = std::chrono::steady_clock::now().time_since_epoch().count();
}

inline void Stopwatch::Stop() noexcept
{
    oldTime_ = 0;
}

inline bool Stopwatch::IsRunning() const noexcept
{
    return oldTime_ > 0;
}

inline int64_t Stopwatch::GetElapsedSeconds() const
{
    return static_cast<int64_t>(GetElapsedMilliseconds() * 0.001);
}

inline int64_t Stopwatch::GetElapsedMilliseconds() const
{
    return static_cast<int64_t>(GetElapsedNanoseconds() * 0.000001);
}

inline int64_t Stopwatch::GetElapsedNanoseconds() const
{
    return (IsRunning()) ? std::chrono::steady_clock::now().time_since_epoch().count() - oldTime_ : 0;
}

CS2CPP_NAMESPACE_END
