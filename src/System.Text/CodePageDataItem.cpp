#include "System.Text/CodePageDataItem.h"

CS2CPP_NAMESPACE_BEGIN

CodePageDataItem::CodePageDataItem(int32_t uiFamilyCodePage, std::u16string_view webName, std::u16string_view headerName, std::u16string_view bodyName, std::u16string_view displayName, ReadOnlySpan<std::byte> preamble, uint32_t flags) noexcept :
    uiFamilyCodePage_(uiFamilyCodePage),
    webName_(webName),
    headerName_(headerName),
    bodyName_(bodyName),
    displayName_(displayName),
    preamble_(preamble),
    flags_(flags)
{
}

int32_t CodePageDataItem::UIFamilyCodePage() const noexcept
{
    return uiFamilyCodePage_;
}

std::u16string_view CodePageDataItem::WebName() const noexcept
{
    return webName_;
}

std::u16string_view CodePageDataItem::HeaderName() const noexcept
{
    return headerName_;
}

std::u16string_view CodePageDataItem::BodyName() const noexcept
{
    return bodyName_;
}

std::u16string_view CodePageDataItem::DisplayName() const noexcept
{
    return displayName_;
}

ReadOnlySpan<std::byte> CodePageDataItem::Preamble() const noexcept
{
    return preamble_;
}

uint32_t CodePageDataItem::Flags() const noexcept
{
    return flags_;
}

CS2CPP_NAMESPACE_END