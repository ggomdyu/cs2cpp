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
    [[nodiscard]] constexpr bool operator==(const SafeFindHandle& rhs) const noexcept;
    [[nodiscard]] constexpr bool operator!=(const SafeFindHandle& rhs) const noexcept;
    [[nodiscard]] bool operator==(std::nullptr_t rhs) const noexcept;
    [[nodiscard]] bool operator!=(std::nullptr_t rhs) const noexcept;
    [[nodiscard]] operator HANDLE() const noexcept;

private:
    HANDLE _handle = INVALID_HANDLE_VALUE;
};

constexpr SafeFindHandle::SafeFindHandle(HANDLE handle) noexcept :
    _handle(handle)
{
}

inline SafeFindHandle::SafeFindHandle(SafeFindHandle&& rhs) noexcept :
    _handle(rhs._handle)
{
    rhs._handle = INVALID_HANDLE_VALUE;
}

inline SafeFindHandle::~SafeFindHandle()
{
    if (_handle != nullptr)
    {
        FindClose(_handle);
    }
}

inline SafeFindHandle& SafeFindHandle::operator=(SafeFindHandle&& rhs) noexcept
{
    std::swap(_handle, rhs._handle);
    return *this;
}

constexpr bool SafeFindHandle::operator==(const SafeFindHandle& rhs) const noexcept
{
    return _handle == rhs._handle;
}

constexpr bool SafeFindHandle::operator!=(const SafeFindHandle& rhs) const noexcept
{
    return _handle != rhs._handle;
}

inline bool SafeFindHandle::operator==(std::nullptr_t) const noexcept
{
    return _handle == INVALID_HANDLE_VALUE;
}

inline bool SafeFindHandle::operator!=(std::nullptr_t) const noexcept
{
    return _handle != INVALID_HANDLE_VALUE;
}

inline SafeFindHandle::operator HANDLE() const noexcept
{
    return _handle;
}

CS2CPP_NAMESPACE_END