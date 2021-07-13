#pragma once

#include <array>
#include <type_traits>

#include "FunctionTraits.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::delegate
{

template <typename T>
using RemoveCvref = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename R, typename... Ts>
class CS2CPP_NOVTABLE Functor
{
public:
    Functor() = default;
    Functor(const Functor& rhs) = delete;
    Functor(Functor&& rhs) = delete;
    virtual ~Functor() = default;

public:
    Functor& operator=(const Functor& rhs) = delete;
    Functor& operator=(Functor&& rhs) = delete;

public:
    virtual R Invoke(Ts... args) const = 0;
    virtual void CopyTo(Functor* functor) const = 0;
    virtual void Destroy() = 0;
    [[nodiscard]] virtual size_t GetSize() const noexcept = 0;
};

template <typename F, typename R, typename... Ts>
class FunctorImpl final :
    public Functor<R, Ts...>
{
public:
    using Storage = std::conditional_t<FunctionTraits<F>::IsMemberFunction, std::tuple<F, void*>, std::tuple<F>>;

public:
    explicit FunctorImpl(Storage storage) noexcept;

public:
    R Invoke(Ts... args) const override;
    void CopyTo(Functor<R, Ts...>* functor) const override;
    void Destroy() override;
    [[nodiscard]] size_t GetSize() const noexcept override;

private:
    Storage _storage;
};

template <typename F, typename R, typename... Ts>
FunctorImpl<F, R, Ts...>::FunctorImpl(Storage storage) noexcept :
    _storage(std::move(storage))
{
}

template <typename F, typename R, typename... Ts>
void FunctorImpl<F, R, Ts...>::CopyTo(Functor<R, Ts...>* functor) const
{
    new (functor) FunctorImpl<F, R, Ts...>(_storage);
}

template <typename F, typename R, typename... Ts>
R FunctorImpl<F, R, Ts...>::Invoke(Ts... args) const
{
    if constexpr (FunctionTraits<F>::IsMemberFunction)
    {
        auto& [function, receiver] = _storage;
        return (reinterpret_cast<typename FunctionTraits<F>::ClassType*>(receiver)->*(function))(std::move(args)...);
    }
    else
        return std::get<0>(_storage)(std::move(args)...);
}

template <typename F, typename R, typename... Ts>
void FunctorImpl<F, R, Ts...>::Destroy()
{
    if constexpr (FunctionTraits<F>::IsFunctor)
    {
        std::get<0>(_storage).~_Function();
    }
}

template <typename F, typename R, typename... Ts>
size_t FunctorImpl<F, R, Ts...>::GetSize() const noexcept
{
    return sizeof(*this);
}

}

template <typename>
class Delegate;

template <typename>
constexpr bool IsDelegate = std::false_type::value;

template <typename T>
constexpr bool IsDelegate<Delegate<T>> = std::true_type::value;

template <typename R, typename... Ts>
class Delegate<R(Ts...)> final
{
public:
    using Return = R;

private:
    template <typename F>
    using FunctorImpl = detail::delegate::FunctorImpl<F, R, Ts...>;
    using Functor = detail::delegate::Functor<R, Ts...>;

public:
    constexpr Delegate() noexcept = default;
    constexpr Delegate(std::nullptr_t) noexcept;
    Delegate(const Delegate& rhs);
    Delegate(Delegate&& rhs) noexcept;
    template <typename F>
    Delegate(F function);
    template <typename F>
    Delegate(F function, void* receiver);
    ~Delegate();

public:
    template <typename... Ts2>
    R operator()(Ts2&&... args) const;
    Delegate& operator=(const Delegate& rhs);
    Delegate& operator=(Delegate&& rhs) noexcept;
    constexpr bool operator==(std::nullptr_t rhs) const noexcept;
    constexpr bool operator!=(std::nullptr_t rhs) const noexcept;

private:
    template <typename F>
    static constexpr bool IsLargeFunction() noexcept;
    [[nodiscard]] bool IsDynamicAllocated() const noexcept;
    void Copy(const Delegate& rhs);
    void Move(Delegate&& rhs) noexcept;
    void Destroy();

private:
    static constexpr size_t StorageCapacity = 7;

    Functor* _functor{};
    std::array<void*, StorageCapacity> _storage{};
};

template <typename F>
Delegate(F function) -> Delegate<typename FunctionTraits<detail::delegate::RemoveCvref<F>>::Function>;

template <typename F>
Delegate(F function, void* receiver) -> Delegate<typename FunctionTraits<detail::delegate::RemoveCvref<F>>::Function>;

