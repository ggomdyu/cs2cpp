#pragma once

#include <type_traits>

CS2CPP_NAMESPACE_BEGIN

template <typename>
struct FunctionTraits;

template <typename R, typename... Ts>
struct FunctionTraits<R(Ts...)>
{
public:
    using Return = R;
    using Function = R(Ts...);

public:
    static constexpr bool IsMemberFunction = false;
    static constexpr bool IsFunctor = false;
    static constexpr size_t ArgumentCount = sizeof...(Ts);
};

template <typename R, typename... Ts>
struct FunctionTraits<R (*)(Ts...)> :
    FunctionTraits<R(Ts...)>
{
};

template <typename R, typename C, typename... Ts>
struct FunctionTraits<R (C::*)(Ts...)> :
    FunctionTraits<std::remove_cv_t<R(Ts...)>>
{
public:
    using Class = C;

public:
    static constexpr bool IsMemberFunction = true;
};

template <typename R, typename C, typename... Ts>
struct FunctionTraits<R (C::*)(Ts...) const> :
    FunctionTraits<R (C::*)(Ts...)>
{
};

template <typename R, typename C, typename... Ts>
struct FunctionTraits<R (C::*)(Ts...) volatile> :
    FunctionTraits<R (C::*)(Ts...)>
{
};

template <typename R, typename C, typename... Ts>
struct FunctionTraits<R (C::*)(Ts...) const volatile> :
    FunctionTraits<R (C::*)(Ts...)>
{
};

template <typename F>
struct FunctionTraits :
    FunctionTraits<decltype(&F::operator())>
{
public:
    static constexpr bool IsFunctor = true;
    static constexpr bool IsMemberFunction = false;
};

CS2CPP_NAMESPACE_END