#pragma once

#include "ConsoleStream.h"

CS2CPP_NAMESPACE_BEGIN

class WindowsConsoleStream final :
    public ConsoleStream
{
public:
    WindowsConsoleStream(void* handle, FileAccess access, bool useFileAPIs);

public:
    int32_t Read(std::byte* bytes, int32_t count) override;
    int32_t ReadByte() override;
    bool Write(const std::byte* bytes, int32_t count) override;
    bool WriteByte(std::byte value) override;

private:
    void* _handle = nullptr;
    bool _isPipe = false;
    bool _useFileAPIs = false;
};

CS2CPP_NAMESPACE_END