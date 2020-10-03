#pragma once

#include "System.Text/EncoderFallback.h"
#include "System/Span.h"

CS2CPP_NAMESPACE_BEGIN

struct EncoderExceptionFallback :
    public EncoderFallback
{
public:
    void Fallback(ReadOnlySpan<char16_t> charsUnknown) override;

private:
    void Throw(ReadOnlySpan<char16_t> charsUnknown);
};

CS2CPP_NAMESPACE_END