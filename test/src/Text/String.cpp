#include <gtest/gtest.h>
#include <System/String.h>

using tg::String;
using tg::StringSplitOptions;
using tg::NormalizationForm;

TEST(String, Compare)
{
    EXPECT_EQ(String::Compare(u"Hello", u"HELLop", true), -1);
    EXPECT_EQ(String::Compare(u"Hello", u"Hello", true), 0);
    EXPECT_EQ(String::Compare(u"Hello", u"HellO", true), 0);
    EXPECT_EQ(String::Compare(u"Hello", u"HELLp", true), -1);
    EXPECT_EQ(String::Compare(u"Helloppp", u"Hellop", true), 1);
    EXPECT_EQ(String::Compare(u"aa", u"ab", true), -1);
    EXPECT_EQ(String::Compare(u"ab", u"ac", true), -1);
    EXPECT_EQ(String::Compare(u"가나", u"가가", true), 1);
    EXPECT_EQ(String::Compare(u"가가", u"가나", true), -1);
    EXPECT_EQ(String::Compare(u"He", u"HE", false), -1);
    EXPECT_EQ(String::Compare(u"e", u"E", false), -1);
    EXPECT_EQ(String::Compare(u"e", u"D", false), 1);
    EXPECT_EQ(String::Compare(u"He", u"HE", true), 0);
    EXPECT_EQ(String::Compare(u"e", u"E", true), 0);
    EXPECT_EQ(String::Compare(u"e", u"D", true), 1);
    EXPECT_EQ(String::Compare(u"Hello", u"Hello", true), 0);
    EXPECT_EQ(String::Compare(u"Hello", u"HellO", true), 0);
    EXPECT_EQ(String::Compare(u"Hello", u"HELLp", true), -1);
    EXPECT_EQ(String::Compare(u"Helloppp", u"Hellop", true), 1);
    EXPECT_EQ(String::Compare(u"Hello", u"Hello", false), 0);
    EXPECT_EQ(String::Compare(u"Hello", u"HellO", false), -1);
    EXPECT_EQ(String::Compare(u"Hello", u"HELLp", false), -1);
    EXPECT_EQ(String::Compare(u"Helloppp", u"Hellop", false), 1);
}

TEST(String, Concat)
{
    EXPECT_EQ(String::Concat(u"SFえ", u"ＦＷあ아", u'월', String(u"dsdf")), u"SFえＦＷあ아월dsdf");
    EXPECT_EQ(String::Concat(u"SFえ"), u"SFえ");
    EXPECT_NE(String::Concat(u"SFえ"), u"SFえa");
}

TEST(String, Format)
{
    EXPECT_EQ(String::Format(u"{}{}{}", 1,2,3), u"123");
    EXPECT_EQ(String::Format(u"{}", 1), u"1");
}

TEST(String, IsNullOrEmpty)
{
    EXPECT_TRUE(String::IsNullOrEmpty(u""));
    EXPECT_FALSE(String::IsNullOrEmpty(u"a"));
    EXPECT_FALSE(String::IsNullOrEmpty(u"\u2000"));
    EXPECT_FALSE(String::IsNullOrEmpty(u" "));
}

TEST(String, IsNullOrWhiteSpace)
{
    EXPECT_TRUE(String::IsNullOrWhiteSpace(u""));
    EXPECT_FALSE(String::IsNullOrWhiteSpace(u"ABCDE"));
    EXPECT_FALSE(String::IsNullOrWhiteSpace(u"ABCDE "));
    EXPECT_FALSE(String::IsNullOrWhiteSpace(u" ABCDE"));
    EXPECT_FALSE(String::IsNullOrWhiteSpace(u" ABCDE "));
    EXPECT_TRUE(String::IsNullOrWhiteSpace(String(u' ', 20)));
    EXPECT_TRUE(String::IsNullOrWhiteSpace(u"  \t   "));
    EXPECT_TRUE(String::IsNullOrWhiteSpace(String(u'\u2000', 10)));
}

