#pragma once

#include <algorithm>
#include <memory>

CS2CPP_NAMESPACE_BEGIN

class SafeFilePointer final
{
public:
    explicit SafeFilePointer(FILE* filePointer) noexcept;

public:
    bool operator==(const SafeFilePointer& rhs) const noexcept;
    bool operator!=(const SafeFilePointer& rhs) const noexcept;
    bool operator==(std::nullptr_t rhs) const noexcept;
    bool operator!=(std::nullptr_t rhs) const noexcept;
    operator FILE*() const noexcept;

private:
    std::unique_ptr<FILE, decltype(&fclose)> _filePointer;
};

inline SafeFilePointer::SafeFilePointer(FILE* filePointer) noexcept :
    _filePointer(filePointer, fclose)
{
}

inline bool SafeFilePointer::operator==(const SafeFilePointer& rhs) const noexcept
{
    return _filePointer == rhs._filePointer;
}

inline bool SafeFilePointer::operator!=(const SafeFilePointer& rhs) const noexcept
{
    return _filePointer != rhs._filePointer;
}

inline bool SafeFilePointer::operator==(std::nullptr_t) const noexcept
{
    return _filePointer == nullptr;
}

inline bool SafeFilePointer::operator!=(std::nullptr_t) const noexcept
{
    return _filePointer != nullptr;
}

inline SafeFilePointer::operator FILE*() const noexcept
{
    return _filePointer.get();
}

CS2CPP_NAMESPACE_END