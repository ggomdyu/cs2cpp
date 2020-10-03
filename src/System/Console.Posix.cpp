#include "System/Console.h"
#include "System.IO/ConsoleStream.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail
{

FILE* GetInputHandle()
{
    return stdin;
}

FILE* GetOutputHandle()
{
    return stdout;
}

FILE* GetErrorHandle()
{
    return stderr;
}

std::shared_ptr<Stream> GetStandardFile(FILE* handle, FileAccess access, bool useFileAPIs)
{
    if (!handle)
    {
        return nullptr;
    }

    return std::make_shared<ConsoleStream>(handle, access, useFileAPIs);
}

}

std::shared_ptr<Stream> Console::OpenStandardInput()
{
    return detail::GetStandardFile(detail::GetInputHandle(), FileAccess::Read, true);
}

std::shared_ptr<Stream> Console::OpenStandardOutput()
{
    return detail::GetStandardFile(detail::GetOutputHandle(), FileAccess::Write, true);
}

std::shared_ptr<Stream> Console::OpenStandardError()
{
    return detail::GetStandardFile(detail::GetErrorHandle(), FileAccess::Write, true);
}

bool Console::KeyAvailable()
{
    return false;
}

ConsoleKeyInfo Console::ReadKey(bool intercept)
{
    return {};
}

bool Console::NumberLock()
{
    return false;
}

bool Console::CapsLock()
{
    return false;
}

void Console::ResetColor()
{
}

bool Console::SetCursorSize(int32_t cursorSize)
{
    return false;
}

bool Console::SetCursorPosition(int32_t left, int32_t top)
{
    return false;
}

bool Console::SetTreatControlCAsInput(bool value)
{
    return false;
}

bool Console::SetBackgroundColor(ConsoleColor color)
{
    return false;
}

bool Console::SetForegroundColor(ConsoleColor color)
{
    return false;
}

bool Console::SetBufferWidth(int32_t width)
{
    return false;
}

bool Console::SetBufferHeight(int32_t height)
{
    return false;
}

bool Console::SetBufferSize(int32_t width, int32_t height)
{
    return false;
}

bool Console::SetWindowPosition(int32_t left, int32_t top)
{
    return false;
}

bool Console::SetWindowSize(int32_t width, int32_t height)
{
    return false;
}

bool Console::SetCursorVisible(bool visible)
{
    return false;
}

bool Console::SetTitle(std::u16string_view title)
{
    return false;
}

bool Console::SetCursorLeft(int32_t left)
{
    return false;
}

bool Console::SetCursorTop(int32_t top)
{
    return false;
}

int32_t Console::GetCursorSize()
{
    return 0;
}

bool Console::GetTreatControlCAsInput()
{
    return false;
}

ConsoleColor Console::GetBackgroundColor()
{
    return ConsoleColor::Black;
}

ConsoleColor Console::GetForegroundColor()
{
    return ConsoleColor::Gray;
}

int32_t Console::GetBufferWidth()
{
    return 0;
}

int32_t Console::GetBufferHeight()
{
    return 0;
}

int32_t Console::GetWindowLeft()
{
    return 0;
}

int32_t Console::GetWindowTop()
{
    return 0;
}

int32_t Console::GetWindowWidth()
{
    return 0;
}

int32_t Console::GetWindowHeight()
{
    return 0;
}

bool Console::GetCursorVisible()
{
    return false;
}

std::u16string Console::GetTitle()
{
    return {};
}

int32_t Console::GetCursorLeft()
{
    return 0;
}

int32_t Console::GetCursorTop()
{
    return 0;
}

int32_t Console::LargestWindowWidth()
{
    return 0;
}

int32_t Console::LargestWindowHeight()
{
    return 0;
}

bool Console::IsInputRedirected()
{
    return false;
}

bool Console::IsOutputRedirected()
{
    return false;
}

bool Console::IsErrorRedirected()
{
    return false;
}

void Console::Beep(int32_t frequency, int32_t duration)
{
}

bool Console::Clear()
{
    return false;
}

int32_t Console::GetDefaultConsoleInputCP()
{
    return 65001;
}

int32_t Console::GetDefaultConsoleOutputCP()
{
    return 65001;
}

CS2CPP_NAMESPACE_END
