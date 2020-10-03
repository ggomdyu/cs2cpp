#pragma once

#include <fmt/xchar.h>
#include <mutex>
#include <optional>

CS2CPP_NAMESPACE_BEGIN

class Debug final
{
public:
    Debug() = delete;

public:
    template <typename... Ts>
    static void Print(std::u16string_view format, const Ts&... args);
    static void Print(std::u16string_view message);
    static void Assert(bool condition);
    static void Assert(bool condition, std::u16string_view message);
    static void Assert(bool condition, std::u16string_view message, std::u16string_view detailMessage);
    [[noreturn]] static void Fail(std::u16string_view message);
    [[noreturn]] static void Fail(std::u16string_view message, std::u16string_view detailMessage);
    static void Write(std::u16string_view message);
    static void Write(std::u16string_view message, std::u16string_view category);
    static void WriteIf(bool condition, std::u16string_view message);
    static void WriteIf(bool condition, std::u16string_view message, std::u16string_view category);
    static void WriteLine(std::u16string_view message);
    static void WriteLine(std::u16string_view message, std::u16string_view category);
    static void WriteLineIf(bool condition, std::u16string_view message);
    static void WriteLineIf(bool condition, std::u16string_view message, std::u16string_view category);
    static void SetIndentLevel(int32_t indentLevel) noexcept;
    static int32_t GetIndentLevel() noexcept;
    static void Indent();
    static void Unindent();

private:
    static void WriteIndent();

private:
    inline static std::recursive_mutex mutex_;
    inline static thread_local int32_t indentLevel_ = 0;
};

template <typename... Ts>
void Debug::Print(std::u16string_view format, const Ts&... args)
{
    Print(fmt::format(format, args...));
}

CS2CPP_NAMESPACE_END
