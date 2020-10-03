#include <AppKit/NSAlert.h>

#include "System/Diagnostics/Debug.h"

CS2CPP_NAMESPACE_BEGIN

void Debug::Write(std::u16string_view message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    std::lock_guard lockGuard(_mutex);
    WriteIndent();

    auto characters = reinterpret_cast<const unichar*>(message.data());
    auto utf8Message = [NSString stringWithCharacters:characters length:message.length()].UTF8String;

    printf("%s", utf8Message);
#endif
}

void Debug::WriteLine(std::u16string_view message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    std::lock_guard lockGuard(_mutex);
    Write(message);

    printf("\n");
#endif
}

CS2CPP_NAMESPACE_END
