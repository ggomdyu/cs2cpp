#include "System.Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

EncodingInfo::EncodingInfo(int32_t codePage, std::u16string_view name, std::u16string_view displayName) noexcept :
    CodePage(codePage),
    Name(name),
    DisplayName(displayName)
{
}

bool EncodingInfo::operator==(const EncodingInfo& rhs) const noexcept
{
    return CodePage == rhs.CodePage;
}

bool EncodingInfo::operator!=(const EncodingInfo& rhs) const noexcept
{
    return !operator==(rhs);
}

const Encoding* EncodingInfo::GetEncoding() const noexcept
{
    return Encoding::GetEncoding(CodePage).get();
}

bool EncodingInfo::Equals(const EncodingInfo& value) const noexcept
{
    return CodePage == value.CodePage;
}

int32_t EncodingInfo::GetHashCode() const noexcept
{
    return CodePage;
}

CS2CPP_NAMESPACE_END