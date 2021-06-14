#pragma once

#include <fmt/format.h>

#include "System/Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

class TextWriter
{
public:
    virtual ~TextWriter() = default;

public:
    virtual void Write(char16_t value) = 0;
    void Write(std::u16string_view value);
    template <typename... Ts>
    void Write(std::u16string_view format, const Ts&... args);
    void WriteLine();
    void WriteLine(char16_t value);
    void WriteLine(std::u16string_view value);
    template <typename... Ts>
    void WriteLine(std::u16string_view format, const Ts&... args);
    virtual void Flush() = 0;
    virtual void Close() = 0;
    [[nodiscard]] virtual const Encoding& GetEncoding() const noexcept = 0;
    [[nodiscard]] virtual std::u16string_view GetNewLine() const noexcept;
};

template <typename... Ts>
void TextWriter::Write(std::u16string_view format, const Ts&... args)
{
    fmt::basic_memory_buffer<char16_t> buffer;
    fmt::format_to(buffer, format, args...);

    return Write(buffer.data(), static_cast<int32_t>(buffer.size()));
}

template <typename... Ts>
void TextWriter::WriteLine(std::u16string_view format, const Ts&... args)
{
    Write(format, args...);
    WriteLine();
}

CS2CPP_NAMESPACE_END
