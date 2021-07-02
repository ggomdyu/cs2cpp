#pragma once

#include <string>

namespace tg
{

class TempFile final
{
public:
    explicit TempFile(std::u16string path);
    TempFile();
    ~TempFile();

public:
    [[nodiscard]] const std::u16string& GetPath() const;

private:
    std::u16string _path;
};

}