#pragma once

#include "System/ConsoleKey.h"
#include "System/ConsoleModifiers.h"

CS2CPP_NAMESPACE_BEGIN

struct ConsoleKeyInfo final
{
public:
    constexpr ConsoleKeyInfo() = default;
    constexpr ConsoleKeyInfo(char16_t keyChar, ConsoleKey key, bool shift, bool alt, bool control);

public:
    constexpr bool operator==(const ConsoleKeyInfo& value) const noexcept;
    constexpr bool operator!=(const ConsoleKeyInfo& value) const noexcept;

public:
    constexpr bool Equals(const ConsoleKeyInfo& value) const noexcept;
    constexpr char16_t KeyChar() const noexcept;
    constexpr ConsoleKey Key() const noexcept;
    constexpr ConsoleModifiers Modifiers() const noexcept;
    constexpr int32_t GetHashCode() const noexcept;

private:
    char16_t keyChar_ = 0;
    ConsoleKey key_ = {};
    ConsoleModifiers mods_ = {};
};

constexpr ConsoleKeyInfo::ConsoleKeyInfo(char16_t keyChar, ConsoleKey key, bool shift, bool alt, bool control) :
    keyChar_(keyChar),
    key_(key),
    mods_{}
{
    if (shift)
    {
        mods_ |= ConsoleModifiers::Shift;
    }
    if (alt)
    {
        mods_ |= ConsoleModifiers::Alt;
    }
    if (control)
    {
        mods_ |= ConsoleModifiers::Control;
    }
}

constexpr bool ConsoleKeyInfo::operator==(const ConsoleKeyInfo& value) const noexcept
{
    return Equals(value);
}

constexpr bool ConsoleKeyInfo::operator!=(const ConsoleKeyInfo& value) const noexcept
{
    return !operator==(value);
}

constexpr bool ConsoleKeyInfo::Equals(const ConsoleKeyInfo& value) const noexcept
{
    return keyChar_ == value.keyChar_ && key_ == value.key_ && mods_ == value.mods_;
}

constexpr char16_t ConsoleKeyInfo::KeyChar() const noexcept
{
    return keyChar_;
}

constexpr ConsoleKey ConsoleKeyInfo::Key() const noexcept
{
    return key_;
}

constexpr ConsoleModifiers ConsoleKeyInfo::Modifiers() const noexcept
{
    return mods_;
}

constexpr int32_t ConsoleKeyInfo::GetHashCode() const noexcept
{
    return keyChar_ | (static_cast<int32_t>(key_) << 16) | (static_cast<int32_t>(mods_) << 24);
}

CS2CPP_NAMESPACE_END
