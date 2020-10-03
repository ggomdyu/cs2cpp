#pragma once

#if CS2CPP_PLATFORM_WINDOWS
#   include <io.h>
#else
#   include <unistd.h>
#endif

CS2CPP_NAMESPACE_BEGIN

class SafeFileDescriptor final
{
public:
    SafeFileDescriptor() noexcept = default;
    SafeFileDescriptor(int fd) noexcept;
    SafeFileDescriptor(const SafeFileDescriptor& rhs) = delete;
    SafeFileDescriptor(SafeFileDescriptor&& rhs) noexcept;
    ~SafeFileDescriptor();

public:
    SafeFileDescriptor& operator=(const SafeFileDescriptor& rhs) = delete;
    SafeFileDescriptor& operator=(SafeFileDescriptor&& rhs) noexcept;
    bool operator==(const SafeFileDescriptor& rhs) const noexcept;
    bool operator!=(const SafeFileDescriptor& rhs) const noexcept;
    bool operator==(std::nullptr_t rhs) const noexcept;
    bool operator!=(std::nullptr_t rhs) const noexcept;
    operator int() const noexcept;

private:
    int fd_ = -1;
};

inline SafeFileDescriptor::SafeFileDescriptor(int fd) noexcept :
    fd_(fd)
{
}

inline SafeFileDescriptor::~SafeFileDescriptor()
{
    if (fd_ != -1)
    {
#if CS2CPP_PLATFORM_WINDOWS
        _close(fd_);
#else
        close(fd_);
#endif
        fd_ = -1;
    }
}

inline SafeFileDescriptor::SafeFileDescriptor(SafeFileDescriptor&& rhs) noexcept :
    fd_(rhs.fd_)
{
    rhs.fd_ = -1;
}

inline SafeFileDescriptor& SafeFileDescriptor::operator=(SafeFileDescriptor&& rhs) noexcept
{
    fd_ = rhs.fd_;
    rhs.fd_ = -1;

    return *this;
}

inline bool SafeFileDescriptor::operator==(const SafeFileDescriptor& rhs) const noexcept
{
    return fd_ == rhs.fd_;
}

inline bool SafeFileDescriptor::operator!=(const SafeFileDescriptor& rhs) const noexcept
{
    return fd_ != rhs.fd_;
}

inline bool SafeFileDescriptor::operator==(std::nullptr_t) const noexcept
{
    return fd_ == -1;
}

inline bool SafeFileDescriptor::operator!=(std::nullptr_t) const noexcept
{
    return fd_ != -1;
}

inline SafeFileDescriptor::operator int() const noexcept
{
    return fd_;
}

CS2CPP_NAMESPACE_END