#pragma once

#include <optional>
#include <string>

#include "System/Span.h"

CS2CPP_NAMESPACE_BEGIN

class TextReader
{
public:
    virtual ~TextReader() = default;

public:
    virtual int32_t Peek() = 0;
    virtual int32_t Read() = 0;
    virtual int32_t Read(Span<char16_t> bytes);
    virtual std::optional<std::u16string> ReadLine();
    virtual std::u16string ReadToEnd();
    virtual void Close() = 0;
};

CS2CPP_NAMESPACE_END
