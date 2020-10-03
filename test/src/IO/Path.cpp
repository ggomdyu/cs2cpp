//#pragma once
//
//#include <System/IO/Path.h>
//
//#include "../Test.h"
//
//CS2CPP_NAMESPACE_BEGIN
//
//class PathTest :
//    public Test
//{
//public:
//    void Evaluate() override
//    {
//        assert(Path::GetPathRoot(u"/Users/chajunho/") == u"/");
//        assert(Path::GetPathRoot(u"/") == u"/");
//        assert(Path::GetPathRoot(u"/Users") == u"/");
//        assert(Path::GetPathRoot(u"") == u"");
//        assert(Path::GetPathRoot(u"C:/") == u"");
//        assert(Path::GetPathRoot(u"C:") == u"");
//        assert(Path::GetPathRoot(u"C://") == u"");
//        assert(Path::GetPathRoot(u"C:\\") == u"");
//        assert(Path::GetPathRoot(u"\\Users\\chajunho\\") == u"");
//        assert(Path::GetPathRoot(u"\\") == u"");
//        assert(Path::HasExtension(u"/Users/chajunho/Desktop/1.png") == true);
//        assert(Path::HasExtension(u"/Users/chajunho/Desktop/1") == false);
//        assert(Path::HasExtension(u"/Users/chajunho/Desktop/") == false);
//        assert(Path::HasExtension(u"/Users/chajunho/Desktop") == false);
//        assert(Path::HasExtension(u"1.png") == true);
//        assert(Path::HasExtension(u".png") == true);
//        assert(Path::HasExtension(u"png") == false);
//        assert(Path::GetExtension(u"/Users/chajunho/Desktop/1.png") == u".png");
//        assert(Path::GetExtension(u"/Users/chajunho/Desktop/1") == u"");
//        assert(Path::GetExtension(u"1.png") == u".png");
//        assert(Path::GetExtension(u"1") == u"");
//        assert(Path::GetExtension(u"") == u"");
//        assert(Path::GetFileName(u"/Users/chajunho/Desktop/1.png") == u"1.png");
//        assert(Path::GetFileName(u"") == u"");
//        assert(Path::GetFileNameWithoutExtension(u"") == u"");
//        assert(Path::GetFileNameWithoutExtension(u"/Users/chajunho/Desktop/123") == u"123");
//        assert(Path::GetFileNameWithoutExtension(u"/Users/chajunho/Desktop/1") == u"1");
//        assert(Path::GetFileNameWithoutExtension(u"/Users/123123.png") == u"123123");
//        assert(Path::GetFileNameWithoutExtension(u"/Users/chajunho/Desktop/1.png") == u"1");
//        assert(Path::GetFileNameWithoutExtension(u"/Users/chajunho/Desktop/1.1.png") == u"1.1");
//        assert(Path::GetFileNameWithoutExtension(u"1") == u"1");
//        assert(Path::GetFileNameWithoutExtension(u"1.png") == u"1");
//        assert(Path::GetFileNameWithoutExtension(u"123123") == u"123123");
//        assert(Path::ChangeExtension(u"/Users/chajunho/Desktop/1.png", u"ext") == u"/Users/chajunho/Desktop/1.ext");
//        assert(Path::ChangeExtension(u".png", u"ext") == u".ext");
//        assert(Path::ChangeExtension(u"/Users/chajunho/Desktop", u"ext") == u"/Users/chajunho/Desktop.ext");
//        assert(Path::ChangeExtension(u"/Users/chajunho/Desktop/", u"ext") == u"/Users/chajunho/Desktop/.ext");
//        assert(Path::ChangeExtension(u"1.png", u"ext") == u"1.ext");
//        assert(Path::ChangeExtension(u"png", u"ext") == u"png.ext");
//        assert(Path::ChangeExtension(u"", u"ext") == u"");
//        assert(Path::ChangeExtension(u"", u"..ext") == u"");
//        assert(Path::ChangeExtension(u"", u"") == u"");
//        assert(Path::ChangeExtension(u"a", u".ext") == u"a.ext");
//        assert(Path::ChangeExtension(u"a", u"ext") == u"a.ext");
//        assert(Path::ChangeExtension(u"a", u"..ext") == u"a..ext");
//#if CS2CPP_PLATFORM_WINDOWS
//        assert(Path::Combine(u"dec", u"col") == u"dec\\col");
//        assert(Path::Combine(u"dec/", u"col") == u"dec/col");
//        assert(Path::Combine(u"dec", u"col/") == u"dec\\col/");
//        assert(Path::Combine(u"dec/", u"col/.png") == u"dec/col/.png");
//        assert(Path::Combine(u"C:/col/2.png", u"C:/col/.png") == u"C:/col/.png");
//        assert(Path::Combine(u"col/2.png", u"") == u"col/2.png");
//        assert(Path::Combine(u"", u"col/2.png") == u"col/2.png");
//        assert(Path::Combine(u"/User/Sys", u"ibs") == u"/User/Sys\\ibs");
//        assert(Path::Combine(u"/User/Sys", u"/ibs") == u"/ibs");
//        assert(Path::Combine(u"/User/Sys/", u"/ibs") == u"/ibs");
//        assert(Path::Combine(u"", u"") == u"");
//        assert(Path::Combine(u"/User/Sys", u"C:/ibs") == u"C:/ibs");
//        assert(Path::Combine(u"C:/User/Sys", u"C:/ibs") == u"C:/ibs");
//
//        assert(Path::GetPathRoot(u"/Users/chajunho/") == u"\\");
//        assert(Path::GetPathRoot(u"/") == u"\\");
//        assert(Path::GetPathRoot(u"/Users") == u"\\");
//        assert(Path::GetPathRoot(u"") == u"");
//        assert(Path::GetPathRoot(u"C:/") == u"C:\\");
//        assert(Path::GetPathRoot(u"C:") == u"C:");
//        assert(Path::GetPathRoot(u"C://") == u"C:\\");
//        assert(Path::GetPathRoot(u"C:\\") == u"C:\\");
//        assert(Path::GetPathRoot(u"\\Users\\chajunho\\") == u"\\");
//        assert(Path::GetPathRoot(u"\\") == u"\\");
//#elif CS2CPP_PLATFORM_MACOS
//        assert(Path::Combine(u"dec", u"col") == u"dec/col");
//        assert(Path::Combine(u"dec/", u"col") == u"dec/col");
//        assert(Path::Combine(u"dec", u"col/") == u"dec/col/");
//        assert(Path::Combine(u"dec/", u"col/.png") == u"dec/col/.png");
//        assert(Path::Combine(u"col/2.png", u"") == u"col/2.png");
//        assert(Path::Combine(u"", u"col/2.png") == u"col/2.png");
//        assert(Path::Combine(u"/User/Sys", u"ibs") == u"/User/Sys/ibs");
//        assert(Path::Combine(u"/User/Sys", u"/ibs") == u"/ibs");
//        assert(Path::Combine(u"/User/Sys/", u"/ibs") == u"/ibs");
//        assert(Path::Combine(u"", u"") == u"");
//        assert(Path::Combine(u"C:/col/2.png", u"C:/col/.png") == u"C:/col/2.png/C:/col/.png");
//        assert(Path::Combine(u"/User/Sys", u"C:/ibs") == u"/User/Sys/C:/ibs");
//        assert(Path::Combine(u"C:/User/Sys", u"C:/ibs") == u"C:/User/Sys/C:/ibs");
//
//        assert(Path::GetPathRoot(u"/Users/chajunho/") == u"/");
//        assert(Path::GetPathRoot(u"/") == u"/");
//        assert(Path::GetPathRoot(u"/Users") == u"/");
//        assert(Path::GetPathRoot(u"") == u"");
//        assert(Path::GetPathRoot(u"C:/") == u"");
//        assert(Path::GetPathRoot(u"C:") == u"");
//        assert(Path::GetPathRoot(u"C://") == u"");
//        assert(Path::GetPathRoot(u"C:\\") == u"");
//        assert(Path::GetPathRoot(u"\\Users\\chajunho\\") == u"");
//        assert(Path::GetPathRoot(u"\\") == u"");
//#endif
//        assert(Path::GetDirectoryName(u"C:/1.png") == u"C:");
//        assert(Path::GetDirectoryName(u"C:\\1.png") == u"");
//        assert(Path::GetDirectoryName(u"C:/Dir/1.png") == u"C:/Dir");
//        assert(Path::GetDirectoryName(u"/Users/chajunho/Desktop/1.png") == u"/Users/chajunho/Desktop");
//        assert(Path::GetDirectoryName(u"") == u"");
//        assert(Path::GetDirectoryName(u"abc.png") == u"");
//        assert(Path::GetDirectoryName(u"abc") == u"");
//        assert(Path::GetFullPath(u"") == u"");
//        assert(Path::GetFullPath(u"/") == u"/");
//    }
//
//private:
//
//};
//
//CS2CPP_NAMESPACE_END