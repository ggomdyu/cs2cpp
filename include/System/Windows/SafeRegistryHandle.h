#pragma once

#include <algorithm>

#include "Windows.h"

CS2CPP_NAMESPACE_BEGIN

class SafeRegistryHandle final
{
public:
    constexpr SafeRegistryHandle() noexcept = default;
    constexpr explicit SafeRegistryHandle(HKEY keyHandle) noexcept;
    SafeRegistryHandle(const SafeRegistryHandle& rhs) = delete;
    SafeRegistryHandle(SafeRegistryHandle&& rhs) noexcept;

public:
    ~SafeRegistryHandle();

public:
    SafeRegistryHandle& operator=(const SafeRegistryHandle& rhs) = delete;
    SafeRegistryHandle& operator=(SafeRegistryHandle&& rhs) noexcept;
    [[nodiscard]] constexpr bool operator==(const SafeRegistryHandle& rhs) const noexcept;
    [[nodiscard]] constexpr bool operator!=(const SafeRegistryHandle& rhs) const noexcept;
    [[nodiscard]] constexpr bool operator==(std::nullptr_t rhs) const noexcept;
    [[nodiscard]] constexpr bool operator!=(std::nullptr_t rhs) const noexcept;
    [[nodiscard]] operator PHKEY() noexcept;
    [[nodiscard]] operator HKEY() const noexcept;

private:
    HKEY _handle = nullptr;
};

constexpr SafeRegistryHandle::SafeRegistryHandle(HKEY keyHandle) noexcept :
    _handle(keyHandle)
{
}

inline SafeRegistryHandle::SafeRegistryHandle(SafeRegistryHandle&& rhs) noexcept :
    _handle(rhs._handle)
{
    rhs._handle = nullptr;
}

inline SafeRegistryHandle::~SafeRegistryHandle()
{
    if (_handle != nullptr)
    {
        CloseHandle(_handle);
    }
}

inline SafeRegistryHandle& SafeRegistryHandle::operator=(SafeRegistryHandle&& rhs) noexcept
{
    std::swap(_handle, rhs._handle);
    return *this;
}

constexpr bool SafeRegistryHandle::operator==(const SafeRegistryHandle& rhs) const noexcept
{
    return _handle == rhs._handle;
}

constexpr bool SafeRegistryHandle::operator!=(const SafeRegistryHandle& rhs) const noexcept
{
    return _handle != rhs._handle;
}

constexpr bool SafeRegistryHandle::operator==(std::nullptr_t) const noexcept
{
    return _handle == nullptr;
}

constexpr bool SafeRegistryHandle::operator!=(std::nullptr_t) const noexcept
{
    return _handle != nullptr;
}

inline SafeRegistryHandle::operator PHKEY() noexcept
{
    return &_handle;
}

inline SafeRegistryHandle::operator HKEY() const noexcept
{
    return _handle;
}

CS2CPP_NAMESPACE_END