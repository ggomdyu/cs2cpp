//#pragma once
//
//#include <System/Diagnostics/Debug.h>
//#include <System/IO/File.h>
//#include <System/UEnvironment.h>
//
//#include "../Test.h"
//
//CS2CPP_NAMESPACE_BEGIN
//
//class FileTest :
//    public Test
//{
//public:
//    void Evaluate() override
//    {
//        CreateTemporaryFile();
//
//        auto desktopPath = UEnvironment::GetFolderPath(UEnvironment::SpecialFolder::Desktop);
//        auto srcImagePath = UEnvironment::GetFolderPath(UEnvironment::SpecialFolder::Desktop) + u"/qwexqwexqw.png";
//
//        auto dtl = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Local);
//        auto dtu = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Utc);
//
//        assert(File::SetCreationTime(srcImagePath, dtl));
//        assert(File::GetCreationTime(srcImagePath).value() == dtu);
//        assert(File::SetCreationTime(srcImagePath, dtu));
//        assert(File::GetCreationTime(srcImagePath).value() == dtu.AddHours(9.0));
//
//        File::SetCreationTimeUtc(srcImagePath, dtu);
//        assert(File::GetCreationTimeUtc(srcImagePath).value() == dtu);
//        File::SetCreationTimeUtc(srcImagePath, dtl);
//        assert(File::GetCreationTimeUtc(srcImagePath).value() == dtu.AddHours(-9.0));
//
//        File::SetLastWriteTime(srcImagePath, dtl);
//        DateTime c = File::GetLastWriteTime(srcImagePath).value();
//        assert(c == dtu);
//        File::SetLastWriteTime(srcImagePath, dtu);
//        c = File::GetLastWriteTime(srcImagePath).value();
//        assert(c == dtu.AddHours(9.0));
//
//        File::SetLastAccessTimeUtc(srcImagePath, dtu);
//        assert(File::GetLastAccessTimeUtc(srcImagePath).value() == dtu);
//        File::SetLastAccessTimeUtc(srcImagePath, dtl);
//        assert(File::GetLastAccessTimeUtc(srcImagePath).value() == dtu.AddHours(-9.0));
//
//        File::SetLastWriteTime(srcImagePath, dtl);
//        assert(File::GetLastWriteTime(srcImagePath).value() == dtu);
//        File::SetLastWriteTime(srcImagePath, dtu);
//        assert(File::GetLastWriteTime(srcImagePath).value() == dtu.AddHours(9.0));
//
//        File::SetLastWriteTimeUtc(srcImagePath, dtu);
//        assert(File::GetLastWriteTimeUtc(srcImagePath).value() == dtu);
//        File::SetLastWriteTimeUtc(srcImagePath, dtl);
//        assert(File::GetLastWriteTimeUtc(srcImagePath).value() == dtu.AddHours(-9.0));
//
//        auto destImagePath = desktopPath + "/1_q43289n231nk.png";
//        File::Move(srcImagePath, destImagePath);
//        assert(File::Exists(destImagePath));
//        assert(File::Exists(srcImagePath) == false);
//        File::Move(destImagePath, srcImagePath);
//
//        File::Copy(srcImagePath, destImagePath);
//        assert(File::Exists(destImagePath));
//
//        File::Delete(destImagePath);
//        assert(File::Exists(destImagePath) == false);
//
//        File::Delete(srcImagePath);
//    }
//
//private:
//    void CreateTemporaryFile()
//    {
//        auto filePath = UEnvironment::GetFolderPath(UEnvironment::SpecialFolder::Desktop) + u"/qwexqwexqw.png";
//        File::Delete(&filePath[0]);
//
//        FileStream f3(filePath, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::None);;
//        for (int i = 0; i < 1024; ++i)
//        {
//            f3.WriteByte(std::byte(i));
//        }
//        f3.Close();
//    }
//};
//
//CS2CPP_NAMESPACE_END