TEST(String, CompareOrdinal)
{
    EXPECT_EQ(String::CompareOrdinal(u"a", u"ac"), -99);
    EXPECT_EQ(String::CompareOrdinal(u"a", u"bc"), -1);
    EXPECT_EQ(String::CompareOrdinal(u"a34rt34t346245v3", u"a44rt34t346245v3"), -1);
    EXPECT_EQ(String::CompareOrdinal(u"a34rt34t346245v3", u"a34rt34t346245v378j"), -3);
}

TEST(String, Join)
{
    EXPECT_EQ(String::Join(u",", {u"hi", u"min", u"Joe"}), u"hi,min,Joe");
    EXPECT_EQ(String::Join(u"", {u"hi", u"min", u"Joe"}), u"himinJoe");
    EXPECT_EQ(String::Join(u"ppap", {u"hi", u"min", u"Joe"}), u"hippapminppapJoe");
    EXPECT_EQ(String::Join(u",", {u"hi"}), u"hi");
    EXPECT_EQ(String::Join(u",", {}), u"");
    EXPECT_EQ(String::Join(u"", {}), u"");
}

TEST(String, OperatorEqual)
{
    EXPECT_EQ(String(U'a', 10), u"aaaaaaaaaa");
    EXPECT_EQ(String(U'😋', 10), u"😋😋😋😋😋😋😋😋😋😋");
    EXPECT_EQ(std::u16string_view(String(U'a', 10)), std::u16string_view(u"aaaaaaaaaa"));
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ"), u"QqQRr楽yYYㅎあ");
    EXPECT_EQ(String(u"  QqQRr   楽yYYㅎあ  "), u"  QqQRr   楽yYYㅎあ  ");
    EXPECT_EQ(String(), u"");
}

TEST(String, OperatorNotEqual)
{
    EXPECT_NE(String(U'a', 10), u"aaaaaqaaaa");
    EXPECT_NE(String(U'😋', 10), u"😋😋😋😋😋😋😋😋😋");
    EXPECT_NE(String(U'😋', 10), u"😋😋😋😋😋a😋😋😋😋😋");
}

TEST(String, OperatorAssignment)
{
    EXPECT_EQ(String(U'a', 10) = u"asfdq", u"asfdq");
    EXPECT_EQ((String(U'a', 10) = u"asfdq").Length(), 5);
}

TEST(String, OperatorAdditionAssignment)
{
    EXPECT_EQ(String(u"absabs") += u"erwcw", u"absabserwcw");
    EXPECT_EQ(String(u"wwwerwkehr") += u"adsf", u"wwwerwkehradsf");
    EXPECT_EQ(String(u"asfdq") += u"秋안原あ", u"asfdq秋안原あ");
    EXPECT_EQ((String(u"asfdq") += String(u"秋안原あ")).Length(), 9);
    EXPECT_EQ(String(u"asfdq秋안原あ") += u"큐", u"asfdq秋안原あ큐");
    EXPECT_EQ((String(u"asfdq秋안原あ") += String(u"큐")).Length(), 10);
    EXPECT_EQ((String(u"asfdq秋안原あ") += String(u"큐"))[3], u'd');
}

TEST(String, OperatorSubscript)
{
    EXPECT_EQ(String(u"a안녕😋cc")[0], u'a');
    EXPECT_EQ(String(u"a안녕😋cc")[1], u'안');
    EXPECT_EQ(String(u"a안녕😋cc")[2], u'녕');
    EXPECT_EQ(String(u"a안녕😋cc")[5], u'c');
    EXPECT_EQ(String(u"a안녕😋cc")[6], u'c');
}

TEST(String, Insert)
{
    EXPECT_EQ(String(U'a', 10).Insert(0, u"あやや"), u"あややaaaaaaaaaa");
    EXPECT_EQ(String(U'a', 10).Insert(9, u"あやや"), u"aaaaaaaaaあややa");
    EXPECT_EQ(String(U'a', 10).Insert(10, u"あやや"), u"aaaaaaaaaaあやや");
    EXPECT_EQ(String(u"sdfwfoij").Insert(0, u"あやや"), u"あややsdfwfoij");
}

