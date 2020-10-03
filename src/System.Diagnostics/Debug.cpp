#include <algorithm>
#include <fmt/xchar.h>

#include "System.Diagnostics/Debug.h"

CS2CPP_NAMESPACE_BEGIN

void Debug::Print(std::u16string_view message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    WriteLine(message);
#endif
}

void Debug::Assert(bool condition)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    Assert(condition, {}, {});
#endif
}

void Debug::Assert(bool condition, std::u16string_view message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    Assert(condition, message, {});
#endif
}

void Debug::Assert(bool condition, std::u16string_view message, std::u16string_view detailMessage)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    if (condition)
    {
        return;
    }

    Fail(message, detailMessage);
#endif
}

void Debug::Fail(std::u16string_view message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    Fail(message, {});
#endif
}

void Debug::Fail(std::u16string_view message, std::u16string_view detailMessage)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    auto str = fmt::format(u"---- DEBUG ASSERTION FAILED ----\n---- Assert Short Message ----\n{0}\n---- Assert Long Message ----\n{1}\n",
        message, detailMessage);

    // Ignore the indent when printing the failure message.
    auto prevIndentLevel = indentLevel_;
    indentLevel_ = 0;
    {
        Write(str);
    }
    indentLevel_ = prevIndentLevel;

    abort();
#endif
}

void Debug::Write(std::u16string_view message, std::u16string_view category)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    Write(fmt::format(u"{}: {}", category, message));
#endif
}

void Debug::WriteIf(bool condition, std::u16string_view message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    if (condition)
    {
        Write(message);
    }
#endif
}

void Debug::WriteIf(bool condition, std::u16string_view message, std::u16string_view category)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    if (condition)
    {
        Write(message, category);
    }
#endif
}

void Debug::WriteLine(std::u16string_view message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    Write(fmt::format(u"{}\n", message));
#endif
}

void Debug::WriteLine(std::u16string_view message, std::u16string_view category)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    Write(fmt::format(u"{}: {}\n", category, message));
#endif
}

void Debug::WriteLineIf(bool condition, std::u16string_view message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    if (condition)
    {
        WriteLine(message);
    }
#endif
}

void Debug::WriteLineIf(bool condition, std::u16string_view message, std::u16string_view category)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    if (condition)
    {
        WriteLine(message, category);
    }
#endif
}

void Debug::SetIndentLevel(int32_t indentLevel) noexcept
{
    indentLevel_ = std::max(indentLevel, 0);
}

int32_t Debug::GetIndentLevel() noexcept
{
    return indentLevel_;
}

void Debug::Indent()
{
#if defined(_DEBUG) || !defined(NDEBUG)
    indentLevel_ += 1;
#endif
}

void Debug::Unindent()
{
#if defined(_DEBUG) || !defined(NDEBUG)
    indentLevel_ = std::max(indentLevel_ - 1, 0);
#endif
}
CS2CPP_NAMESPACE_END
