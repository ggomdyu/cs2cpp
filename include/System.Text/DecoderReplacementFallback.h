#pragma once

#include <string>
#include <vector>

#include "System.Text/DecoderFallback.h"

CS2CPP_NAMESPACE_BEGIN

class DecoderReplacementFallback :
    public DecoderFallback
{
public:
    explicit DecoderReplacementFallback(std::u16string replacement);

public:
    void Fallback(ReadOnlySpan<std::byte> bytesUnknown) override;
    ReadOnlySpan<char16_t> GetChars() const noexcept override;

public:
    std::u16string replacement_;
    std::vector<std::byte> fallbackBuffer_;
};

CS2CPP_NAMESPACE_END