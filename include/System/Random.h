#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "System/Span.h"

CS2CPP_NAMESPACE_BEGIN

class Random final
{
public:
    Random() noexcept;
    explicit Random(int32_t seed) noexcept;

public:
    int32_t Next() noexcept;
    int32_t Next(int32_t maxValue) noexcept;
    int32_t Next(int32_t minValue, int32_t maxValue) noexcept;
    void NextBytes(std::byte* bytes, int32_t count) noexcept;
    void NextBytes(Span<std::byte> bytes) noexcept;
    double NextDouble() noexcept;
    double NextDouble(double minValue, double maxValue) noexcept;

private:
    double Sample() noexcept;
    static int32_t GenerateSeed() noexcept;

private:
    std::array<unsigned, 32> state_;
    unsigned int stateIdx_ = 0;
    unsigned int z0_ = 0;
    unsigned int z1_ = 0;
    unsigned int z2_ = 0;
};

CS2CPP_NAMESPACE_END
