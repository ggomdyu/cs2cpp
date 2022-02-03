#pragma once

#include <memory>
#include <limits>
#include <string>

#include "System.IO.Enumeration/SearchOption.h"

CS2CPP_NAMESPACE_BEGIN

struct DirectoryIterator final
{
public:
    DirectoryIterator() = default;
    DirectoryIterator(std::u16string_view basePath, std::u16string_view searchPattern, uint32_t filter, SearchOption searchOption);

public:
    bool operator==(const DirectoryIterator& rhs) noexcept;
    bool operator!=(const DirectoryIterator& rhs) noexcept;
    std::u16string_view operator*() noexcept;
    std::u16string_view operator->() noexcept;
    DirectoryIterator& operator++();
    std::u16string operator++(int32_t);

private:
    void Increment();

private:
    std::shared_ptr<class IteratorImpl> impl_;
};

inline DirectoryIterator begin(DirectoryIterator it) noexcept
{
    return it;
}

inline DirectoryIterator end(const DirectoryIterator&) noexcept
{
    return DirectoryIterator{};
}

CS2CPP_NAMESPACE_END

namespace std
{

template<>
struct iterator_traits<CS2CPP_NAMESPACE_NAME::DirectoryIterator>
{
    using iterator_category = std::input_iterator_tag;
};

}