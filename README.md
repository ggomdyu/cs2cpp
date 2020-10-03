# cs2cpp
[![windows-latest](https://github.com/ggomdyu/cs2cpp/actions/workflows/windows-latest.yml/badge.svg)](https://github.com/ggomdyu/cs2cpp/actions/workflows/windows-latest.yml)
[![macos-latest](https://github.com/ggomdyu/cs2cpp/actions/workflows/macos-latest.yml/badge.svg)](https://github.com/ggomdyu/cs2cpp/actions/workflows/macos-latest.yml)
[![ubuntu-latest](https://github.com/ggomdyu/cs2cpp/actions/workflows/ubuntu-latest.yml/badge.svg)](https://github.com/ggomdyu/cs2cpp/actions/workflows/ubuntu-latest.yml)

A C++17 based library that provides C# style classes

## What is this library

If you are familiar with C#, you might know C# utility classes are powerful and convenient for development. This library enables C# users to code C++ easily by providing C# like classes.

## Requirements
*   [fmt](https://github.com/fmtlib/fmt)
*   [vcpkg](https://github.com/microsoft/vcpkg)
*   [icu](https://github.com/unicode-org/icu)
*   [gtest](https://github.com/google/googletest)

## Examples
**System.DateTime**
```cpp
#include <System/DateTime.h>

int main()
{
    DateTime dateTime(2021, 7, 1);
    Console::WriteLine(dateTime.Month());
    Console::WriteLine(dateTime.Day());
    Console::WriteLine(dateTime.Year());

    Console::WriteLine(DateTime::Now().ToString());
    Console::WriteLine(DateTime::UtcNow().ToString(u"MM-dd-yyyy hh:mm:ss"));
    
    return 0;
}
```

**System.Text.Encoding**
```cpp
#include <System.Text/Encoding.h>

int main()
{
    auto utf8Str = std::string_view(u8"Hi 你好 こんにちは");
    auto utf16Str = Encoding::UTF8().GetString(ReadOnlySpan(reinterpret_cast<const std::byte*>(utf8Str.data()), utf8Str.length()));
    if (utf16Str)
    {
        Console::WriteLine(*utf16Str);
    }
    
    return 0;
}

```

**System.Environment**

```cpp
#include <System/Environment.h>

int main()
{
    auto stackTrace = Environment::StackTrace();
    auto ticks = Environment::TickCount64();
    auto desktop = Environment::GetFolderPath(Environment::SpecialFolder::Desktop);

    Console::WriteLine(stackTrace);
    Console::WriteLine(ticks);
    Console::WriteLine(desktop);
       
    return 0;
}
```

**System.IO.Directory**
```cpp
#include <System.IO/Directory.h>

int main()
{ 
    Directory::Delete(u"C:/Temp", true);
    
    auto directories = Directory::GetDirectories(u"D:/Users", u"*.png", SearchOption::AllDirectories);
    for (const auto& directory : directories)
    {
        Debug::WriteLine(directory);
    }

    return 0;
}
```