TEST(String, Split)
{
    EXPECT_EQ(String(U'a', 10).Split({u'a'}).size(), 11);
    EXPECT_EQ(String(U'a', 10).Split({u'a'})[0], u"");
    EXPECT_EQ(String(U'a', 10).Split({u'a'})[10], u"");
    EXPECT_EQ(String(U'a', 10).Split({u'a'}, StringSplitOptions::RemoveEmptyEntries).size(), 0);
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Split({u'Y'}).size(), 3);
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Split({u'Y'})[0], u"QqQRr楽y");
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Split({u'Y'})[1], u"");
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Split({u'Y'})[2], u"ㅎあ");
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Split({u'Y'}, StringSplitOptions::RemoveEmptyEntries).size(), 2);
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Split({u'Y'}, StringSplitOptions::RemoveEmptyEntries)[0], u"QqQRr楽y");
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Split({u'Y'}, StringSplitOptions::RemoveEmptyEntries)[1], u"ㅎあ");
    EXPECT_EQ(String(u"thel9lo a\tt").Split({u'o'}).size(), 2);
    EXPECT_EQ(String(u"thel9lo a\tt").Split({u'o'})[0], u"thel9l");
    EXPECT_EQ(String(u"thel9lo a\tt").Split({u'o'})[1], u" a\tt");
    EXPECT_EQ(String(u"thel9lo a\tt").Split({u'Q'}).size(), 1);
    EXPECT_EQ(String(u"thel9lo a\tt").Split({u'Q'})[0], u"thel9lo a\tt");
}

TEST(String, Length)
{
    EXPECT_EQ(String(U'a', 10).Length(), 10);
    EXPECT_EQ(std::u16string_view(String(U'a', 10)).length(), 10);
    EXPECT_EQ(String(U'😋', 10).Length(), 20);
    EXPECT_EQ(String(u"a안녕😋cc").Length(), 7);
    EXPECT_EQ(String().Length(), 0);
}

TEST(String, Equals)
{
    EXPECT_TRUE(String(U'a', 10).Equals(u"aaaaaaaaaa"));
    EXPECT_TRUE(String(U'😋', 10).Equals(u"😋😋😋😋😋😋😋😋😋😋"));
    EXPECT_TRUE(String(u"a안녕😋cc").Equals(u"a안녕😋cc"));
    EXPECT_TRUE(String::Equals(String(u"a안녕😋cc"), u"a안녕😋cc"));
    EXPECT_TRUE(String::Equals(u"a안녕😋cc", String(u"a안녕😋cc")));
    EXPECT_FALSE(String::Equals(u"a안녕😋cac", String(u"a안녕😋cc")));
    EXPECT_FALSE(String::Equals(u"a안녕😋cca", String(u"a안녕😋cc")));
}

TEST(String, Remove)
{
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Remove(0, 5), u"楽yYYㅎあ");
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Remove(5, 5), u"QqQRrあ");
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Remove(5, 5).Length(), 6);
}

TEST(String, Contains)
{
    EXPECT_TRUE(String(U'😋', 10).Contains(u"😋"));
    EXPECT_FALSE(String(U'😋', 10).Contains(u"a"));
    EXPECT_TRUE(String(u"a안녕😋cc").Contains(u"안"));
    EXPECT_TRUE(String(u"a안녕😋cc").Contains(u"c"));
    EXPECT_TRUE(String(u"a안녕😋cc").Contains(u"😋"));
    EXPECT_FALSE(String(u"a안녕😋cc").Contains(u"Q"));
    EXPECT_TRUE(String(u"QqQRr楽yYYㅎあ").Contains(u'あ'));
    EXPECT_FALSE(String(u"QqQRr楽yYYㅎあ").Contains(u'궐'));
    EXPECT_TRUE(String(u"QqQRr楽yYYㅎあ").Contains(u"QRr楽yY"));
    EXPECT_FALSE(String(u"QqQRr楽yYYㅎあ").Contains(u"QRr楽yp"));
}

