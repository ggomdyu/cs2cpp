#pragma once

CS2CPP_NAMESPACE_BEGIN

enum class FileOptions
{
    WriteThrough = std::numeric_limits<int>::min(),
    None = 0,
    Encrypted = 16384,
    DeleteOnClose = 67108864,
    SequentialScan = 134217728,
    RandomAccess = 268435456,
    Asynchronous = 1073741824
};

CS2CPP_NAMESPACE_END
