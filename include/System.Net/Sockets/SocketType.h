#pragma once

CS2CPP_NAMESPACE_BEGIN

enum class SocketType
{
    Unknown = -1,
    Stream = 1,
    Dgram = 2,
    Raw = 3,
    Rdm = 4,
    Seqpacket = 5,
};

CS2CPP_NAMESPACE_END