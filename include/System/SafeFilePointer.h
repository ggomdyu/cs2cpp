#pragma once

#include <algorithm>
#include <cstdio>

CS2CPP_NAMESPACE_BEGIN

class SafeFilePointer final
{
public:
    constexpr SafeFilePointer() noexcept = default;
    constexpr explicit SafeFilePointer(FILE* filePointer) noexcept;
    SafeFilePointer(const SafeFilePointer& rhs) = delete;
    SafeFilePointer(SafeFilePointer&& rhs) noexcept;

public:
    ~SafeFilePointer();

public:
    SafeFilePointer& operator=(const SafeFilePointer& rhs) = delete;
    SafeFilePointer& operator=(SafeFilePointer&& rhs) noexcept;
    [[nodiscard]] constexpr bool operator==(const SafeFilePointer& rhs) const noexcept;
    [[nodiscard]] constexpr bool operator!=(const SafeFilePointer& rhs) const noexcept;
    [[nodiscard]] constexpr bool operator==(std::nullptr_t rhs) const noexcept;
    [[nodiscard]] constexpr bool operator!=(std::nullptr_t rhs) const noexcept;
    [[nodiscard]] operator FILE*() const noexcept;
    [[nodiscard]] operator FILE* *() noexcept;

private:
    FILE* _filePointer = nullptr;
};

constexpr SafeFilePointer::SafeFilePointer(FILE* filePointer) noexcept :
    _filePointer(filePointer)
{
}

inline SafeFilePointer::SafeFilePointer(SafeFilePointer&& rhs) noexcept :
    _filePointer(rhs._filePointer)
{
    rhs._filePointer = nullptr;
}

inline SafeFilePointer::~SafeFilePointer()
{
    if (_filePointer != nullptr)
    {
        fclose(_filePointer);
    }
}

inline SafeFilePointer& SafeFilePointer::operator=(SafeFilePointer&& rhs) noexcept
{
    std::swap(_filePointer, rhs._filePointer);
    return *this;
}

constexpr bool SafeFilePointer::operator==(const SafeFilePointer& rhs) const noexcept
{
    return _filePointer == rhs._filePointer;
}

constexpr bool SafeFilePointer::operator!=(const SafeFilePointer& rhs) const noexcept
{
    return _filePointer != rhs._filePointer;
}

constexpr bool SafeFilePointer::operator==(std::nullptr_t) const noexcept
{
    return _filePointer == nullptr;
}

constexpr bool SafeFilePointer::operator!=(std::nullptr_t) const noexcept
{
    return _filePointer != nullptr;
}

inline SafeFilePointer::operator FILE*() const noexcept
{
    return _filePointer;
}

inline SafeFilePointer::operator FILE* *() noexcept
{
    return &_filePointer;
}

CS2CPP_NAMESPACE_END