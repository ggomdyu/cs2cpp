#pragma once

#include <fmt/xchar.h>

#include "System.Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

class TextWriter
{
public:
    TextWriter();
    virtual ~TextWriter() = default;

public:
    virtual void Write(std::u16string_view value) = 0;
    void Write(const char16_t* value);
    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    void Write(T value);
    void Write(bool value);
    virtual void WriteLine(std::u16string_view value) = 0;
    void WriteLine(const char16_t* value);
    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    void WriteLine(T value);
    void WriteLine();
    void WriteLine(bool value);
    virtual void Flush() = 0;
    virtual void Close() = 0;
    virtual std::shared_ptr<class Encoding> Encoding() const noexcept = 0;
    virtual void SetNewLine(std::u16string_view newLine);
    virtual std::u16string_view GetNewLine() const noexcept;

protected:
    virtual void InternalWrite(std::u16string_view span, bool appendNewLine) = 0;

protected:
    std::u16string_view newLine_;
};

template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int>>
void TextWriter::Write(T value)
{
    char16_t buffer[64];
    auto result = fmt::format_to_n(buffer, sizeof(buffer) - 1, u"{}", value);
    *result.out = '\0';

    Write(std::u16string_view(buffer, result.size));
}

template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int>>
void TextWriter::WriteLine(T value)
{
    char16_t buffer[64];
    auto result = fmt::format_to_n(buffer, sizeof(buffer) - 1, u"{}", value);
    *result.out = '\0';

    WriteLine(std::u16string_view(buffer, result.size));
}

CS2CPP_NAMESPACE_END