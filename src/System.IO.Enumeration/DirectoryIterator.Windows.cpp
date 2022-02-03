#include <Shlwapi.h>
#include <deque>
#include <optional>

#include "System.IO.Enumeration/DirectoryIterator.h"
#include "System.IO/Path.h"

#pragma comment(lib, "Shlwapi.lib")

CS2CPP_NAMESPACE_BEGIN

using UniqueFindFileHandle = std::unique_ptr<void, decltype(&FindClose)>;

static bool IsPatternMatched(std::wstring_view fileName, std::u16string_view searchPattern)
{
    if (searchPattern == u"*")
    {
        return true;
    }

    return PathMatchSpecW(fileName.data(), reinterpret_cast<const wchar_t*>(searchPattern.data())) == TRUE;
}

static bool IsDotDirectory(std::wstring_view path) noexcept
{
    return path[0] == L'.' && (path[1] == L'\0' || (path[1] == L'.' && path[2] == L'\0'));
}

static bool TryFindNextFile(std::u16string_view path, UniqueFindFileHandle* handle, WIN32_FIND_DATAW* findData)
{
    if (handle->get() == INVALID_HANDLE_VALUE)
    {
        std::u16string wildcardPath = Path::Combine(path, u"*");
        return (*handle = {FindFirstFileW(reinterpret_cast<LPCWSTR>(wildcardPath.data()), findData), &FindClose}).get() != INVALID_HANDLE_VALUE;
    }

    return FindNextFileW(handle->get(), findData);
}

class IteratorImpl final
{
public:
    IteratorImpl(std::u16string_view basePath, std::u16string_view searchPattern, uint32_t filter, bool recursive) noexcept;

public:
    bool Increment();
    bool Increment(std::u16string_view dir);
    std::u16string_view GetBuffer() const;

private:
    UniqueFindFileHandle handle_ = {INVALID_HANDLE_VALUE, &FindClose};
    WIN32_FIND_DATAW findData_ = {};
    std::u16string basePath_;
    std::u16string searchPattern_;
    uint32_t filter_;
    bool recursive_;
    std::deque<std::u16string> directories_;
    mutable std::u16string buffer_;
};

IteratorImpl::IteratorImpl(std::u16string_view basePath, std::u16string_view searchPattern, uint32_t filter, bool recursive) noexcept :
    basePath_(basePath),
    searchPattern_(searchPattern),
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

        handle_.reset(INVALID_HANDLE_VALUE);
        directories_.pop_front();
    }

    return false;
}

bool IteratorImpl::Increment(std::u16string_view dir)
{
    while (TryFindNextFile(dir, &handle_, &findData_))
    {
        if (IsDotDirectory(findData_.cFileName))
        {
            continue;
        }

        if (recursive_ && findData_.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            directories_.push_back(Path::Combine(dir, reinterpret_cast<const char16_t*>(findData_.cFileName)));
        }

        if (!(findData_.dwFileAttributes & filter_))
        {
            continue;
        }

        if (!IsPatternMatched(findData_.cFileName, searchPattern_))
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

    return buffer_ = Path::Combine(basePath_, reinterpret_cast<const char16_t*>(findData_.cFileName));
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