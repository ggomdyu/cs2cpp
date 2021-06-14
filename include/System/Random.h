#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <gsl/span>

CS2CPP_NAMESPACE_BEGIN

class Random final
{
public:
    Random() noexcept;
    explicit Random(int32_t seed) noexcept;

public:
    [[nodiscard]] int32_t Next() noexcept;
    [[nodiscard]] int32_t Next(int32_t maxValue) noexcept;
    [[nodiscard]] int32_t Next(int32_t minValue, int32_t maxValue) noexcept;
    void NextBytes(std::byte* bytes, int32_t count) noexcept;
    void NextBytes(gsl::span<std::byte> bytes) noexcept;
    [[nodiscard]] double NextDouble() noexcept;
    [[nodiscard]] double NextDouble(double minValue, double maxValue) noexcept;

private:
    double Sample() noexcept;

private:
    std::array<unsigned, 32> _state;
    unsigned int _stateIdx = 0;
    unsigned int _z0 = 0;
    unsigned int _z1 = 0;
    unsigned int _z2 = 0;
};

CS2CPP_NAMESPACE_END
