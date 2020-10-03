#include "System/Diagnostics/Debug.h"
#include "System/Text/Encoding.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

void Debug::Write(std::u16string_view message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    std::lock_guard lockGuard(_mutex);
    WriteIndent();

    constexpr size_t WideCharBufferSize = 4096;
    std::array<wchar_t, WideCharBufferSize + 1> wideCharBuffer{};
    while (message.size() > 0)
    {
        const auto bufferCopySize = std::min(WideCharBufferSize, message.size());

        memcpy(wideCharBuffer.data(), message.data(), bufferCopySize * 2);
        wideCharBuffer[bufferCopySize] = L'\0';
        OutputDebugStringW(reinterpret_cast<const wchar_t*>(wideCharBuffer.data()));

        message = message.substr(bufferCopySize);
    }
#endif
}

void Debug::WriteLine(std::u16string_view message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    std::lock_guard lockGuard(_mutex);
    Write(message);

    OutputDebugStringW(L"\n");
#endif
}

void Debug::Assert(bool condition, std::u16string_view message, std::u16string_view detailMessage)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    if (condition)
    {
        return;
    }

    Fail(message, detailMessage);
#endif
}

void Debug::WriteIndent()
{
    for (int32_t i = 0; i < _indentLevel; ++i)
    {
        OutputDebugStringW(L"\t");
    }
}

CS2CPP_NAMESPACE_END