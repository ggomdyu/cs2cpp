#include <System/Console.h>
#include <System.IO/Path.h>
#include <gtest/gtest.h>

using namespace CS2CPP_NAMESPACE_NAME;

#if 0
TEST(Console, SetTitle)
{
    auto originalTitle = Console::GetTitle();
    auto title = Path::GetRandomFileName();
    EXPECT_TRUE(Console::SetTitle(title));
    auto title2 = Console::GetTitle();
    EXPECT_EQ(title, title2);
    EXPECT_TRUE(Console::SetTitle(originalTitle));
}

TEST(Console, SetWindowLeft)
{
    auto original = Console::GetWindowLeft();
    EXPECT_TRUE(Console::SetWindowLeft(10));
    EXPECT_EQ(Console::GetWindowLeft(), 10);
    EXPECT_TRUE(Console::SetWindowLeft(original));
    EXPECT_EQ(Console::GetWindowLeft(), original);
}

TEST(Console, SetWindowTop)
{
    auto original = Console::GetWindowTop();
    EXPECT_TRUE(Console::SetWindowTop(10));
    EXPECT_EQ(Console::GetWindowTop(), 10);
    EXPECT_TRUE(Console::SetWindowTop(original));
    EXPECT_EQ(Console::GetWindowTop(), original);
}


TEST(Console, SetWindowWidth)
{
    auto original = Console::GetWindowWidth();
    EXPECT_TRUE(Console::SetWindowWidth(10));
    EXPECT_EQ(Console::GetWindowWidth(), 10);
    EXPECT_TRUE(Console::SetWindowWidth(original));
    EXPECT_EQ(Console::GetWindowWidth(), original);
}

TEST(Console, SetWindowHeight)
{
    auto original = Console::GetWindowHeight();
    EXPECT_TRUE(Console::SetWindowHeight(10));
    EXPECT_EQ(Console::GetWindowHeight(), 10);
    EXPECT_TRUE(Console::SetWindowHeight(original));
    EXPECT_EQ(Console::GetWindowHeight(), original);
}

TEST(Console, SetWindowSize)
{
    auto originalWidth = Console::GetWindowWidth();
    auto originalHeight = Console::GetWindowHeight();
    EXPECT_TRUE(Console::SetWindowSize(10, 10));
    EXPECT_EQ(Console::GetWindowWidth(), 10);
    EXPECT_EQ(Console::GetWindowHeight(), 10);
    EXPECT_TRUE(Console::SetWindowSize(originalWidth, originalHeight));
    EXPECT_EQ(Console::GetWindowWidth(), originalWidth);
    EXPECT_EQ(Console::GetWindowHeight(), originalHeight);
}

TEST(Console, SetBackgroundColor)
{
    EXPECT_TRUE(Console::SetBackgroundColor(ConsoleColor::Red));
    EXPECT_EQ(Console::GetBackgroundColor(), ConsoleColor::Red);
    EXPECT_TRUE(Console::SetBackgroundColor(ConsoleColor::Black));
    EXPECT_EQ(Console::GetBackgroundColor(), ConsoleColor::Black);
}

TEST(Console, SetForegroundColor)
{
    EXPECT_TRUE(Console::SetForegroundColor(ConsoleColor::Blue));
    EXPECT_EQ(Console::GetForegroundColor(), ConsoleColor::Blue);
    EXPECT_TRUE(Console::SetForegroundColor(ConsoleColor::White));
    EXPECT_EQ(Console::GetForegroundColor(), ConsoleColor::White);
}

TEST(Console, SetCursorSize)
{
    auto original = Console::GetCursorSize();
    EXPECT_TRUE(Console::SetCursorSize(5));
    EXPECT_EQ(Console::GetCursorSize(), 5);
    EXPECT_TRUE(Console::SetCursorSize(original));
}

TEST(Console, SetCursorVisible)
{
    auto original = Console::GetCursorVisible();
    EXPECT_TRUE(Console::SetCursorVisible(!original));
    EXPECT_EQ(Console::GetCursorVisible(), !original);
    EXPECT_TRUE(Console::SetCursorVisible(original));
    EXPECT_EQ(Console::GetCursorVisible(), original);
}

TEST(Console, SetTreatControlCAsInput)
{
    auto original = Console::GetTreatControlCAsInput();
    EXPECT_TRUE(Console::SetTreatControlCAsInput(!original));
    EXPECT_EQ(Console::GetTreatControlCAsInput(), !original);
    EXPECT_TRUE(Console::SetTreatControlCAsInput(original));
    EXPECT_EQ(Console::GetTreatControlCAsInput(), original);
}
#endif
