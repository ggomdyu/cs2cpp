# cs2cpp
[![Windows](https://github.com/ggomdyu/cs2cpp/actions/workflows/Windows.yml/badge.svg)](https://github.com/ggomdyu/cs2cpp/actions/workflows/Windows.yml)
[![MacOS](https://github.com/ggomdyu/cs2cpp/actions/workflows/MacOS.yml/badge.svg)](https://github.com/ggomdyu/cs2cpp/actions/workflows/MacOS.yml)

A C++17 based library that provides C# style classes

## What is this library
If you are familiar with C#, you might know C# utility classes are powerful and convenient for development. This library enables C# users to code C++ easily by providing C# like classes.<br></br>

## Requirements
*   [vcpkg](https://github.com/microsoft/vcpkg)
*   [fmt](https://github.com/fmtlib/fmt)
*   [ms-gsl](https://github.com/microsoft/GSL)
*   [icu](https://github.com/unicode-org/icu)
*   [stb](https://github.com/nothings/stb)
*   [gtest](https://github.com/google/googletest)

## Examples
**System.DateTime**
```cpp
#include <System/DateTime.h>

auto now = DateTime::Now();
auto timeStr = String::Format("{}/{}/{} {}:{}:{}", now.GetMonth(), now.GetDay(), now.GetYear(), now.GetHour(), now.GetMinute(), now.GetSecond())

Debug::WriteLine(timeStr);
```

**System.Text.Encoding**
```cpp
#include <System/Text/Encoding.h>

std::string_view utf8Str = u8"Hi 안녕하세요 こんにちは 你好";

auto utf16Str = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(utf8Str.data()), utf8Str.length() + 1);
if (utf16Str.has_value())
{
    Debug::WriteLine(*utf16Str);
}
```

**System.Environment**

```cpp
#include <System/Environment.h>

const auto stackTrace = Environment::GetStackTrace();
Debug::WriteLine(stackTrace);
```

**System.IO.Directory**
```cpp
#include <System/IO/Directory.h>

auto directories = Directory::GetDirectories(u"D:/Users/ggomdyu/Desktop", u"*", SearchOption::AllDirectories);
for (const auto& directory : directories)
{
    Debug::WriteLine(directory);
}
```