TEST(String, IndexOf)
{
    EXPECT_EQ(String(U'😋', 10).IndexOf(u"😋"), 0);
    EXPECT_EQ(String(u"a안녕😋cc").IndexOf(u"c"), 5);
    EXPECT_EQ(String(u"a안녕😋cc").IndexOf(u"😋c"), 3);
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").IndexOf(u"😋c"), -1);
}

TEST(String, IndexOfAny)
{
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").IndexOfAny({u'Q'}, 0), 0);
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").IndexOfAny({u'Q'}, 1), 2);
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").IndexOfAny({u'ㅎ'}, 1, 1), -1);
}

TEST(String, LastIndexOf)
{
    EXPECT_EQ(String(U'😋', 10).LastIndexOf(u'a'), -1);
    EXPECT_EQ(String(U'😋', 10).LastIndexOf(u"😋😋"), 16);
    EXPECT_EQ(String(U'😋', 10).LastIndexOf(u"😋😋"), 16);
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").LastIndexOf(u'Q'), 2);
    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u"c"), 4);
    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u"c", 3, 1), 3);
    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u"c", 3), 3);
    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u"c", 2), -1);
    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u"a", 0), 0);
    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u"cc", 3, 1), -1);
    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u'c'), 4);
    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u'c', 3, 1), 3);
    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u'c', 3), 3);
    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u'c', 2), -1);
    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u'a', 0), 0);
}

TEST(String, LastIndexOfAny)
{
    EXPECT_EQ(String(u"a안녕😋cc").LastIndexOfAny({u'a', u'안'}), 1);
    EXPECT_EQ(String(u"a안녕😋cc").LastIndexOfAny({u'a', u'안'}, 0), 0);
    EXPECT_EQ(String(u"a안녕😋cc").LastIndexOfAny({u'a', u'안'}, 1), 1);
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").LastIndexOfAny({u'楽'}, 4, 1), -1);
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").LastIndexOfAny({u'楽'}, 5, 1), 5);
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").LastIndexOfAny({u'楽'}, 6, 2), 5);
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").LastIndexOfAny({u'楽'}, 5, 0), -1);
}

TEST(String, StartsWith)
{
    EXPECT_FALSE(String(U'😋', 10).StartsWith(u"a안녕b"));
    EXPECT_TRUE(String(U'😋', 10).StartsWith(u"😋"));
    EXPECT_FALSE(String(U'😋', 10).StartsWith(u"a"));
    EXPECT_TRUE(String(u"a안녕😋cc").StartsWith(u"a안"));
    EXPECT_TRUE(String(u"a안녕😋cc").StartsWith(u"a안녕"));
    EXPECT_FALSE(String(u"a안녕😋cc").StartsWith(u"a안녕b"));
    EXPECT_TRUE(String(u"a안녕😋cc").StartsWith(u"A안녕😋C", true));
}

TEST(String, EndsWith)
{
    EXPECT_TRUE(String(u"QqQRr楽yYYㅎあ").EndsWith(u"yYYㅎあ"));
    EXPECT_TRUE(String(u"QqQRr楽yYYㅎあ").EndsWith(u"yyyㅎあ", true));
    EXPECT_FALSE(String(u"QqQRr楽yYYㅎあ").EndsWith(u"ayyㅎあ", true));
}

TEST(String, Substring)
{
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Substring(0), u"QqQRr楽yYYㅎあ");
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Substring(5), u"楽yYYㅎあ");
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Substring(2,7), u"QRr楽yYY");
}

TEST(String, PadLeft)
{
    EXPECT_EQ(String(u"a안녕ccatttt").PadLeft(15, u'#'), u"#####a안녕ccatttt");
    EXPECT_EQ(String(u"a안녕ccatttt").PadLeft(8, u'#'), u"a안녕ccatttt");
    EXPECT_EQ(String(u"a안녕ccatttt").PadLeft(15), u"     a안녕ccatttt");
    EXPECT_EQ(String(u"a안녕ccatttt").PadLeft(8), u"a안녕ccatttt");
}

