#pragma once

#include <gsl/span>
#include <optional>

#include "System/String.h"

CS2CPP_NAMESPACE_BEGIN

class TextReader
{
public:
    virtual ~TextReader() = default;

public:
    virtual int32_t Peek() = 0;
    virtual int32_t Read() = 0;
    virtual int32_t Read(gsl::span<char16_t> bytes);
    int32_t Read(char16_t* bytes, int32_t count);
    virtual std::optional<String> ReadLine();
    virtual String ReadToEnd();
    virtual void Close() = 0;
};

CS2CPP_NAMESPACE_END