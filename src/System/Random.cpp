#include <algorithm>
#include <chrono>

#include "System/Random.h"

#define R 32
#define M1 3
#define M2 24
#define M3 10

#define MAT0POS(t, v) (v ^ (v >> t))
#define MAT0NEG(t, v) (v ^ (v << (-(t))))
#define Identity(v) (v)
#define V0 state_[stateIdx_]
#define VM1 state_[(stateIdx_ + M1) & 0x0000001fU]
#define VM2 state_[(stateIdx_ + M2) & 0x0000001fU]
#define VM3 state_[(stateIdx_ + M3) & 0x0000001fU]
#define VRm1 state_[(stateIdx_ + 31) & 0x0000001fU]
#define newV0 state_[(stateIdx_ + 31) & 0x0000001fU]
#define newV1 state_[stateIdx_]

#define FACT 2.32830643653869628906e-10

CS2CPP_NAMESPACE_BEGIN

Random::Random() noexcept :
    Random(GenerateSeed())
{
}

Random::Random(int32_t seed) noexcept :
    state_([&]()
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

void Random::NextBytes(Span<std::byte> bytes) noexcept
{
    for (auto& byte : bytes)
    {
        byte = static_cast<std::byte>(Next(0, 256));
    }
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
    z0_ = VRm1;
    z1_ = Identity(V0) ^ MAT0POS(8, VM1);
    z2_ = MAT0NEG(-19, VM2) ^ MAT0NEG(-14, VM3);
    newV1 = z1_ ^ z2_;
    newV0 = MAT0NEG(-11, z0_) ^ MAT0NEG(-7, z1_) ^ MAT0NEG(-13, z2_);
    stateIdx_ = (stateIdx_ + R - 1) & 0x0000001fU;

    return double(state_[stateIdx_]) * FACT;
}

int32_t Random::GenerateSeed() noexcept
{
    using std::chrono::high_resolution_clock;

    auto seed = high_resolution_clock::now().time_since_epoch().count();
    return static_cast<int32_t>(seed);
}

CS2CPP_NAMESPACE_END