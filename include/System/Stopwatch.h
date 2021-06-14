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
    [[nodiscard]] bool IsRunning() const noexcept;
    [[nodiscard]] int64_t GetElapsedSeconds() const;
    [[nodiscard]] int64_t GetElapsedMilliseconds() const;
    [[nodiscard]] int64_t GetElapsedNanoseconds() const;

private:
    int64_t _oldTime;
};

inline Stopwatch::Stopwatch() noexcept :
    _oldTime(0)
{
}

inline void Stopwatch::Start()
{
    _oldTime = std::chrono::steady_clock::now().time_since_epoch().count();
}

inline void Stopwatch::Reset()
{
    _oldTime = std::chrono::steady_clock::now().time_since_epoch().count();
}

inline void Stopwatch::Stop() noexcept
{
    _oldTime = 0;
}

inline bool Stopwatch::IsRunning() const noexcept
{
    return _oldTime > 0;
}

inline int64_t Stopwatch::GetElapsedSeconds() const
{
    return static_cast<int64_t>(this->GetElapsedMilliseconds() * 0.001);
}

inline int64_t Stopwatch::GetElapsedMilliseconds() const
{
    return static_cast<int64_t>(this->GetElapsedNanoseconds() * 0.000001);
}

inline int64_t Stopwatch::GetElapsedNanoseconds() const
{
    return (this->IsRunning()) ? std::chrono::steady_clock::now().time_since_epoch().count() - _oldTime : 0;
}

CS2CPP_NAMESPACE_END
