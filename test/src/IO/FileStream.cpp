//#pragma once
//
//#include <System/IO/FileStream.h>
//#include <System/Environment.h>
//
//#include "../Test.h"
//
//CS2CPP_NAMESPACE_BEGIN
//
//class FileStreamTest :
//    public Test
//{
//public:
//    class CustomFileStream : public FileStream
//    {
//    public:
//        using FileStream::FileStream;
//        using FileStream::m_writePos;
//        using FileStream::m_readPos;
//        using FileStream::m_readLen;
//        using FileStream::m_filePos;
//        using FileStream::m_buffer;
//    };
//
//public:
//    void Evaluate() override
//    {
//        CreateTemporaryFile();
//
//        auto filePath = *Environment::GetFolderPath(Environment::SpecialFolder::Desktop) + "/qwexqds2";
//        const char str[] = "wer123f2534636433g3634357345f34aaaaf59375f345f7345f35f3ff213d12d31d21d1d123123we46546wer123f2534636433g3634357345f34aaaaf59375f345f7345f35f3ff213d12d31d21d1d123123we46546wer123f2534636433g3634357345f34aaaaf59375f345f7345f35f3ff213d12d31d21d1d123123we46546";
//        const char str2[] = "wer123f2534636433g3634357345f34aaaaf59375f345f7345f35f3ff213d12d31d21d1d123123we46546";
//        CustomFileStream f2(filePath, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::None);
//#if CS2CPP_PLATFORM_WINDOWS
//        {
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 0 && f2.m_writePos == 0 && f2.Length() == 256);
//            f2.SetLength(2050);
//            std::byte buffer[1024]{};
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 0 && f2.m_writePos == 0 && f2.Length() == 2050);
//            f2.Write(reinterpret_cast<const std::byte*>(str2), std::extent_v<decltype(str2)> -1);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 0 && f2.m_writePos == 85 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            f2.Write(reinterpret_cast<const std::byte*>(str), std::extent_v<decltype(str)> -1);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 0 && f2.m_writePos == 340 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(51) && f2.m_buffer[120] == std::byte(102));
//            auto cc1 = (char)f2.ReadByte();
//            assert(f2.m_readLen == 1710 && f2.m_readPos == 1 && f2.m_filePos == 2050 && f2.m_writePos == 0 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(0) && f2.m_buffer[20] == std::byte(0) && f2.m_buffer[50] == std::byte(0) && f2.m_buffer[80] == std::byte(0) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            auto cc2 = (char)f2.ReadByte();
//            assert(f2.m_readLen == 1710 && f2.m_readPos == 2 && f2.m_filePos == 2050 && f2.m_writePos == 0 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(0) && f2.m_buffer[20] == std::byte(0) && f2.m_buffer[50] == std::byte(0) && f2.m_buffer[80] == std::byte(0) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            auto cc3 = (char)f2.ReadByte();
//            assert(f2.m_readLen == 1710 && f2.m_readPos == 3 && f2.m_filePos == 2050 && f2.m_writePos == 0 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(0) && f2.m_buffer[20] == std::byte(0) && f2.m_buffer[50] == std::byte(0) && f2.m_buffer[80] == std::byte(0) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            auto cc4 = (char)f2.ReadByte();
//            assert(f2.m_readLen == 1710 && f2.m_readPos == 4 && f2.m_filePos == 2050 && f2.m_writePos == 0 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(0) && f2.m_buffer[20] == std::byte(0) && f2.m_buffer[50] == std::byte(0) && f2.m_buffer[80] == std::byte(0) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            f2.WriteByte(std::byte(255));
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 344 && f2.m_writePos == 1 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(0) && f2.m_buffer[20] == std::byte(0) && f2.m_buffer[50] == std::byte(0) && f2.m_buffer[80] == std::byte(0) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            f2.Seek(50, SeekOrigin::Current);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 395 && f2.m_writePos == 0 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(0) && f2.m_buffer[20] == std::byte(0) && f2.m_buffer[50] == std::byte(0) && f2.m_buffer[80] == std::byte(0) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            f2.Seek(185, SeekOrigin::Begin);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 185 && f2.m_writePos == 0 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(0) && f2.m_buffer[20] == std::byte(0) && f2.m_buffer[50] == std::byte(0) && f2.m_buffer[80] == std::byte(0) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            f2.ReadByte();
//            assert(f2.m_readLen == 1865 && f2.m_readPos == 1 && f2.m_filePos == 2050 && f2.m_writePos == 0 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(102) && f2.m_buffer[50] == std::byte(49) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(53) && f2.m_buffer[120] == std::byte(102));
//            f2.Write(reinterpret_cast<const std::byte*>(str), std::extent_v<decltype(str)> -1);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 186 && f2.m_writePos == 255 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(51) && f2.m_buffer[120] == std::byte(102));
//            f2.ReadByte();
//            assert(f2.m_readLen == 1609 && f2.m_readPos == 1 && f2.m_filePos == 2050 && f2.m_writePos == 0 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(0) && f2.m_buffer[20] == std::byte(0) && f2.m_buffer[50] == std::byte(0) && f2.m_buffer[80] == std::byte(0) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            f2.Write(reinterpret_cast<const std::byte*>(str2), std::extent_v<decltype(str2)> -1);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 442 && f2.m_writePos == 85 && f2.Length() == 2050 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            f2.SetLength(50);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 50 && f2.m_writePos == 0 && f2.Length() == 50 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            f2.ReadByte();
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 50 && f2.m_writePos == 0 && f2.Length() == 50 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            f2.ReadByte();
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 50 && f2.m_writePos == 0 && f2.Length() == 50 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            f2.ReadByte();
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 50 && f2.m_writePos == 0 && f2.Length() == 50 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//        }
//#else // Test for SetLength unsupported platform
//        {
//            std::byte buffer[1024]{};
//            f2.Write(reinterpret_cast<const std::byte*>(str2), std::extent_v<decltype(str2)> -1);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 0 && f2.m_writePos == 85 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(0) && f2.m_buffer[120] == std::byte(0));
//            f2.Write(reinterpret_cast<const std::byte*>(str), std::extent_v<decltype(str)> -1);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 0 && f2.m_writePos == 340 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(51) && f2.m_buffer[120] == std::byte(102));
//            auto cc1 = (char)f2.ReadByte();
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 340 && f2.m_writePos == 0 && cc1 == -1);
//            auto cc2 = (char)f2.ReadByte();
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 340 && f2.m_writePos == 0 && cc2 == -1);
//            auto cc3 = (char)f2.ReadByte();
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 340 && f2.m_writePos == 0 && cc3 == -1);
//            auto cc4 = (char)f2.ReadByte();
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 340 && f2.m_writePos == 0 && cc4 == -1);
//            f2.WriteByte(std::byte(255));
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 340 && f2.m_writePos == 1 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(51) && f2.m_buffer[120] == std::byte(102));
//            f2.Seek(50, SeekOrigin::Current);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 391 && f2.m_writePos == 0 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(51) && f2.m_buffer[120] == std::byte(102));
//            f2.Seek(185, SeekOrigin::Begin);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 185 && f2.m_writePos == 0 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(51) && f2.m_buffer[120] == std::byte(102));
//            auto cc5 = f2.ReadByte();
//            assert(f2.m_readLen == 156 && f2.m_readPos == 1 && f2.m_filePos == 341 && f2.m_writePos == 0 && cc5 == 51);
//            f2.Write(reinterpret_cast<const std::byte*>(str), std::extent_v<decltype(str)> -1);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 186 && f2.m_writePos == 255 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(51) && f2.m_buffer[120] == std::byte(102));
//            auto cc6 = f2.ReadByte();
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 441 && f2.m_writePos == 0 && cc6 == -1);
//            f2.Write(reinterpret_cast<const std::byte*>(str2), std::extent_v<decltype(str2)> -1);
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 441 && f2.m_writePos == 85 && f2.m_buffer[5] == std::byte(51) && f2.m_buffer[20] == std::byte(51) && f2.m_buffer[50] == std::byte(102) && f2.m_buffer[80] == std::byte(52) && f2.m_buffer[110] == std::byte(51) && f2.m_buffer[120] == std::byte(102));
//            auto cc7 = f2.ReadByte();
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 526 && f2.m_writePos == 0 && cc7 == -1);
//            auto cc8 = f2.ReadByte();
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 526 && f2.m_writePos == 0 && cc8 == -1);
//            auto cc9 = f2.ReadByte();
//            assert(f2.m_readLen == 0 && f2.m_readPos == 0 && f2.m_filePos == 526 && f2.m_writePos == 0 && cc9 == -1);
//        }
//#endif
//
//        File::Delete(filePath);
//    }
//
//private:
//    void CreateTemporaryFile()
//    {
//        auto filePath = *Environment::GetFolderPath(Environment::SpecialFolder::Desktop) + "/qwexqds2";
//        File::Delete(filePath);
//
//        FileStream f3(filePath, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::None);;
//        for (int i = 0; i < 256; ++i)
//        {
//            f3.WriteByte(std::byte(i));
//        }
//        f3.Close();
//    }
//};
//
//CS2CPP_NAMESPACE_END