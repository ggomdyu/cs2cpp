//#include <array>
//#include <gtest/gtest.h>
//#include <System/String.h>
//
//using cs::String;
//using cs::StringSplitOptions;
//using cs::NormalizationForm;
//
//TEST(String, Compare)
//{
//   EXPECT_EQ(String::Compare(u"Hello", u"HELLop", true), -1);
//   EXPECT_EQ(String::Compare(u"Hello", u"Hello", true), 0);
//   EXPECT_EQ(String::Compare(u"Hello", u"HellO", true), 0);
//   EXPECT_EQ(String::Compare(u"Hello", u"HELLp", true), -1);
//   EXPECT_EQ(String::Compare(u"Helloppp", u"Hellop", true), 1);
//   EXPECT_EQ(String::Compare(u"aa", u"ab", true), -1);
//   EXPECT_EQ(String::Compare(u"ab", u"ac", true), -1);
//   EXPECT_EQ(String::Compare(u"가나", u"가가", true), 1);
//   EXPECT_EQ(String::Compare(u"가가", u"가나", true), -1);
//   EXPECT_EQ(String::Compare(u"He", u"HE", false), -1);
//   EXPECT_EQ(String::Compare(u"e", u"E", false), -1);
//   EXPECT_EQ(String::Compare(u"e", u"D", false), 1);
//   EXPECT_EQ(String::Compare(u"He", u"HE", true), 0);
//   EXPECT_EQ(String::Compare(u"e", u"E", true), 0);
//   EXPECT_EQ(String::Compare(u"e", u"D", true), 1);
//   EXPECT_EQ(String::Compare(u"Hello", u"Hello", true), 0);
//   EXPECT_EQ(String::Compare(u"Hello", u"HellO", true), 0);
//   EXPECT_EQ(String::Compare(u"Hello", u"HELLp", true), -1);
//   EXPECT_EQ(String::Compare(u"Helloppp", u"Hellop", true), 1);
//   EXPECT_EQ(String::Compare(u"Hello", u"Hello", false), 0);
//   EXPECT_EQ(String::Compare(u"Hello", u"HellO", false), -1);
//   EXPECT_EQ(String::Compare(u"Hello", u"HELLp", false), -1);
//   EXPECT_EQ(String::Compare(u"Helloppp", u"Hellop", false), 1);
//}
//
//TEST(String, Concat)
//{
//    EXPECT_EQ(String::Concat(u"SFえ", u"ＦＷあ아", u'월', String(u"dsdf")), u"SFえＦＷあ아월dsdf");
//    EXPECT_EQ(String::Concat(u"SFえ"), u"SFえ");
//    EXPECT_NE(String::Concat(u"SFえ"), u"SFえa");
//    EXPECT_NE(String::Concat(u"5ertwet", 4, u"w"), u"5ertwet4w");
//    EXPECT_NE(String::Concat(u"aa", 7), u"aa7");
//}
//
//TEST(String, Format)
//{
//    EXPECT_EQ(String::Format(u"{}{}{}", 1,2,3), u"123");
//    EXPECT_EQ(String::Format(u"{}", 1), u"1");
//}
//
//TEST(String, IsNullOrEmpty)
//{
//    EXPECT_TRUE(String::IsNullOrEmpty(u""));
//    EXPECT_FALSE(String::IsNullOrEmpty(u"a"));
//    EXPECT_FALSE(String::IsNullOrEmpty(u"\u2000"));
//    EXPECT_FALSE(String::IsNullOrEmpty(u" "));
//}
//
//TEST(String, IsNullOrWhiteSpace)
//{
//    EXPECT_TRUE(String::IsNullOrWhiteSpace(u""));
//    EXPECT_TRUE(String::IsNullOrWhiteSpace(String(u' ', 20)));
//    EXPECT_TRUE(String::IsNullOrWhiteSpace(u"  \t   "));
//    EXPECT_TRUE(String::IsNullOrWhiteSpace(String(u'\u2000', 10)));
//    EXPECT_FALSE(String::IsNullOrWhiteSpace(u"ABCDE"));
//    EXPECT_FALSE(String::IsNullOrWhiteSpace(u"ABCDE "));
//    EXPECT_FALSE(String::IsNullOrWhiteSpace(u" ABCDE"));
//    EXPECT_FALSE(String::IsNullOrWhiteSpace(u" ABCDE "));
//}
//
//TEST(String, CompareOrdinal)
//{
//    EXPECT_EQ(String::CompareOrdinal(u"a", u"ac"), -99);
//    EXPECT_EQ(String::CompareOrdinal(u"a", u"bc"), -1);
//    EXPECT_EQ(String::CompareOrdinal(u"a34rt34t346245v3", u"a44rt34t346245v3"), -1);
//    EXPECT_EQ(String::CompareOrdinal(u"a34rt34STt346245v3", u"a34rt34t346245v378j"), -3);
//}
//
//TEST(String, Join)
//{
//    EXPECT_EQ(String::Join(u",", {u"hi", u"min", u"Joe"}), u"hi,min,Joe");
//    EXPECT_EQ(String::Join(u"", {u"hi", u"min", u"Joe"}), u"himinJoe");
//    EXPECT_EQ(String::Join(u"ppap", {u"hi", u"min", u"Joe"}), u"hippapminppapJoe");
//    EXPECT_EQ(String::Join(u",", {u"hi"}), u"hi");
//    EXPECT_EQ(String::Join(u",", {}), u"");
//    EXPECT_EQ(String::Join(u"", {}), u"");
//}
//
//TEST(String, OperatorEqual)
//{
//    EXPECT_EQ(String(U'a', 10), u"aaaaaaaaaa");
//    EXPECT_EQ(String(U'😋', 10), u"😋😋😋😋😋😋😋😋😋😋");
//    EXPECT_EQ(std::u16string_view(String(U'a', 10)), std::u16string_view(u"aaaaaaaaaa"));
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ"), u"QqQRr楽yYYㅎあ");
//    EXPECT_EQ(String(u"  QqQRr   楽yYYㅎあ  "), u"  QqQRr   楽yYYㅎあ  ");
//    EXPECT_EQ(String(u""), u"");
//}
//
//TEST(String, OperatorNotEqual)
//{
//    EXPECT_NE(String(U'a', 10), u"aaaaaqaaaa");
//    EXPECT_NE(String(U'😋', 10), u"😋😋😋😋😋😋😋😋😋");
//    EXPECT_NE(String(U'😋', 10), u"😋😋😋😋😋a😋😋😋😋😋");
//}
//
//TEST(String, OperatorAssignment)
//{
//    EXPECT_EQ(String(U'a', 10) = u"asfdq", u"asfdq");
//    EXPECT_EQ((String(U'a', 10) = u"asfdq").Length(), 5);
//}
//
//TEST(String, OperatorAdditionAssignment)
//{
//    EXPECT_EQ(String(u"absabs") += u"erwcw", u"absabserwcw");
//    EXPECT_EQ(String(u"wwwerwkehr") += u"adsf", u"wwwerwkehradsf");
//    EXPECT_EQ(String(u"asfdq") += u"秋안原あ", u"asfdq秋안原あ");
//    EXPECT_EQ(String(u"asfdq") += u'あ', u"asfdqあ");
//    EXPECT_EQ((String(u"asfdq") += String(u"秋안原あ")).Length(), 9);
//    EXPECT_EQ((String(u"asfdq") += u'秋').Length(), 6);
//    EXPECT_EQ(String(u"asfdq秋안原あ") += u"큐", u"asfdq秋안原あ큐");
//    EXPECT_EQ((String(u"asfdq秋안原あ") += String(u"큐")).Length(), 10);
//    EXPECT_EQ((String(u"asfdq秋안原あ") += String(u"큐"))[3], u'd');
//}
//
//TEST(String, OperatorSubscript)
//{
//    String str(u"a안녕😋cc");
//    EXPECT_EQ(str[0], u'a');
//    EXPECT_EQ(str[1], u'안');
//    EXPECT_EQ(str[2], u'녕');
//    EXPECT_EQ(str[5], u'c');
//    EXPECT_EQ(str[6], u'c');
//}
//
//TEST(String, Insert)
//{
//    String str1(U'a', 10);
//    EXPECT_EQ(str1.Insert(0, u"あやや"), u"あややaaaaaaaaaa");
//    EXPECT_EQ(str1.Insert(9, u"あやや"), u"aaaaaaaaaあややa");
//    EXPECT_EQ(str1.Insert(10, u"あやや"), u"aaaaaaaaaaあやや");
//
//    String str2(u"sdfwfoij");
//    EXPECT_EQ(str2.Insert(0, u"あやや"), u"あややsdfwfoij");
//    EXPECT_EQ(str2.Insert(4, u""), u"sdfwfoij");
//}
//
//TEST(String, Split)
//{
//    String str1(U'a', 10);
//    EXPECT_EQ(str1.Split(u'a').size(), 11);
//    EXPECT_EQ(str1.Split({u'a'}).size(), 11);
//    EXPECT_EQ(str1.Split({u'a'})[0], u"");
//    EXPECT_EQ(str1.Split({u'a'})[10], u"");
//    EXPECT_EQ(str1.Split({u'a'}, StringSplitOptions::RemoveEmptyEntries).size(), 0);
//
//    String str2(u"QqQRr楽yYYㅎあ");
//    EXPECT_EQ(str2.Split({u'Y'}).size(), 3);
//    EXPECT_EQ(str2.Split({u'Y'})[0], u"QqQRr楽y");
//    EXPECT_EQ(str2.Split({u'Y'})[1], u"");
//    EXPECT_EQ(str2.Split({u'Y'})[2], u"ㅎあ");
//    EXPECT_EQ(str2.Split({u'Y'}, StringSplitOptions::RemoveEmptyEntries).size(), 2);
//    EXPECT_EQ(str2.Split({u'Y'}, StringSplitOptions::RemoveEmptyEntries)[0], u"QqQRr楽y");
//    EXPECT_EQ(str2.Split({u'Y'}, StringSplitOptions::RemoveEmptyEntries)[1], u"ㅎあ");
//
//    String str3(u"thel9lo a\tt");
//    EXPECT_EQ(str3.Split({u'o'}).size(), 2);
//    EXPECT_EQ(str3.Split({u'o'})[0], u"thel9l");
//    EXPECT_EQ(str3.Split({u'o'})[1], u" a\tt");
//    EXPECT_EQ(str3.Split({u'Q'}).size(), 1);
//    EXPECT_EQ(str3.Split({u'Q'})[0], u"thel9lo a\tt");
//}
//
//TEST(String, Length)
//{
//    EXPECT_EQ(String(U'a', 10).Length(), 10);
//    EXPECT_EQ(std::u16string_view(String(U'a', 10)).length(), 10);
//    EXPECT_EQ(String(U'😋', 10).Length(), 20);
//    EXPECT_EQ(String(u"a안녕😋cc").Length(), 7);
//    EXPECT_EQ(String(u"").Length(), 0);
//}
//
//TEST(String, Equals)
//{
//    EXPECT_TRUE(String(U'a', 10).Equals(u"aaaaaaaaaa"));
//    EXPECT_TRUE(String(U'😋', 10).Equals(u"😋😋😋😋😋😋😋😋😋😋"));
//    EXPECT_TRUE(String(u"a안녕😋cc").Equals(u"a안녕😋cc"));
//    EXPECT_TRUE(String::Equals(String(u"a안녕😋cc"), u"a안녕😋cc"));
//    EXPECT_TRUE(String::Equals(u"a안녕😋cc", String(u"a안녕😋cc")));
//    EXPECT_FALSE(String::Equals(u"a안녕😋cac", String(u"a안녕😋cc")));De
//    EXPECT_FALSE(String::Equals(u"a안녕😋cca", String(u"a안녕😋cc")));
//}
//
//TEST(String, Remove)
//{
//    EXPECT_EQ(String(u"abfbf가").Remove(0, 0), u"abfbf가");
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Remove(0, 5), u"楽yYYㅎあ");
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Remove(5, 5), u"QqQRrあ");
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").Remove(5, 5).Length(), 6);
//    EXPECT_EQ(String(u"wertwor2oi안녕").Remove(5), u"wertw");
//    EXPECT_EQ(String(u"wertwor2oi안녕").Remove(0), u"");
//    EXPECT_NE(String(u"wertwor2oi안녕").Remove(5), u"2oi안녕");
//    EXPECT_NE(String(u"abfbf가").Remove(0, 0), u"abfbㅁf가");
//}
//
//TEST(String, Contains)
//{
//    EXPECT_TRUE(String(U'😋', 10).Contains(u"😋"));
//    EXPECT_TRUE(String(u"a안녕😋cc").Contains(u"안"));
//    EXPECT_TRUE(String(u"a안녕😋cc").Contains(u"c"));
//    EXPECT_TRUE(String(u"a안녕😋cc").Contains(u"😋"));
//    EXPECT_TRUE(String(u"QqQRr楽yYYㅎあ").Contains(u'あ'));
//    EXPECT_TRUE(String(u"QqQRr楽yYYㅎあ").Contains(u"QRr楽yY"));
//    EXPECT_FALSE(String(U'😋', 10).Contains(u"a"));
//    EXPECT_FALSE(String(u"a안녕😋cc").Contains(u"Q"));
//    EXPECT_FALSE(String(u"QqQRr楽yYYㅎあ").Contains(u'궐'));
//    EXPECT_FALSE(String(u"QqQRr楽yYYㅎあ").Contains(u"QRr楽yp"));
//}
//
//TEST(String, IndexOf)
//{
//    EXPECT_EQ(String(U'😋', 10).IndexOf(u"😋"), 0);
//    EXPECT_EQ(String(u"a안녕😋cc").IndexOf(u"c"), 5);
//    EXPECT_EQ(String(u"a안녕😋cc").IndexOf(u"😋c"), 3);
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").IndexOf(u"😋c"), -1);
//}
//
//TEST(String, IndexOfAny)
//{
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").IndexOfAny({u'Q'}, 0), 0);
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").IndexOfAny({u'Q'}, 1), 2);
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").IndexOfAny({u'ㅎ'}, 1, 1), -1);
//}
//
//TEST(String, LastIndexOf)
//{
//    EXPECT_EQ(String(U'😋', 10).LastIndexOf(u'a'), -1);
//    EXPECT_EQ(String(U'😋', 10).LastIndexOf(u"😋😋"), 16);
//    EXPECT_EQ(String(U'😋', 10).LastIndexOf(u"😋😋"), 16);
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").LastIndexOf(u'Q'), 2);
//    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u"c"), 4);
//    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u"c", 3, 1), 3);
//    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u"c", 3), 3);
//    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u"c", 2), -1);
//    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u"a", 0), 0);
//    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u"cc", 3, 1), -1);
//    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u'c'), 4);
//    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u'c', 3, 1), 3);
//    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u'c', 3), 3);
//    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u'c', 2), -1);
//    EXPECT_EQ(String(u"a안녕ccatttt").LastIndexOf(u'a', 0), 0);
//}
//
//TEST(String, LastIndexOfAny)
//{
//    EXPECT_EQ(String(u"a안녕😋cc").LastIndexOfAny({u'a', u'안'}), 1);
//    EXPECT_EQ(String(u"a안녕😋cc").LastIndexOfAny({u'a', u'안'}, 0), 0);
//    EXPECT_EQ(String(u"a안녕😋cc").LastIndexOfAny({u'a', u'안'}, 1), 1);
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").LastIndexOfAny({u'楽'}, 4, 1), -1);
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").LastIndexOfAny({u'楽'}, 5, 1), 5);
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").LastIndexOfAny({u'楽'}, 6, 2), 5);
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").LastIndexOfAny({u'楽'}, 5, 0), -1);
//}
//
//TEST(String, StartsWith)
//{
//    EXPECT_TRUE(String(U'😋', 10).StartsWith(u"😋"));
//    EXPECT_TRUE(String(u"a안녕😋cc").StartsWith(u"a안"));
//    EXPECT_TRUE(String(u"a안녕😋cc").StartsWith(u"a안녕"));
//    EXPECT_TRUE(String(u"a안녕😋cc").StartsWith(u"A안녕😋C", true));
//    EXPECT_FALSE(String(U'😋', 10).StartsWith(u"a안녕b"));
//    EXPECT_FALSE(String(U'😋', 10).StartsWith(u"a"));
//    EXPECT_FALSE(String(u"a안녕😋cc").StartsWith(u"a안녕b"));
//}
//
//TEST(String, EndsWith)
//{
//    String str(u"QqQRr楽yYYㅎあ");
//    EXPECT_TRUE(str.EndsWith(u"yYYㅎあ"));
//    EXPECT_TRUE(str.EndsWith(u"yyyㅎあ", true));
//    EXPECT_FALSE(str.EndsWith(u"ayyㅎあ", true));
//}
//
//TEST(String, Substring)
//{
//    String str(u"QqQRr楽yYYㅎあ");
//    EXPECT_EQ(str.Substring(0), u"QqQRr楽yYYㅎあ");
//    EXPECT_EQ(str.Substring(5), u"楽yYYㅎあ");
//    EXPECT_EQ(str.Substring(2,7), u"QRr楽yYY");
//}
//
//TEST(String, PadLeft)
//{
//    String str(u"a안녕ccatttt");
//    EXPECT_EQ(str.PadLeft(15, u'#'), u"#####a안녕ccatttt");
//    EXPECT_EQ(str.PadLeft(8, u'#'), u"a안녕ccatttt");
//    EXPECT_EQ(str.PadLeft(15), u"     a안녕ccatttt");
//    EXPECT_EQ(str.PadLeft(8), u"a안녕ccatttt");
//}
//
//TEST(String, PadRight)
//{
//    String str(u"a안녕ccatttt");
//    EXPECT_EQ(str.PadRight(15, u'#'), u"a안녕ccatttt#####");
//    EXPECT_EQ(str.PadRight(8, u'#'), u"a안녕ccatttt");
//    EXPECT_EQ(str.PadRight(15), u"a안녕ccatttt     ");
//    EXPECT_EQ(str.PadRight(8), u"a안녕ccatttt");
//}
//
//TEST(String, Replace)
//{
//    String str1(u"a안녕ccatttt");
//    EXPECT_EQ(str1.Replace(u"안녕", u"こんにちは"), u"aこんにちはccatttt");
//    EXPECT_EQ(str1.Replace(u"안녕하", u"こんにちは"), str1);
//    EXPECT_EQ(str1.Replace(u"안녕하", u"こんにちは"), u"a안녕ccatttt");
//    EXPECT_EQ(str1.Replace(u'안', u'こ'), u"aこ녕ccatttt");
//    EXPECT_EQ(str1.Replace(u'a', u'こ'), u"こ안녕ccatttt");
//
//    String str2(u"Bㄱㄷㅅ후4tnio");
//    EXPECT_EQ(str2.Replace(u'후', u'b'), u"Bㄱㄷㅅb4tnio");
//    EXPECT_EQ(str2.Replace(u'q', u'p'), str2);
//
//    String str3(u"");
//    EXPECT_EQ(str3.Replace(u'a', u'b'), String::Empty);
//    EXPECT_EQ(str3.Replace(u"aaaa", u"bb"), u"");
//    EXPECT_EQ(str3.Replace(u"aa", u"bbbb"), u"");
//}
//
//TEST(String, Trim)
//{
//    EXPECT_EQ(String(u"  QqQRr   楽yYYㅎあ  ").Trim(), u"QqQRr   楽yYYㅎあ");
//    EXPECT_EQ(String(u"a안녕ccatttt").Trim({u't'}), u"a안녕cca");
//    EXPECT_EQ(String(u"\na'qerqru\t").Trim(), u"a'qerqru");
//    EXPECT_EQ(String(u"thel9lo a\tt").Trim({'t'}), u"hel9lo a\t");
//    EXPECT_EQ(String(u"thel9lo a\tt").Trim(), u"thel9lo a\tt");
//}
//
//TEST(String, TrimStart)
//{
//    EXPECT_EQ(String(u"a안녕ccatttt").TrimStart({u't'}), u"a안녕ccatttt");
//    EXPECT_EQ(String(u"\na'qerqru\t").TrimStart(), u"a'qerqru\t");
//    EXPECT_EQ(String(u"thel9lo a\tt").TrimStart({'t'}), u"hel9lo a\tt");
//}
//
//TEST(String, TrimEnd)
//{
//    EXPECT_EQ(String(u"a안녕ccatttt").TrimEnd({u't'}), u"a안녕cca");
//    EXPECT_EQ(String(u"\na'qerqru\t").TrimEnd(), u"\na'qerqru");
//    EXPECT_EQ(String(u"thel9lo a\tt").TrimEnd({'t'}), u"thel9lo a\t");
//}
//
//TEST(String, ToUpper)
//{
//    EXPECT_EQ(String(u"a안녕😋cc").ToUpper(), u"A안녕😋CC");
//    EXPECT_NE(String(u"a안녕😋cc").ToUpper(), u"a안녕😋cc");
//}
//
//TEST(String, ToLower)
//{
//    EXPECT_EQ(String(u"QqQRr楽yYYㅎあ").ToLower(), u"qqqrr楽yyyㅎあ");
//}
//
//TEST(String, Normalize)
//{
//    EXPECT_EQ(String(u"가나다라a").Normalize(NormalizationForm::FormC), u"가나다라a");
//    EXPECT_EQ(String(u"가나다라a").Normalize(NormalizationForm::FormD), u"가나다라a");
//    EXPECT_EQ(String(u"가나다라a").Normalize(NormalizationForm::FormC), u"가나다라a");
//}
//
//TEST(String, IsNormalized)
//{
//    EXPECT_TRUE(String(u"안녕a").IsNormalized(NormalizationForm::FormC));
//    EXPECT_TRUE(String(u"안녕a").IsNormalized(NormalizationForm::FormD));
//    EXPECT_FALSE(String(u"안녕a").IsNormalized(NormalizationForm::FormC));
//    EXPECT_FALSE(String(u"안녕a").IsNormalized(NormalizationForm::FormD));
//}
//
//TEST(String, GetHashCode)
//{
//   EXPECT_EQ(String(u"").GetHashCode(), 0);
//   EXPECT_NE(String(u"qwercqwe").GetHashCode(), 0);
//}
//
//TEST(String, CopyTo)
//{
//   String str(u"QqQr楽");
//
//   std::array<char16_t, 32> charArray1{};
//   str.CopyTo(0, charArray1, str.Length());
//   EXPECT_EQ(charArray1[0], u'Q');
//   EXPECT_EQ(charArray1[1], u'q');
//   EXPECT_EQ(charArray1[2], u'Q');
//   EXPECT_EQ(charArray1[3], u'r');
//   EXPECT_EQ(charArray1[4], u'楽');
//   EXPECT_EQ(charArray1[5], u'\0');
//
//   std::array<char16_t, 32> charArray2{};
//   str.CopyTo(0, charArray2, str.Length() - 1);
//   EXPECT_EQ(charArray2[0], u'Q');
//   EXPECT_EQ(charArray2[1], u'q');
//   EXPECT_EQ(charArray2[2], u'Q');
//   EXPECT_EQ(charArray2[3], u'r');
//   EXPECT_EQ(charArray2[4], u'\0');
//
//   String str2(u" 가 a");
//   std::array<char16_t, 32> charArray3{};
//   str2.CopyTo(0, charArray3, str2.Length() - 1);
//   EXPECT_EQ(charArray3[0], u' ');
//   EXPECT_EQ(charArray3[1], u'가');
//   EXPECT_EQ(charArray3[2], u' ');
//   EXPECT_EQ(charArray3[3], u'\0');
//}
//
//TEST(String, ToCharArray)
//{
//   auto charArray = String(u"a안녕tacc").ToCharArray();
//   EXPECT_EQ(charArray[0], u'a');
//   EXPECT_EQ(charArray[1], u'안');
//   EXPECT_EQ(charArray[2], u'녕');
//   EXPECT_EQ(charArray[3], u't');
//   EXPECT_EQ(charArray[4], u'a');
//   EXPECT_EQ(charArray[5], u'c');
//   EXPECT_EQ(charArray[6], u'c');
//   EXPECT_EQ(charArray.size(), 7);
//
//   auto charArray2 = String(u"a안녕tacc").ToCharArray(1,3);
//   EXPECT_EQ(charArray2[0], u'안');
//   EXPECT_EQ(charArray2[1], u'녕');
//   EXPECT_EQ(charArray2[2], u't');
//   EXPECT_EQ(charArray2.size(), 3);
//}
