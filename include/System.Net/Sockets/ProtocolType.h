#pragma once

CS2CPP_NAMESPACE_BEGIN

enum class ProtocolType
{
    Unknown = -1,
    IP = 0,
    IPv6HopByHopOptions = 0,
    Icmp = 1,
    Igmp = 2,
    Ggp = 3,
    IPv4 = 4,
    Tcp = 6,
    Pup = 12,
    Udp = 17,
    Idp = 22,
    IPv6 = 41,
    IPv6RoutingHeader = 43,
    IPv6FragmentHeader = 44,
    IPSecEncapsulatingSecurityPayload = 50,
    IPSecAuthenticationHeader = 51,
    IcmpV6 = 58,
    IPv6NoNextHeader = 59,
    IPv6DestinationOptions = 60,
    ND = 77,
    Raw = 0xFF,
    Unspecified = 0,
    Ipx = 1000,
    Spx = 1256,
    SpxII = 1257,
};

CS2CPP_NAMESPACE_END