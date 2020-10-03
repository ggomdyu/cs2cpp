#include <array>

#include "System.Diagnostics/Debug.h"
#include "System.Text/Encoding.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

void Debug::Write(std::u16string_view message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    constexpr size_t WideCharBufferSize = 4096;

    [[maybe_unused]] std::lock_guard lock(mutex_);
    WriteIndent();

    std::array<wchar_t, WideCharBufferSize + 1> wideCharBuffer{};
    while (!message.empty())
    {
        auto bufferCopySize = std::min(WideCharBufferSize, message.size());

        memcpy(wideCharBuffer.data(), message.data(), bufferCopySize * sizeof(wchar_t));
        wideCharBuffer[bufferCopySize] = L'\0';
        OutputDebugStringW(reinterpret_cast<LPCWSTR>(wideCharBuffer.data()));

        message = message.substr(bufferCopySize);
    }
#endif
}

void Debug::WriteIndent()
{
    for (int32_t i = 0; i < indentLevel_; ++i)
    {
        OutputDebugStringW(L"\t");
    }
}

CS2CPP_NAMESPACE_END
