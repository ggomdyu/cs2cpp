#include <algorithm>
#include <chrono>
#include <ctime>
#include <memory>

#include "System/Random.h"

#define R 32
#define M1 3
#define M2 24
#define M3 10

#define MAT0POS(t, v) (v ^ (v >> t))
#define MAT0NEG(t, v) (v ^ (v << (-(t))))
#define Identity(v) (v)
#define V0 _state[_stateIdx]
#define VM1 _state[(_stateIdx + M1) & 0x0000001fU]
#define VM2 _state[(_stateIdx + M2) & 0x0000001fU]
#define VM3 _state[(_stateIdx + M3) & 0x0000001fU]
#define VRm1 _state[(_stateIdx + 31) & 0x0000001fU]
#define newV0 _state[(_stateIdx + 31) & 0x0000001fU]
#define newV1 _state[_stateIdx]

#define FACT 2.32830643653869628906e-10

CS2CPP_NAMESPACE_BEGIN

Random::Random() noexcept :
    Random(static_cast<int32_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count()))
{
}

Random::Random(int32_t seed) noexcept :
    _state([&]()
    {
        srand(seed);

        std::array<unsigned, R> state{};
        std::generate(state.begin(), state.end(), std::rand);
        return state;
    } ())
{
}

int32_t Random::Next() noexcept
{
    return Next(0, std::numeric_limits<int32_t>::max());
}

int32_t Random::Next(int32_t maxValue) noexcept
{
    return Next(0, maxValue);
}

int32_t Random::Next(int32_t minValue, int32_t maxValue) noexcept
{
    return minValue + int32_t(NextDouble() * (double(maxValue) - minValue));
}

void Random::NextBytes(std::byte* bytes, int32_t count) noexcept
{
    for (auto i = 0; i < count; ++i)
        bytes[i] = std::byte(Next(0, 255));
}

void Random::NextBytes(gsl::span<std::byte> bytes) noexcept
{
    return NextBytes(bytes.data(), bytes.size());
}

double Random::NextDouble() noexcept
{
    return Sample();
}

double Random::NextDouble(double minValue, double maxValue) noexcept
{
    return minValue + (NextDouble() * (maxValue - minValue));
}

double Random::Sample() noexcept
{
    _z0 = VRm1;
    _z1 = Identity(V0) ^ MAT0POS(8, VM1);
    _z2 = MAT0NEG(-19, VM2) ^ MAT0NEG(-14, VM3);
    newV1 = _z1 ^ _z2;
    newV0 = MAT0NEG(-11, _z0) ^ MAT0NEG(-7, _z1) ^ MAT0NEG(-13, _z2);
    _stateIdx = (_stateIdx + R - 1) & 0x0000001fU;

    return double(_state[_stateIdx]) * FACT;
}

CS2CPP_NAMESPACE_END

#undef R
#undef M1
#undef M2
#undef M3
#undef MAT0POS
#undef MAT0NEG
#undef Identity
#undef V0
#undef VM1
#undef VM2
#undef VM3
#undef VRm1
#undef newV0
#undef newV1
#undef FACT
