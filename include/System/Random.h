#pragma once

#include <cstddef>
#include <cstdint>
#include <gsl/span>

CS2CPP_NAMESPACE_BEGIN

class RandomSampler
{
public:
    virtual ~RandomSampler() noexcept = default;

public:
    [[nodiscard]] virtual double Sample() noexcept = 0;
};
    
class Random
{
public:
    Random() noexcept;
    explicit Random(int32_t seed) noexcept;
    template <typename T, std::enable_if_t<std::is_convertible_v<T*, RandomSampler*>>* = nullptr>
    explicit Random(std::shared_ptr<T> sampler) noexcept(std::is_nothrow_constructible_v<T>);

public:
    [[nodiscard]] int32_t Next() noexcept;
    [[nodiscard]] int32_t Next(int32_t maxValue) noexcept;
    [[nodiscard]] int32_t Next(int32_t minValue, int32_t maxValue) noexcept;
    void NextBytes(std::byte* bytes, int32_t count) noexcept;
    void NextBytes(gsl::span<std::byte> bytes) noexcept;
    [[nodiscard]] double NextDouble() noexcept;
    [[nodiscard]] double NextDouble(double minValue, double maxValue) noexcept;

private:
    std::shared_ptr<RandomSampler> _sampler;
};

template <typename T, std::enable_if_t<std::is_convertible_v<T*, RandomSampler*>>*>
Random::Random(std::shared_ptr<T> sampler) noexcept(std::is_nothrow_constructible_v<T>) :
    _sampler(std::move(sampler))
{
}

CS2CPP_NAMESPACE_END
