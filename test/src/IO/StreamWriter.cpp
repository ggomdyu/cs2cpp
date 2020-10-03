#include <System.IO/File.h>
#include <System.IO/Path.h>
#include <System.IO/StreamWriter.h>
#include <System/Environment.h>
#include <System/Random.h>
#include <gtest/gtest.h>

#include "TempFile.h"

using namespace CS2CPP_NAMESPACE_NAME;

bool CheckFileData(std::u16string_view path, const char* data)
{
    auto bytes = File::ReadAllBytes(path);
    auto dataLen = strlen(data);
    if (bytes->size() != dataLen)
        return false;

    int i = 0;
    for (auto byte : *bytes)
    {
        if ((char)byte != data[i++])
            return false;
    }

    return true;
}

TEST(StreamWriter, Write)
{
    auto file = TempFile(Path::GetRandomFileName());
    auto sw = StreamWriter::Create(file.GetPath());
    EXPECT_TRUE(File::Exists(file.GetPath()));
    sw->Write(true);
    sw->Write(false);
    EXPECT_TRUE(File::Exists(file.GetPath()));
    sw->Close();
    EXPECT_TRUE(CheckFileData(file.GetPath(), "TrueFalse"));

    file = TempFile(Path::GetRandomFileName());
    sw = StreamWriter::Create(file.GetPath());
    sw->Write(3);
    sw->Write(57888L);
    sw->Close();
    EXPECT_TRUE(CheckFileData(file.GetPath(), "357888"));

    file = TempFile(Path::GetRandomFileName());
    sw = StreamWriter::Create(file.GetPath());
    sw->Write(std::u16string_view(u"ASFASD"));
    sw->Write(u"AQQQQ");
    sw->Close();
    EXPECT_TRUE(CheckFileData(file.GetPath(), "ASFASDAQQQQ"));

//    file = TempFile(Path::GetRandomFileName());
//    sw = StreamWriter::Create(file.GetPath());
//    sw->Write(std::u16string_view(u"ASFASD{}"), 5);
//    sw->Write(u"AQQQQ");
//    sw->Close();
//    EXPECT_TRUE(CheckFileData(file.GetPath(), "ASFASD5AQQQQ"));
}