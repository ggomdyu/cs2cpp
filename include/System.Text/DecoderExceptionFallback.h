#pragma once

#include "System.Text/DecoderFallback.h"

CS2CPP_NAMESPACE_BEGIN

class DecoderExceptionFallback :
    public DecoderFallback
{
public:
    void Fallback(ReadOnlySpan<std::byte> bytesUnknown) override;

private:
    static void Throw(ReadOnlySpan<std::byte> bytesUnknown);
};

CS2CPP_NAMESPACE_END