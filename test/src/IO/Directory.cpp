//#pragma once
//
//#include <System/IO/Path.h>
//#include <System/Environment.h>
//
//#include "../Test.h"
//
//CS2CPP_NAMESPACE_BEGIN
//
//class DirectoryTest :
//    public Test
//{
//public:
//    void Evaluate() override
//    {
//        using namespace tgon;
//
//        auto desktopPath = Environment::GetFolderPath(Environment::SpecialFolder::Desktop);
//        auto newDirPath = Path::Combine(desktopPath, "WA3eERCWER3ni");
//        Directory::CreateDirectory(newDirPath.c_str());
//
//        assert(Directory::Exists(newDirPath.c_str()) == true);
//
//        auto newDirPath2 = Path::Combine(desktopPath, "oiwjciohwu");
//        assert(Directory::Move(newDirPath.c_str(), newDirPath2.c_str()) == true);
//        assert(Directory::Exists(newDirPath.c_str()) == false);
//        assert(Directory::Exists(newDirPath2.c_str()) == true);
//
//        assert(Directory::GetParent("").ToString() == "");
//        assert(Directory::GetParent("/").ToString() == "/");
//        assert(Directory::GetParent("/Users/chajunho/Desktop").ToString() == "/Users/chajunho/");
//        assert(Directory::GetParent("/Users").ToString() == "/");
//        assert(Directory::GetParent("/a").ToString() == "/");
//
//        auto prevCurrDir = Directory::GetCurrentDirectory();
//        assert(Directory::SetCurrentDirectory("/Users") == true);
//        assert(Directory::GetCurrentDirectory() == "/Users");
//        assert(Directory::SetCurrentDirectory(prevCurrDir.c_str()) == true);
//    }
//};
//
//CS2CPP_NAMESPACE_END