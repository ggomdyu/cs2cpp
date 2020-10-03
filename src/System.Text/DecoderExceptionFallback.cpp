#include <sstream>
#include <stdexcept>

#include "System.Text/DecoderExceptionFallback.h"

CS2CPP_NAMESPACE_BEGIN

void DecoderExceptionFallback::Fallback(ReadOnlySpan<std::byte> bytesUnknown)
{
    if (!bytesUnknown.IsEmpty())
    {
        Throw(bytesUnknown);
    }
}

void DecoderExceptionFallback::Throw(ReadOnlySpan<std::byte> bytesUnknown)
{
    std::stringstream ss;
    ss << "Unable to translate bytes [";
    for (auto byte : bytesUnknown)
    {
        ss << static_cast<uint32_t>(byte) << ",";
    }
    ss.seekp(-1, std::ios_base::end);
    ss << "] from specified code page to unicode.";


    throw std::runtime_error(ss.str());
}

CS2CPP_NAMESPACE_END