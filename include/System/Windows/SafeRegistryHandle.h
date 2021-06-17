#pragma once

#include <algorithm>
#include <memory>

#include "Windows.h"

CS2CPP_NAMESPACE_BEGIN

class SafeRegistryHandle final
{
public:
    explicit SafeRegistryHandle(HKEY handle = nullptr) noexcept;

public:
    bool operator==(const SafeRegistryHandle& rhs) const noexcept;
    bool operator!=(const SafeRegistryHandle& rhs) const noexcept;
    bool operator==(std::nullptr_t rhs) const noexcept;
    bool operator!=(std::nullptr_t rhs) const noexcept;
    operator HKEY() const noexcept;

private:
    std::unique_ptr<std::remove_pointer_t<HKEY>, decltype(&CloseHandle)> _handle;
};

inline SafeRegistryHandle::SafeRegistryHandle(HKEY handle) noexcept :
    _handle(handle, CloseHandle)
{
}

inline bool SafeRegistryHandle::operator==(const SafeRegistryHandle& rhs) const noexcept
{
    return _handle == rhs._handle;
}

inline bool SafeRegistryHandle::operator!=(const SafeRegistryHandle& rhs) const noexcept
{
    return _handle != rhs._handle;
}

inline bool SafeRegistryHandle::operator==(std::nullptr_t) const noexcept
{
    return _handle == nullptr;
}

inline bool SafeRegistryHandle::operator!=(std::nullptr_t) const noexcept
{
    return _handle != nullptr;
}

inline SafeRegistryHandle::operator HKEY() const noexcept
{
    return _handle.get();
}

CS2CPP_NAMESPACE_END