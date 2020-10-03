#pragma once

#include <algorithm>
#include <memory>

CS2CPP_NAMESPACE_BEGIN

class SafeFilePointer final
{
public:
    SafeFilePointer() noexcept;
    SafeFilePointer(FILE* filePointer) noexcept;

public:
    bool operator==(const SafeFilePointer& rhs) const noexcept;
    bool operator!=(const SafeFilePointer& rhs) const noexcept;
    bool operator==(std::nullptr_t rhs) const noexcept;
    bool operator!=(std::nullptr_t rhs) const noexcept;
    operator FILE*() const noexcept;

private:
    std::unique_ptr<FILE, decltype(&fclose)> filePointer_;
};

inline SafeFilePointer::SafeFilePointer() noexcept :
    filePointer_(nullptr, fclose)
{
}

inline SafeFilePointer::SafeFilePointer(FILE* filePointer) noexcept :
    filePointer_(filePointer, fclose)
{
}

inline bool SafeFilePointer::operator==(const SafeFilePointer& rhs) const noexcept
{
    return filePointer_ == rhs.filePointer_;
}

inline bool SafeFilePointer::operator!=(const SafeFilePointer& rhs) const noexcept
{
    return filePointer_ != rhs.filePointer_;
}

inline bool SafeFilePointer::operator==(std::nullptr_t) const noexcept
{
    return filePointer_ == nullptr;
}

inline bool SafeFilePointer::operator!=(std::nullptr_t) const noexcept
{
    return filePointer_ != nullptr;
}

inline SafeFilePointer::operator FILE*() const noexcept
{
    return filePointer_.get();
}

CS2CPP_NAMESPACE_END