#include "System.IO.Enumeration/DirectoryIterator.h"

CS2CPP_NAMESPACE_BEGIN

bool DirectoryIterator::operator==(const DirectoryIterator& rhs) noexcept
{
    return impl_ == rhs.impl_;
}

bool DirectoryIterator::operator!=(const DirectoryIterator& rhs) noexcept
{
    return !operator==(rhs);
}

std::u16string_view DirectoryIterator::operator->() noexcept
{
    return operator*();
}

DirectoryIterator& DirectoryIterator::operator++()
{
    Increment();
    return *this;
}

std::u16string DirectoryIterator::operator++(int32_t)
{
    std::u16string ret(operator*());
    Increment();
    return ret;
}

CS2CPP_NAMESPACE_END