#pragma once

#include <string>

CS2CPP_NAMESPACE_BEGIN

class TempFile final
{
public:
    explicit TempFile(std::u16string path);
    TempFile();
    ~TempFile();

public:
    const std::u16string& GetPath() const;

private:
    std::u16string path_;
};

CS2CPP_NAMESPACE_END