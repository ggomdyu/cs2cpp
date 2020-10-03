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
    [[nodiscard]] constexpr bool operator==(const SafeHandle& rhs) const noexcept;
    [[nodiscard]] constexpr bool operator!=(const SafeHandle& rhs) const noexcept;
    [[nodiscard]] bool operator==(std::nullptr_t rhs) const noexcept;
    [[nodiscard]] bool operator!=(std::nullptr_t rhs) const noexcept;
    [[nodiscard]] operator HANDLE() const noexcept;

private:
    HANDLE _handle = INVALID_HANDLE_VALUE;
};

constexpr SafeHandle::SafeHandle(HANDLE handle) noexcept :
    _handle(handle)
{
}

inline SafeHandle::SafeHandle(SafeHandle&& rhs) noexcept :
    _handle(rhs._handle)
{
    rhs._handle = INVALID_HANDLE_VALUE;
}

inline SafeHandle::~SafeHandle()
{
    if (_handle != nullptr)
    {
        CloseHandle(_handle);
    }
}

inline SafeHandle& SafeHandle::operator=(SafeHandle&& rhs) noexcept
{
    std::swap(_handle, rhs._handle);
    return *this;
}

constexpr bool SafeHandle::operator==(const SafeHandle& rhs) const noexcept
{
    return _handle == rhs._handle;
}

constexpr bool SafeHandle::operator!=(const SafeHandle& rhs) const noexcept
{
    return _handle != rhs._handle;
}

inline bool SafeHandle::operator==(std::nullptr_t) const noexcept
{
    return _handle == INVALID_HANDLE_VALUE;
}

inline bool SafeHandle::operator!=(std::nullptr_t) const noexcept
{
    return _handle != INVALID_HANDLE_VALUE;
}

inline SafeHandle::operator HANDLE() const noexcept
{
    return _handle;
}

CS2CPP_NAMESPACE_END