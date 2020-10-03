#pragma once

#include <algorithm>

#include "Windows.h"

CS2CPP_NAMESPACE_BEGIN

class SafeHandle final
{
public:
    constexpr SafeHandle() noexcept = default;
    constexpr explicit SafeHandle(HANDLE handle) noexcept;
    SafeHandle(const SafeHandle& rhs) = delete;
    SafeHandle(SafeHandle&& rhs) noexcept;

public:
    ~SafeHandle();

public:
    SafeHandle& operator=(const SafeHandle& rhs) = delete;
    SafeHandle& operator=(SafeHandle&& rhs) noexcept;
    constexpr bool operator==(const SafeHandle& rhs) const noexcept;
    constexpr bool operator!=(const SafeHandle& rhs) const noexcept;
    bool operator==(std::nullptr_t rhs) const noexcept;
    bool operator!=(std::nullptr_t rhs) const noexcept;
    operator HANDLE() const noexcept;

private:
    HANDLE handle_ = INVALID_HANDLE_VALUE;
};

constexpr SafeHandle::SafeHandle(HANDLE handle) noexcept :
    handle_(handle)
{
}

inline SafeHandle::SafeHandle(SafeHandle&& rhs) noexcept :
    handle_(rhs.handle_)
{
    rhs.handle_ = INVALID_HANDLE_VALUE;
}

inline SafeHandle::~SafeHandle()
{
    if (handle_ != nullptr)
    {
        CloseHandle(handle_);
    }
}

inline SafeHandle& SafeHandle::operator=(SafeHandle&& rhs) noexcept
{
    std::swap(handle_, rhs.handle_);
    return *this;
}

constexpr bool SafeHandle::operator==(const SafeHandle& rhs) const noexcept
{
    return handle_ == rhs.handle_;
}

constexpr bool SafeHandle::operator!=(const SafeHandle& rhs) const noexcept
{
    return handle_ != rhs.handle_;
}

inline bool SafeHandle::operator==(std::nullptr_t) const noexcept
{
    return handle_ == INVALID_HANDLE_VALUE;
}

inline bool SafeHandle::operator!=(std::nullptr_t) const noexcept
{
    return handle_ != INVALID_HANDLE_VALUE;
}

inline SafeHandle::operator HANDLE() const noexcept
{
    return handle_;
}

CS2CPP_NAMESPACE_END