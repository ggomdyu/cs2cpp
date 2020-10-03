#pragma once

#include <string>
#include <vector>

#include "System.Text/EncoderFallback.h"

CS2CPP_NAMESPACE_BEGIN

class EncoderReplacementFallback :
    public EncoderFallback
{
public:
    explicit EncoderReplacementFallback(std::u16string replacement);

public:
    void Fallback(ReadOnlySpan<char16_t> charsUnknown) override;
    ReadOnlySpan<char16_t> GetChars() const noexcept override;

public:
    std::u16string replacement_;
};

CS2CPP_NAMESPACE_END