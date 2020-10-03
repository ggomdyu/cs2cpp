#pragma once

CS2CPP_NAMESPACE_BEGIN

enum class FileAccess
{
    Read = 1,
    Write = 2,
    ReadWrite = Read | Write
};

CS2CPP_NAMESPACE_END
