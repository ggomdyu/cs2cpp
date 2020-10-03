#include <sstream>
#include <stdexcept>

#include "System.Text/EncoderExceptionFallback.h"

CS2CPP_NAMESPACE_BEGIN

void EncoderExceptionFallback::Fallback(ReadOnlySpan<char16_t> charsUnknown)
{
    if (!charsUnknown.IsEmpty())
    {
        Throw(charsUnknown);
    }
}

void EncoderExceptionFallback::Throw(ReadOnlySpan<char16_t> charsUnknown)
{
    std::stringstream ss;
    ss << "Unable to translate characters [";
    for (auto c : charsUnknown)
    {
        ss << c << ",";
    }
    ss.seekp(-1, std::ios_base::end);
    ss << "] from unicode to specified code page.";

    throw std::runtime_error(ss.str());
}

CS2CPP_NAMESPACE_END
