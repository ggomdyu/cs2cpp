#pragma once

#include <string>

CS2CPP_NAMESPACE_BEGIN

class TempDirectory final
{
public:
    explicit TempDirectory(std::u16string_view path);
    TempDirectory();
    ~TempDirectory();

public:
    std::u16string AddTempFile();
    std::u16string AddTempFile(std::u16string_view extension);
    std::u16string_view GetPath() const;

private:
    std::u16string path_;
};

CS2CPP_NAMESPACE_END