TEST(String, PadRight)
{
    EXPECT_EQ(String(u"a안녕ccatttt").PadRight(15, u'#'), u"a안녕ccatttt#####");
    EXPECT_EQ(String(u"a안녕ccatttt").PadRight(8, u'#'), u"a안녕ccatttt");
    EXPECT_EQ(String(u"a안녕ccatttt").PadRight(15), u"a안녕ccatttt     ");
    EXPECT_EQ(String(u"a안녕ccatttt").PadRight(8), u"a안녕ccatttt");
}

TEST(String, Replace)
{
    EXPECT_EQ(String(u"a안녕ccatttt").Replace(u"안녕", u"こんにちは"), u"aこんにちはccatttt");
    EXPECT_EQ(String(u"a안녕ccatttt").Replace(u"안녕하", u"こんにちは"), u"a안녕ccatttt");
    EXPECT_EQ(String(u"a안녕ccatttt").Replace(u'안', u'こ'), u"aこ녕ccatttt");
    EXPECT_EQ(String(u"a안녕ccatttt").Replace(u'a', u'こ'), u"こ안녕ccatttt");
}

TEST(String, Trim)
{
    EXPECT_EQ(String(u"  QqQRr   楽yYYㅎあ  ").Trim(), u"QqQRr   楽yYYㅎあ");
    EXPECT_EQ(String(u"a안녕ccatttt").Trim({u't'}), u"a안녕cca");
    EXPECT_EQ(String(u"\na'qerqru\t").Trim(), u"a'qerqru");
    EXPECT_EQ(String(u"thel9lo a\tt").Trim({'t'}), u"hel9lo a\t");
    EXPECT_EQ(String(u"thel9lo a\tt").Trim(), u"thel9lo a\tt");
}

TEST(String, TrimStart)
{
    EXPECT_EQ(String(u"a안녕ccatttt").TrimStart({u't'}), u"a안녕ccatttt");
    EXPECT_EQ(String(u"\na'qerqru\t").TrimStart(), u"a'qerqru\t");
    EXPECT_EQ(String(u"thel9lo a\tt").TrimStart({'t'}), u"hel9lo a\tt");
}

TEST(String, TrimEnd)
{
    EXPECT_EQ(String(u"a안녕ccatttt").TrimEnd({u't'}), u"a안녕cca");
    EXPECT_EQ(String(u"\na'qerqru\t").TrimEnd(), u"\na'qerqru");
    EXPECT_EQ(String(u"thel9lo a\tt").TrimEnd({'t'}), u"thel9lo a\t");
}

TEST(String, ToUpper)
{
    EXPECT_EQ(String(u"a안녕😋cc").ToUpper(), u"A안녕😋CC");
    EXPECT_NE(String(u"a안녕😋cc").ToUpper(), u"a안녕😋cc");
}

TEST(String, ToLower)
{
    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").ToLower(), u"qqqrr楽yyyㅎあ");
}

TEST(String, Normalize)
{
    EXPECT_EQ(String(u"가나다라a").Normalize(NormalizationForm::FormC), u"가나다라a");
    EXPECT_EQ(String(u"가나다라a").Normalize(NormalizationForm::FormD), u"가나다라a");
    EXPECT_EQ(String(u"가나다라a").Normalize(NormalizationForm::FormC), u"가나다라a");
}

TEST(String, IsNormalized)
{
    EXPECT_TRUE(String(u"안녕a").IsNormalized(NormalizationForm::FormC));
    EXPECT_FALSE(String(u"안녕a").IsNormalized(NormalizationForm::FormC));
    EXPECT_TRUE(String(u"안녕a").IsNormalized(NormalizationForm::FormD));
    EXPECT_FALSE(String(u"안녕a").IsNormalized(NormalizationForm::FormD));
}