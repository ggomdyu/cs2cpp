#include "System.IO/FileStream.h"
#include "System.IO/Directory.h"
#include "System.IO/Path.h"

#include "TempDirectory.h"

CS2CPP_NAMESPACE_BEGIN

TempDirectory::TempDirectory(std::u16string_view path) :
    path_(path)
{
}

TempDirectory::TempDirectory() :
    path_(Directory::CreateDirectory(Path::GetRandomFileName())->ToString())
{
}

TempDirectory::~TempDirectory()
{
    Directory::Delete(path_, true);
}

std::u16string TempDirectory::AddTempFile()
{
    std::u16string path = Path::Combine(path_, Path::GetRandomFileName());
    std::shared_ptr<FileStream> fs = FileStream::Create(path, FileMode::OpenOrCreate);
    for (int i = 0; i < 1024; ++i)
    {
        fs->WriteByte(std::byte(i % 256));
    }

    fs->Close();

    return path;
}

std::u16string TempDirectory::AddTempFile(std::u16string_view extension)
{
    std::u16string path = Path::Combine(path_, Path::GetRandomFileName() + u"." + extension.data());

    auto fs = FileStream::Create(path, FileMode::OpenOrCreate);
    for (int i = 0; i < 1024; ++i)
    {
        fs->WriteByte(std::byte(i % 256));
    }

    fs->Close();

    return path;
}

std::u16string_view TempDirectory::GetPath() const
{
    return path_;
}

CS2CPP_NAMESPACE_END