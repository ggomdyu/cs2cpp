#pragma once

#include <type_traits>

CS2CPP_NAMESPACE_BEGIN

enum class ConsoleModifiers
{
    Alt = 0x1,
    Shift = 0x2,
    Control = 0x4
};

constexpr ConsoleModifiers operator|(ConsoleModifiers lhs, ConsoleModifiers rhs) noexcept
{
    using T = std::underlying_type_t<ConsoleModifiers>;
    
    return static_cast<ConsoleModifiers>(
        static_cast<T>(lhs) | static_cast<T>(rhs)
    );
}

constexpr ConsoleModifiers operator&(ConsoleModifiers lhs, ConsoleModifiers rhs) noexcept
{
    using T = std::underlying_type_t<ConsoleModifiers>;
    
    return static_cast<ConsoleModifiers>(
        static_cast<T>(lhs) & static_cast<T>(rhs)
    );
}

constexpr ConsoleModifiers& operator|=(ConsoleModifiers& lhs, ConsoleModifiers rhs) noexcept
{
    lhs = lhs | rhs;
    return lhs;
}

constexpr ConsoleModifiers& operator&=(ConsoleModifiers& lhs, ConsoleModifiers rhs) noexcept
{
    lhs = lhs & rhs;
    return lhs;
}

CS2CPP_NAMESPACE_END