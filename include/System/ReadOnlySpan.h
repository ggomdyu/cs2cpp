#pragma once

#include "System/Span.h"

CS2CPP_NAMESPACE_BEGIN

template <typename T>
class ReadOnlySpan final
{
public:
    constexpr ReadOnlySpan() noexcept = default;
    constexpr ReadOnlySpan(const T* ptr, int32_t length) noexcept;
    template <int32_t N>
    constexpr ReadOnlySpan(const T (&arr)[N]) noexcept;
    template <typename C, decltype(std::declval<C>().size(), std::declval<C>().data())* = nullptr>
    constexpr ReadOnlySpan(const C& container) noexcept;

public:
    constexpr explicit operator const T*() const noexcept;
    constexpr const T& operator[](int32_t index) const noexcept;
    constexpr bool operator==(ReadOnlySpan rhs) const noexcept;
    constexpr bool operator!=(ReadOnlySpan rhs) const noexcept;

public:
    constexpr int32_t Length() const noexcept;
    constexpr bool IsEmpty() const noexcept;
    constexpr ReadOnlySpan Slice(int32_t start) const noexcept;
    constexpr ReadOnlySpan Slice(int32_t start, int32_t length) const noexcept;
    bool CopyTo(Span<T> destination) const;
    std::vector<T> ToArray() const;

private:
    const T* storage_ = nullptr;
    int32_t length_ = 0;
};

template <typename T>
ReadOnlySpan(const T* ptr, int32_t length) -> ReadOnlySpan<T>;

template <typename T, int32_t N>
ReadOnlySpan(const T (&arr)[N]) -> ReadOnlySpan<T>;

template <typename C, typename E = std::remove_pointer_t<decltype(std::declval<C>().data())>>
ReadOnlySpan(const C& container) -> ReadOnlySpan<E>;

template <typename T>
constexpr ReadOnlySpan<T>::ReadOnlySpan(const T* ptr, int32_t length) noexcept :
    storage_(ptr),
    length_(length)
{
}

template <typename T>
template <int32_t N>
constexpr ReadOnlySpan<T>::ReadOnlySpan(const T (&arr)[N]) noexcept :
    ReadOnlySpan(arr, N)
{
}

template <typename T>
template <typename C, decltype(std::declval<C>().size(), std::declval<C>().data())*>
constexpr ReadOnlySpan<T>::ReadOnlySpan(const C& container) noexcept :
    ReadOnlySpan(container.data(), static_cast<int32_t>(container.size()))
{
}

template <typename T>
constexpr ReadOnlySpan<T>::operator const T*() const noexcept
{
    return storage_;
}

template <typename T>
constexpr int32_t ReadOnlySpan<T>::Length() const noexcept
{
    return length_;
}

template <typename T>
constexpr bool ReadOnlySpan<T>::IsEmpty() const noexcept
{
    return 0 >= length_;
}

template <typename T>
constexpr ReadOnlySpan<T> ReadOnlySpan<T>::Slice(int32_t start) const noexcept
{
    assert(0 <= start && start <= length_);
    return ReadOnlySpan<T>(storage_ + start, length_ - start);
}

template <typename T>
constexpr ReadOnlySpan<T> ReadOnlySpan<T>::Slice(int32_t start, int32_t length) const noexcept
{
    assert(0 <= start && start <= length);
    assert(length_ - start >= length);
    return ReadOnlySpan<T>(storage_ + start, length);
}

template <typename T>
constexpr const T& ReadOnlySpan<T>::operator[](int32_t index) const noexcept
{
    return storage_[index];
}

template <typename T>
constexpr bool ReadOnlySpan<T>::operator==(ReadOnlySpan<T> rhs) const noexcept
{
    return length_ == rhs.length_ && storage_ == rhs.storage_;
}

template <typename T>
constexpr bool ReadOnlySpan<T>::operator!=(ReadOnlySpan<T> rhs) const noexcept
{
    return !operator==(rhs);
}

template <typename T>
bool ReadOnlySpan<T>::CopyTo(Span<T> destination) const
{
    if (length_ > destination.length_)
    {
        return false;
    }

    std::copy(storage_, storage_ + length_, destination.storage_);
    return true;
}

template <typename T>
std::vector<T> ReadOnlySpan<T>::ToArray() const
{
    return std::vector<T>(storage_, storage_ + length_);
}

template <typename T>
constexpr const T* begin(ReadOnlySpan<T> span)
{
    return static_cast<const T*>(span);
}

template <typename T>
constexpr const T* end(ReadOnlySpan<T> span)
{
    return static_cast<const T*>(span) + span.Length();
}

CS2CPP_NAMESPACE_END
