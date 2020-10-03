#pragma once

#include <array>
#include <type_traits>

#include "FunctionTraits.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail
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
    virtual size_t GetSize() const noexcept = 0;
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
    size_t GetSize() const noexcept override;

private:
    Storage storage_;
};

template <typename F, typename R, typename... Ts>
FunctorImpl<F, R, Ts...>::FunctorImpl(Storage storage) noexcept :
    storage_(std::move(storage))
{
}

template <typename F, typename R, typename... Ts>
void FunctorImpl<F, R, Ts...>::CopyTo(Functor<R, Ts...>* functor) const
{
    new (functor) FunctorImpl<F, R, Ts...>(storage_);
}

template <typename F, typename R, typename... Ts>
R FunctorImpl<F, R, Ts...>::Invoke(Ts... args) const
{
    if constexpr (FunctionTraits<F>::IsMemberFunction)
    {
        auto& [function, receiver] = storage_;
        return (reinterpret_cast<typename FunctionTraits<F>::ClassType*>(receiver)->*(function))(std::move(args)...);
    }
    else
    {
        return std::get<0>(storage_)(std::move(args)...);
    }
}

template <typename F, typename R, typename... Ts>
void FunctorImpl<F, R, Ts...>::Destroy()
{
    if constexpr (FunctionTraits<F>::IsFunctor)
    {
        std::get<0>(storage_).~_Function();
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

template <typename R, typename... Ts>
class Delegate<R(Ts...)> final
{
private:
    template <typename F>
    using FunctorImpl = detail::FunctorImpl<F, R, Ts...>;
    using Functor = detail::Functor<R, Ts...>;

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
    bool IsDynamicAllocated() const noexcept;
    void Copy(const Delegate& rhs);
    void Move(Delegate&& rhs) noexcept;
    void Destroy();

private:
    static constexpr size_t StorageCapacity = 7;

    Functor* functor_{};
    std::array<void*, StorageCapacity> storage_{};
};

template <typename F>
Delegate(F function) -> Delegate<typename FunctionTraits<detail::RemoveCvref<F>>::Function>;

template <typename F>
Delegate(F function, void* receiver) -> Delegate<typename FunctionTraits<detail::RemoveCvref<F>>::Function>;

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
    {
        functor_ = static_cast<Functor*>(operator new(sizeof(FunctorImpl<Function>)));
    }
    else
    {
        functor_ = reinterpret_cast<Functor*>(&storage_[0]);
    }

    new (functor_) FunctorImpl<Function>(typename FunctorImpl<Function>::Storage{std::move(function)});
}

template <typename R, typename... Ts>
template <typename F>
Delegate<R(Ts...)>::Delegate(F function, void* receiver) :
    Delegate()
{
    using Function = std::decay_t<F>;
    if constexpr (Delegate::template IsLargeFunction<Function>())
    {
        functor_ = static_cast<Functor*>(operator new(sizeof(FunctorImpl<Function>)));
    }
    else
    {
        functor_ = reinterpret_cast<Functor*>(&storage_[0]);
    }

    new (functor_) FunctorImpl<Function>(typename FunctorImpl<Function>::Storage{std::move(function), receiver});
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
    if (!functor_)
    {
        return R();
    }

    return functor_->Invoke(std::forward<Ts2>(args)...);
}

template <typename R, typename... Ts>
template <typename F>
constexpr bool Delegate<R(Ts...)>::IsLargeFunction() noexcept
{
    return sizeof(FunctorImpl<F>) > (sizeof(storage_) - sizeof(storage_[0]));
}

template <typename R, typename... Ts>
Delegate<R(Ts...)>& Delegate<R(Ts...)>::operator=(const Delegate& rhs)
{
    if (this != &rhs)
    {
        Copy(rhs);
    }

    return *this;
}

template <typename R, typename... Ts>
Delegate<R(Ts...)>& Delegate<R(Ts...)>::operator=(Delegate&& rhs) noexcept
{
    if (this != &rhs)
    {
        Move(std::move(rhs));
    }

    return *this;
}

template <typename R, typename... Ts>
constexpr bool Delegate<R(Ts...)>::operator==(std::nullptr_t rhs) const noexcept
{
    return functor_ == rhs;
}

template <typename R, typename... Ts>
constexpr bool Delegate<R(Ts...)>::operator!=(std::nullptr_t rhs) const noexcept
{
    return functor_ != rhs;
}

template <typename R, typename... Ts>
bool Delegate<R(Ts...)>::IsDynamicAllocated() const noexcept
{
    return functor_ != reinterpret_cast<const Functor*>(&storage_[0]);
}

template <typename R, typename... Ts>
void Delegate<R(Ts...)>::Copy(const Delegate& rhs)
{
    Destroy();

    if (!rhs.functor_)
    {
        return;
    }

    functor_ = static_cast<Functor*>(rhs.IsDynamicAllocated() ? operator new(rhs.functor_->GetSize()) : &storage_[0]);

    rhs.functor_->CopyTo(functor_);
}

template <typename R, typename... Ts>
void Delegate<R(Ts...)>::Move(Delegate&& rhs) noexcept
{
    if (IsDynamicAllocated() && rhs.IsDynamicAllocated())
    {
        std::swap(functor_, rhs.functor_);
    }
    else if (rhs.IsDynamicAllocated())
    {
        functor_ = rhs.functor_;
        rhs.functor_ = nullptr;
    }
    else
    {
        if (IsDynamicAllocated())
        {
            std::swap(functor_, rhs.functor_);
        }

        storage_ = rhs.storage_;
        functor_ = reinterpret_cast<Functor*>(&storage_[0]);
    }
}

template <typename R, typename... Ts>
void Delegate<R(Ts...)>::Destroy()
{
    if (!functor_)
    {
        return;
    }

    functor_->Destroy();

    if (IsDynamicAllocated())
    {
        operator delete(functor_);
    }

    storage_ = {};
}

CS2CPP_NAMESPACE_END