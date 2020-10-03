#include "System.Text/EncoderFallback.h"
#include "System.Text/EncoderReplacementFallback.h"
#include "System.Text/EncoderExceptionFallback.h"

CS2CPP_NAMESPACE_BEGIN

std::shared_ptr<EncoderFallback> EncoderFallback::ReplacementFallback = std::make_shared<EncoderReplacementFallback>(u"?");
std::shared_ptr<EncoderFallback> EncoderFallback::ExceptionFallback = std::make_shared<EncoderExceptionFallback>();

ReadOnlySpan<char16_t> EncoderFallback::GetChars() const noexcept
{
    return {};
}

CS2CPP_NAMESPACE_END