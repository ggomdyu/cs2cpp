#include "System/Environment.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"

#include "TempFile.h"

namespace tg
{

TempFile::TempFile(std::u16string path) :
    _path(std::move(path))
{
}

TempFile::TempFile() :
    TempFile([&]()
    {
        auto cd = Environment::GetCurrentDirectory();
        auto path = Path::Combine(cd, Path::GetRandomFileName());

        auto fs = FileStream::Create(path, FileMode::OpenOrCreate);
        for (int i = 0; i < 1024; ++i)
            fs->WriteByte(std::byte(i % 256));

        fs->Close();

        return path;
    } ())
{
}

TempFile::~TempFile()
{
    File::Delete(_path);
}

const std::u16string& TempFile::GetPath() const
{
    return _path;
}

}