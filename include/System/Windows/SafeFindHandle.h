#pragma once

#include <algorithm>

#include "Windows.h"

CS2CPP_NAMESPACE_BEGIN

class SafeFindHandle final
{
public:
    constexpr SafeFindHandle() noexcept = default;
    constexpr explicit SafeFindHandle(HANDLE handle) noexcept;
    SafeFindHandle(const SafeFindHandle& rhs) = delete;
    SafeFindHandle(SafeFindHandle&& rhs) noexcept;

public:
    ~SafeFindHandle();

public:
    SafeFindHandle& operator=(const SafeFindHandle& rhs) = delete;
    SafeFindHandle& operator=(SafeFindHandle&& rhs) noexcept;
    constexpr bool operator==(const SafeFindHandle& rhs) const noexcept;
    constexpr bool operator!=(const SafeFindHandle& rhs) const noexcept;
    bool operator==(std::nullptr_t rhs) const noexcept;
    bool operator!=(std::nullptr_t rhs) const noexcept;
    operator HANDLE() const noexcept;
    operator bool() const noexcept;

private:
    HANDLE handle_ = INVALID_HANDLE_VALUE;
};

constexpr SafeFindHandle::SafeFindHandle(HANDLE handle) noexcept :
    handle_(handle)
{
}

inline SafeFindHandle::SafeFindHandle(SafeFindHandle&& rhs) noexcept :
    handle_(rhs.handle_)
{
    rhs.handle_ = INVALID_HANDLE_VALUE;
}

inline SafeFindHandle::~SafeFindHandle()
{
    if (handle_ != nullptr)
    {
        FindClose(handle_);
    }
}

inline SafeFindHandle& SafeFindHandle::operator=(SafeFindHandle&& rhs) noexcept
{
    std::swap(handle_, rhs.handle_);
    return *this;
}

constexpr bool SafeFindHandle::operator==(const SafeFindHandle& rhs) const noexcept
{
    return handle_ == rhs.handle_;
}

constexpr bool SafeFindHandle::operator!=(const SafeFindHandle& rhs) const noexcept
{
    return handle_ != rhs.handle_;
}

inline bool SafeFindHandle::operator==(std::nullptr_t) const noexcept
{
    return handle_ == INVALID_HANDLE_VALUE;
}

inline bool SafeFindHandle::operator!=(std::nullptr_t) const noexcept
{
    return handle_ != INVALID_HANDLE_VALUE;
}

inline SafeFindHandle::operator HANDLE() const noexcept
{
    return handle_;
}

inline SafeFindHandle::operator bool() const noexcept
{
    return handle_ != INVALID_HANDLE_VALUE;
}

CS2CPP_NAMESPACE_END