#include <gtest/gtest.h>
#include <System/Span.h>
#include <vector>
#include <array>

using namespace CS2CPP_NAMESPACE_NAME;

TEST(Span, Length)
{
    std::vector<int> c1{1,3,4,5};
    std::array<int, 4> c2{1,3,4,5};
    std::string c4 = "234mk";
    EXPECT_EQ(Span(c1).Length(), 4);
    EXPECT_EQ(Span(c2).Length(), 4);
    EXPECT_EQ(Span(c4).Length(), 5);
}

TEST(Span, IsEmpty)
{
    std::vector<int> c1{1,3,4,5};
    std::vector<int> c2;
    EXPECT_FALSE(Span(c1).IsEmpty());
    EXPECT_TRUE(Span(c2).IsEmpty());
}

TEST(Span, Slice)
{
    std::vector<int> c1{1,3,4,5};
    auto s = Span(c1).Slice(0);
    EXPECT_EQ(s.Length(), 4);
    EXPECT_EQ(s[0], 1);
    s = Span(c1).Slice(2);
    EXPECT_EQ(s.Length(), 2);
    EXPECT_EQ(s[0], 4);
    s = Span(c1).Slice(1, 3);
    EXPECT_EQ(s.Length(), 3);
    EXPECT_EQ(s[0], 3);
    int sum = 0;
    for (auto item : s) { sum += item; }
    EXPECT_EQ(sum, 12);
}

TEST(Span, Clear)
{
    std::vector<int> c1{1,3,4,5};
    Span(c1).Clear();
    EXPECT_EQ(c1[0], 0);
    EXPECT_EQ(c1[1], 0);
    EXPECT_EQ(c1[2], 0);
    EXPECT_EQ(c1[3], 0);
}

TEST(Span, Fill)
{
    std::vector<int> c1{1,3,4,5};
    Span(c1).Fill(100);
    EXPECT_EQ(c1[0], 100);
    EXPECT_EQ(c1[1], 100);
    EXPECT_EQ(c1[2], 100);
    EXPECT_EQ(c1[3], 100);
}

TEST(Span, CopyTo)
{
    std::vector<int> c1{1,3,4,5};
    std::vector<int> c2;
    EXPECT_FALSE(Span(c1).CopyTo(c2));
    c2.resize(4);
    EXPECT_TRUE(Span(c1).CopyTo(c2));
    EXPECT_EQ(c2[0], 1);
    EXPECT_EQ(c2[1], 3);
    EXPECT_EQ(c2[2], 4);
    EXPECT_EQ(c2[3], 5);
}

TEST(Span, ToArray)
{
    std::vector<int> c1{1,3,4,5};
    auto c2 = Span(c1).ToArray();
    EXPECT_EQ(c1.size(), c2.size());
    EXPECT_EQ(c1[0], c2[0]);
    EXPECT_EQ(c1[1], c2[1]);
    EXPECT_EQ(c1[2], c2[2]);
    EXPECT_EQ(c1[3], c2[3]);
}