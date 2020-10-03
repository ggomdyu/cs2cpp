#include <gtest/gtest.h>
#include <System/Random.h>

using namespace CS2CPP_NAMESPACE_NAME;

TEST(Random, Next)
{
    Random r;

    // Zero value test
    for (size_t i = 0; i < 10; ++i)
        EXPECT_EQ(r.Next(0), 0);
    for (size_t i = 0; i < 10; ++i)
        EXPECT_EQ(r.Next(1), 0);

    // Same seed test
    Random r2(345);
    Random r3(345);
    for (size_t i = 0; i < 100; ++i)
        EXPECT_EQ(r2.Next(256), r3.Next(256));

    // Sum test
    int64_t tempSum = 0;
    for (size_t i = 0; i < 100; ++i)
        tempSum += r.Next();
    EXPECT_NE(tempSum, 0);

    // Min/Max value test
    for (size_t i = 0; i < 100; ++i)
    {
        EXPECT_NE(r.Next(1, 2), 0);
        EXPECT_EQ(r.Next(1, 2), 1);
        EXPECT_NE(r.Next(1, 2), 2);
    }
}

TEST(Random, NextBytes)
{
    Random r;
    std::array<int32_t, 5> sumArray{};

    for (size_t i = 0; i < 1000; ++i)
    {
        std::array<std::byte, sumArray.size()> bytes{};
        r.NextBytes(bytes);
        for (size_t j = 0; j < sumArray.size(); ++j)
            sumArray[j] += static_cast<int32_t>(bytes[j]);
    }

    for (auto sum : sumArray)
        EXPECT_NE(0, sum);
}

TEST(Random, NextDouble)
{
    Random r;
    for (size_t i = 0; i < 1000; ++i)
    {
        EXPECT_LE(0.0, r.NextDouble());
        EXPECT_LE(r.NextDouble(), 1.0);
    }

    for (size_t i = 0; i < 1000; ++i)
    {
        EXPECT_LE(5.0, r.NextDouble(5.0, 100.0));
        EXPECT_LE(r.NextDouble(5.0, 100.0), 100.0);
    }

    EXPECT_DOUBLE_EQ(r.NextDouble(0, 0), 0.0);
    EXPECT_DOUBLE_EQ(r.NextDouble(1, 1), 1.0);
}