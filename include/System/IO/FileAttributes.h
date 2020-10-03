#pragma once

CS2CPP_NAMESPACE_BEGIN

enum class FileAttributes
{
    Archive = 0x20,
    Compressed = 0x800,
    Device = 0x40,
    Directory = 0x10,
    Encrypted = 0x4000,
    Hidden = 0x2,
    IntegrityStream = 0x8000,
    Normal = 0x80,
    NoScrubData = 0x20000,
    NotContentIndexed = 0x2000,
    Offline = 0x1000,
    ReadOnly = 0x1,
    ReparsePoint = 0x400,
    SparseFile = 0x200,
    System = 0x4,
    Temporary = 0x100
};

CS2CPP_NAMESPACE_END