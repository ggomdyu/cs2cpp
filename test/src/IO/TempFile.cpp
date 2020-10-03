#include "System/Environment.h"
#include "System.IO/File.h"
#include "System.IO/Path.h"

#include "TempFile.h"

CS2CPP_NAMESPACE_BEGIN

TempFile::TempFile(std::u16string path) :
    path_(std::move(path))
{
}

TempFile::TempFile() :
    TempFile([&]()
    {
        auto cd = Environment::GetCurrentDirectory();
        auto path = Path::Combine(cd, Path::GetRandomFileName());
        auto streamWriter = StreamWriter::Create(path);
        streamWriter->Write(Environment::StackTrace());

        return path;
    } ())
{
}

TempFile::~TempFile()
{
    File::Delete(path_);
}

const std::u16string& TempFile::GetPath() const
{
    return path_;
}

CS2CPP_NAMESPACE_END