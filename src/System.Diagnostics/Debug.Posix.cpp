#include "System.Diagnostics/Debug.h"
#include "System.Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

void Debug::Write(std::u16string_view message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    [[maybe_unused]] std::lock_guard lock(mutex_);
    WriteIndent();

    auto utf8Message = Encoding::UTF8().GetBytes(message);
    printf("%s", reinterpret_cast<const char*>(utf8Message->data()));
#endif
}

void Debug::WriteIndent()
{
    for (int32_t i = 0; i < indentLevel_; ++i)
    {
        printf("\t");
    }
}

CS2CPP_NAMESPACE_END
