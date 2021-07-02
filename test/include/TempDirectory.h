#pragma once

#include <string>

namespace tg
{

class TempDirectory final
{
public:
    explicit TempDirectory(std::u16string path);
    TempDirectory();
    ~TempDirectory();

public:
    std::u16string AddTempFile();
    std::u16string AddTempFile(std::u16string_view extension);
    [[nodiscard]] const std::u16string& GetPath() const;

private:
    std::u16string _path;
};

}