template <typename R, typename... Ts>
constexpr Delegate<R(Ts...)>::Delegate(std::nullptr_t) noexcept :
    Delegate()
{
}

template <typename R, typename... Ts>
Delegate<R(Ts...)>::Delegate(const Delegate& rhs) :
    Delegate()
{
    Copy(rhs);
}

template <typename R, typename... Ts>
Delegate<R(Ts...)>::Delegate(Delegate&& rhs) noexcept :
    Delegate()
{
    Move(std::move(rhs));
}

template <typename R, typename... Ts>
template <typename F>
Delegate<R(Ts...)>::Delegate(F function) :
    Delegate()
{
    using Function = std::decay_t<F>;
    if constexpr (Delegate::template IsLargeFunction<Function>())
        _functor = static_cast<Functor*>(operator new(sizeof(FunctorImpl<Function>)));
    else
        _functor = reinterpret_cast<Functor*>(&_storage[0]);

    new (_functor) FunctorImpl<Function>(typename FunctorImpl<Function>::Storage{std::move(function)});
}

template <typename R, typename... Ts>
template <typename F>
Delegate<R(Ts...)>::Delegate(F function, void* receiver) :
    Delegate()
{
    using Function = std::decay_t<F>;
    if constexpr (Delegate::template IsLargeFunction<Function>())
        _functor = static_cast<Functor*>(operator new(sizeof(FunctorImpl<Function>)));
    else
        _functor = reinterpret_cast<Functor*>(&_storage[0]);

    new (_functor) FunctorImpl<Function>(typename FunctorImpl<Function>::Storage{std::move(function), receiver});
}

template <typename R, typename... Ts>
Delegate<R(Ts...)>::~Delegate()
{
    Destroy();
}

template <typename R, typename... Ts>
template <typename... Ts2>
R Delegate<R(Ts...)>::operator()(Ts2&&... args) const
{
    if (_functor == nullptr)
        return R();

    return _functor->Invoke(std::forward<Ts2>(args)...);
}

template <typename R, typename... Ts>
template <typename F>
constexpr bool Delegate<R(Ts...)>::IsLargeFunction() noexcept
{
    return sizeof(FunctorImpl<F>) > (sizeof(_storage) - sizeof(_storage[0]));
}

template <typename R, typename... Ts>
Delegate<R(Ts...)>& Delegate<R(Ts...)>::operator=(const Delegate& rhs)
{
    if (this != &rhs)
        Copy(rhs);

    return *this;
}

template <typename R, typename... Ts>
Delegate<R(Ts...)>& Delegate<R(Ts...)>::operator=(Delegate&& rhs) noexcept
{
    if (this != &rhs)
        Move(std::move(rhs));

    return *this;
}

template <typename R, typename... Ts>
constexpr bool Delegate<R(Ts...)>::operator==(std::nullptr_t rhs) const noexcept
{
    return _functor == rhs;
}

template <typename R, typename... Ts>
constexpr bool Delegate<R(Ts...)>::operator!=(std::nullptr_t rhs) const noexcept
{
    return _functor != rhs;
}

template <typename R, typename... Ts>
bool Delegate<R(Ts...)>::IsDynamicAllocated() const noexcept
{
    return _functor != reinterpret_cast<const Functor*>(&_storage[0]);
}

template <typename R, typename... Ts>
void Delegate<R(Ts...)>::Copy(const Delegate& rhs)
{
    Destroy();

    if (rhs._functor == nullptr)
        return;

    _functor = static_cast<Functor*>(rhs.IsDynamicAllocated() ? operator new(rhs._functor->GetSize()) : &_storage[0]);

    rhs._functor->CopyTo(_functor);
}

template <typename R, typename... Ts>
void Delegate<R(Ts...)>::Move(Delegate&& rhs) noexcept
{
    if (IsDynamicAllocated() && rhs.IsDynamicAllocated())
        std::swap(_functor, rhs._functor);
    else if (rhs.IsDynamicAllocated())
    {
        _functor = rhs._functor;
        rhs._functor = nullptr;
    }
    else
    {
        if (IsDynamicAllocated())
            std::swap(_functor, rhs._functor);

        _storage = rhs._storage;
        _functor = reinterpret_cast<Functor*>(&_storage[0]);
    }
}

template <typename R, typename... Ts>
void Delegate<R(Ts...)>::Destroy()
{
    if (_functor == nullptr)
        return;

    _functor->Destroy();

    if (IsDynamicAllocated())
        operator delete(_functor);

    _storage = {};
}

CS2CPP_NAMESPACE_END

#undef CS2CPP_NOVTABLE
