#include "System/IO/FileStream.h"
#include "System/IO/Directory.h"

#include "TempDirectory.h"

namespace tg
{

TempDirectory::TempDirectory(std::u16string path) :
    _path(std::move(path))
{
}

TempDirectory::TempDirectory() :
    TempDirectory([&]()
    {
        return Directory::CreateDirectory(Path::GetRandomFileName()).ToString();
    } ())
{
}

TempDirectory::~TempDirectory()
{
    Directory::Delete(_path, true);
}

std::u16string TempDirectory::AddTempFile()
{
    auto path = Path::Combine(_path, Path::GetRandomFileName());

    auto fs = FileStream::Create(path, FileMode::OpenOrCreate);
    for (int i = 0; i < 1024; ++i)
        fs->WriteByte(std::byte(i % 256));

    fs->Close();

    return path;
}

std::u16string TempDirectory::AddTempFile(std::u16string_view extension)
{
    auto path = Path::Combine(_path, Path::GetRandomFileName() + u"." + extension.data());

    auto fs = FileStream::Create(path, FileMode::OpenOrCreate);
    for (int i = 0; i < 1024; ++i)
        fs->WriteByte(std::byte(i % 256));

    fs->Close();

    return path;
}

const std::u16string& TempDirectory::GetPath() const
{
    return _path;
}

}