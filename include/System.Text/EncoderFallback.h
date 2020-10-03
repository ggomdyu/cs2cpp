#pragma once

#include <memory>

#include "System/ReadOnlySpan.h"

CS2CPP_NAMESPACE_BEGIN

class EncoderFallback
{
public:
    virtual ~EncoderFallback() = default;

public:
    virtual void Fallback(ReadOnlySpan<char16_t> charsUnknown) = 0;
    virtual ReadOnlySpan<char16_t> GetChars() const noexcept;

public:
    static std::shared_ptr<EncoderFallback> ReplacementFallback;
    static std::shared_ptr<EncoderFallback> ExceptionFallback;
};

CS2CPP_NAMESPACE_END