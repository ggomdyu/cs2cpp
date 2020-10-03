#pragma once

#include <type_traits>

CS2CPP_NAMESPACE_BEGIN

template <typename>
struct FunctionTraits;

template <typename _Return, typename... _Types>
struct FunctionTraits<_Return(_Types...)>
{
public:
    using Return = _Return;
    using Function = _Return(_Types...);

public:
    static constexpr bool IsMemberFunction = false;
    static constexpr bool IsFunctor = false;
    static constexpr size_t ArgumentCount = sizeof...(_Types);
};

template <typename _Return, typename... _Types>
struct FunctionTraits<_Return (*)(_Types...)> : FunctionTraits<_Return(_Types...)>
{
};

template <typename _Return, typename _Class, typename... _Types>
struct FunctionTraits<_Return (_Class::*)(_Types...)> :
    FunctionTraits<std::remove_cv_t<_Return(_Types...)>>
{
public:
    using Class = _Class;

public:
    static constexpr bool IsMemberFunction = true;
};

template <typename _Return, typename _Class, typename... _Types>
struct FunctionTraits<_Return (_Class::*)(_Types...) const> : FunctionTraits<_Return (_Class::*)(_Types...)>
{
};

template <typename _Return, typename _Class, typename... _Types>
struct FunctionTraits<_Return (_Class::*)(_Types...) volatile> : FunctionTraits<_Return (_Class::*)(_Types...)>
{
};

template <typename _Return, typename _Class, typename... _Types>
struct FunctionTraits<_Return (_Class::*)(_Types...) const volatile> : FunctionTraits<_Return (_Class::*)(_Types...)>
{
};

template <typename _Function>
struct FunctionTraits :
    FunctionTraits<decltype(&_Function::operator())>
{
public:
    static constexpr bool IsFunctor = true;
    static constexpr bool IsMemberFunction = false;
};

CS2CPP_NAMESPACE_END