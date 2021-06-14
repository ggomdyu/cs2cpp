#pragma once

#include <mutex>

CS2CPP_NAMESPACE_BEGIN

class Debug
{
public:
    Debug() = delete;

public:
    static void Assert(bool condition);
    static void Assert(bool condition, std::u16string_view message);
    static void Assert(bool condition, std::u16string_view message, std::u16string_view detailMessage);
    [[noreturn]] static void Fail(std::u16string_view message);
    [[noreturn]] static void Fail(std::u16string_view message, std::u16string_view detailMessage);
    static void Write(std::u16string_view message);
    static void WriteLine(std::u16string_view message);
    static void SetIndentLevel(int32_t indentLevel) noexcept;
    [[nodiscard]] static int32_t GetIndentLevel() noexcept;
    static void Indent();
    static void Unindent();

private:
    static void WriteIndent();

private:
    inline static int32_t _indentLevel = 0;
    inline static std::recursive_mutex _mutex;
};

CS2CPP_NAMESPACE_END
