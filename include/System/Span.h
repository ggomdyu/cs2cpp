#pragma once

#include <cassert>
#include <vector>
#include <type_traits>

CS2CPP_NAMESPACE_BEGIN

namespace detail
{

template <typename T, typename = std::void_t<>>
constexpr bool HasSizeFunc = false;
template <typename T>
constexpr bool HasSizeFunc<T, std::void_t<decltype(std::declval<T>().size())>> = true;

template <typename T, typename = std::void_t<>>
constexpr bool HasDataFunc = false;
template <typename T>
constexpr bool HasDataFunc<T, std::void_t<decltype(std::declval<T>().data())>> = true;

template <typename T>
constexpr bool IsStdContainer = HasSizeFunc<T> && HasDataFunc<T>;

}

template <typename T>
class Span final
{
    static_assert(!std::is_const_v<T>, "Span doesn't accept const template parameter, use ReadOnlySpan instead.");

public:
    using Element = T;
    using Pointer = T*;

public:
    constexpr Span() noexcept = default;
    constexpr Span(T* ptr, int32_t length) noexcept;
    template <int32_t N>
    constexpr Span(T (&arr)[N]) noexcept;
    template <typename C, std::enable_if_t<detail::IsStdContainer<C>, int> = 0>
    constexpr Span(C& container) noexcept;
    constexpr Span(std::initializer_list<T> container) noexcept;

public:
    constexpr explicit operator T*() noexcept;
    constexpr explicit operator const T*() const noexcept;
    constexpr T& operator[](int32_t index) noexcept;
    constexpr const T& operator[](int32_t index) const noexcept;
    constexpr bool operator==(Span rhs) const noexcept;
    constexpr bool operator!=(Span rhs) const noexcept;

public:
    constexpr int32_t Length() const noexcept;
    constexpr bool IsEmpty() const noexcept;
    constexpr Span Slice(int32_t start) const noexcept;
    constexpr Span Slice(int32_t start, int32_t length) const noexcept;
    void Clear();
    void Fill(T value);
    bool CopyTo(Span<T> destination) const;
    std::vector<T> ToArray() const;

private:
    T* storage_ = nullptr;
    int32_t length_ = 0;
};

template <typename T>
Span(T* ptr, int32_t length) -> Span<T>;

template <typename T, int32_t N>
Span(T (&arr)[N]) -> Span<T>;

template <typename C, typename E = std::remove_pointer_t<decltype(std::declval<C>().data())>>
Span(C& container) -> Span<E>;

template <typename T>
constexpr Span<T>::Span(T* ptr, int32_t length) noexcept :
    storage_(ptr),
    length_(length)
{
}

template <typename T>
template <int32_t N>
constexpr Span<T>::Span(T (&arr)[N]) noexcept :
    Span(arr, N)
{
}

template <typename T>
template <typename C, std::enable_if_t<detail::IsStdContainer<C>, int>>
constexpr Span<T>::Span(C& container) noexcept :
    Span(container.data(), static_cast<int32_t>(container.size()))
{
}

template <typename T>
constexpr Span<T>::Span(std::initializer_list<T> container) noexcept :
    Span(container.begin(), container.end() - container.begin())
{
}

template <typename T>
constexpr Span<T>::operator T*() noexcept
{
    return storage_;
}

template <typename T>
constexpr Span<T>::operator const T*() const noexcept
{
    return storage_;
}

template <typename T>
constexpr int32_t Span<T>::Length() const noexcept
{
    return length_;
}

template <typename T>
constexpr bool Span<T>::IsEmpty() const noexcept
{
    return 0 >= length_;
}

template <typename T>
constexpr Span<T> Span<T>::Slice(int32_t start) const noexcept
{
    assert(0 <= start && start <= length_);
    return Span<T>(storage_ + start, length_ - start);
}

template <typename T>
constexpr Span<T> Span<T>::Slice(int32_t start, int32_t length) const noexcept
{
    assert(0 <= start && start <= length);
    assert(length_ - start >= length);
    return Span<T>(storage_ + start, length);
}

template <typename T>
constexpr T& Span<T>::operator[](int32_t index) noexcept
{
    assert(0 <= index && index < length_);
    return storage_[index];
}

template <typename T>
constexpr const T& Span<T>::operator[](int32_t index) const noexcept
{
    return const_cast<Span&>(*this)[index];
}

template <typename T>
constexpr bool Span<T>::operator==(Span<T> rhs) const noexcept
{
    return length_ == rhs.length_ && storage_ == rhs.storage_;
}

template <typename T>
constexpr bool Span<T>::operator!=(Span<T> rhs) const noexcept
{
    return !operator==(rhs);
}

template <typename T>
void Span<T>::Clear()
{
    std::fill(storage_, storage_ + length_, T());
}

template <typename T>
void Span<T>::Fill(T value)
{
    std::fill(storage_, storage_ + length_, value);
}

template <typename T>
bool Span<T>::CopyTo(Span<T> destination) const
{
    if (length_ > destination.length_)
    {
        return false;
    }

    std::copy(storage_, storage_ + length_, destination.storage_);
    return true;
}

template <typename T>
std::vector<T> Span<T>::ToArray() const
{
    std::vector<T> ret(static_cast<size_t>(length_));
    std::copy(storage_, storage_ + length_, ret.data());

    return ret;
}

template <typename T>
constexpr T* begin(Span<T> span)
{
    return static_cast<T*>(span);
}

template <typename T>
constexpr T* end(Span<T> span)
{
    return static_cast<T*>(span) + span.Length();
}

CS2CPP_NAMESPACE_END
