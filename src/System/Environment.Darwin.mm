#include <Foundation/Foundation.h>

#include "System/Environment.h"

CS2CPP_NAMESPACE_BEGIN

//std::vector<std::u16string> Environment::InternalGetCommandLineArgs()
//{
//    std::vector<std::u16string> ret;
//
//    auto arguments = [[NSProcessInfo processInfo] arguments];
//    for (NSUInteger i = 0; i < [arguments count]; ++i)
//    {
//        ret.emplace_back(reinterpret_cast<const char16_t*>([arguments[i] cStringUsingEncoding:NSUTF16LittleEndianStringEncoding]));
//    }
//
//    return ret;
//}

CS2CPP_NAMESPACE_END