#include <deque>
#include <dirent.h>
#include <fnmatch.h>

#include "System.IO.Enumeration/DirectoryIterator.h"
#include "System.IO/Path.h"
#include "System.Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

using UniqueDirHandle = std::unique_ptr<DIR, decltype(&closedir)>;

static bool IsPatternMatched(std::string_view fileName, std::string_view searchPattern)
{
    if (searchPattern == "*")
    {
        return true;
    }

    return fnmatch(searchPattern.data(), fileName.data(), 0) == 0;
}

static bool IsDotDirectory(std::string_view path) noexcept
{
    return (path.length() == 1 && path[0] == '.') || (path.length() == 2 && path == "..");
}

static bool TryFindNextFile(std::string_view path, UniqueDirHandle* handle, struct dirent** ent)
{
    if (!*handle)
    {
        if (!(*handle = {opendir(path.data()), &closedir}))
            return false;
    }

    return (*ent = readdir(handle->get()));
}

static std::string GetFullPath(std::string_view basePath, std::string_view fileName)
{
    std::string fullPath(basePath);
    bool hasSeparator = Path::IsDirectorySeparator(basePath.back()) || Path::IsVolumeSeparator(basePath.back());
    if (!hasSeparator)
    {
        fullPath += Path::DirectorySeparatorChar;
    }

    fullPath += fileName;

    return fullPath;
}

static std::string Utf16ToUtf8(std::u16string_view str)
{
    auto utf8Bytes = Encoding::UTF8().GetBytes(str);
    if (!utf8Bytes)
    {
        return {};
    }

    return {reinterpret_cast<const char*>(utf8Bytes->data()), utf8Bytes->size()};
}

static std::u16string Utf8ToUtf16(std::string_view str)
{
    auto utf16Str = Encoding::UTF8().GetString({reinterpret_cast<const std::byte*>(str.data()), static_cast<int32_t>(str.length())});
    if (!utf16Str)
    {
        return {};
    }

    return *utf16Str;
}

class IteratorImpl final
{
public:
    IteratorImpl(std::u16string_view basePath, std::u16string_view searchPattern, uint32_t filter, bool recursive) noexcept;

public:
    bool Increment();
    bool Increment(std::string_view directory);
    std::u16string_view GetBuffer() const;

private:
    UniqueDirHandle handle_ = {nullptr, &closedir};
    struct dirent* dirEntry_ = nullptr;
    std::string basePath_;
    std::string searchPattern_;
    uint32_t filter_;
    bool recursive_;
    std::deque<std::string> directories_;
    mutable std::u16string buffer_;
};

IteratorImpl::IteratorImpl(std::u16string_view basePath, std::u16string_view searchPattern, uint32_t filter, bool recursive) noexcept :
    basePath_(Utf16ToUtf8(basePath)),
    searchPattern_(Utf16ToUtf8(searchPattern)),
    filter_(filter),
    recursive_(recursive),
    directories_(1, basePath_)
{
}

bool IteratorImpl::Increment()
{
    while (!directories_.empty())
    {
        if (Increment(directories_.front()))
        {
            return true;
        }

        handle_.reset();
        directories_.pop_front();
    }

    return false;
}

bool IteratorImpl::Increment(std::string_view dir)
{
    while (TryFindNextFile(dir, &handle_, &dirEntry_))
    {
#if CS2CPP_PLATFORM_DARWIN
        std::string_view fileName(dirEntry_->d_name, dirEntry_->d_namlen);
#else
        std::string_view fileName(dirEntry_->d_name);
#endif
        if (IsDotDirectory(fileName))
        {
            continue;
        }

        if (recursive_ && dirEntry_->d_type & DT_DIR)
        {
            directories_.push_back(GetFullPath(dir, dirEntry_->d_name));
        }

        if (!(dirEntry_->d_type & filter_))
        {
            continue;
        }

        if (!IsPatternMatched(fileName, searchPattern_))
        {
            continue;
        }

        buffer_.clear();
        basePath_ = dir;
        return true;
    }

    return false;
}

std::u16string_view IteratorImpl::GetBuffer() const
{
    if (!buffer_.empty())
    {
        return buffer_;
    }

    return buffer_ = Utf8ToUtf16(GetFullPath(basePath_, dirEntry_->d_name));
}

DirectoryIterator::DirectoryIterator(std::u16string_view basePath, std::u16string_view searchPattern, uint32_t filter, SearchOption searchOption) :
    impl_(std::make_shared<IteratorImpl>(basePath, searchPattern, filter, searchOption == SearchOption::AllDirectories))
{
    impl_->Increment();
}

std::u16string_view DirectoryIterator::operator*() noexcept
{
    return impl_->GetBuffer();
}

void DirectoryIterator::Increment()
{
    if (!impl_->Increment())
    {
        // Null impl_ means the end iterator.
        impl_.reset();
    }
}

CS2CPP_NAMESPACE_END