#include <algorithm>
#include <fmt/format.h>

#include "System/Diagnostics/Debug.h"

CS2CPP_NAMESPACE_BEGIN

void Debug::Assert(bool condition)
{
    Assert(condition, {u"", 0}, {u"", 0});
}

void Debug::Assert(bool condition, std::u16string_view message)
{
    Assert(condition, message, {u"", 0});
}

void Debug::Assert(bool condition, std::u16string_view message, std::u16string_view detailMessage)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    if (condition)
        return;

    Fail(message, detailMessage);
#endif
}

void Debug::Fail(std::u16string_view message)
{
    Fail(message, {u"", 0});
}

void Debug::Fail(std::u16string_view message, std::u16string_view detailMessage)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    auto str = fmt::format(u"---- DEBUG ASSERTION FAILED ----\n---- Assert Short Message ----\n{0}\n---- Assert Long Message ----\n{1}\n", message, detailMessage);

    std::lock_guard lock(_mutex);

    // Ignore the indent when printing the failure message.
    auto prevIndentLevel = _indentLevel;
    _indentLevel = 0;
    {
        Write(str);
    }
    _indentLevel = prevIndentLevel;

    abort();
#endif
}

void Debug::SetIndentLevel(int32_t indentLevel) noexcept
{
#if defined(_DEBUG) || !defined(NDEBUG)
    std::lock_guard lock(_mutex);
    _indentLevel = std::max(indentLevel, 0);
#endif
}

int32_t Debug::GetIndentLevel() noexcept
{
    return _indentLevel;
}

void Debug::Indent()
{
#if defined(_DEBUG) || !defined(NDEBUG)
    std::lock_guard lock(_mutex);
    _indentLevel += 1;
#endif
}

void Debug::Unindent()
{
#if defined(_DEBUG) || !defined(NDEBUG)
    std::lock_guard lock(_mutex);
    _indentLevel = std::max(_indentLevel - 1, 0);
#endif
}

CS2CPP_NAMESPACE_END
