#include <array>

#include "System/Console.h"
#include "System.IO/ConsoleStream.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

static HANDLE GetInputHandle()
{
    return GetStdHandle(STD_INPUT_HANDLE);
}

static HANDLE GetOutputHandle()
{
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

static HANDLE GetErrorHandle()
{
    return GetStdHandle(STD_ERROR_HANDLE);
}

static std::shared_ptr<Stream> GetStandardFile(HANDLE handle, FileAccess access, bool useFileAPIs)
{
    if (!handle || handle == INVALID_HANDLE_VALUE)
    {
        return nullptr;
    }

    return std::make_shared<ConsoleStream>(handle, access, useFileAPIs);
}

static bool IsHandleRedirected(HANDLE handle)
{
    auto fileType = GetFileType(handle);
    if ((fileType & FILE_TYPE_CHAR) != FILE_TYPE_CHAR)
    {
        return true;
    }

    DWORD mode;
    return !!GetConsoleMode(handle, &mode);
}

static std::optional<WORD> DefaultColors;

static std::optional<CONSOLE_SCREEN_BUFFER_INFO> GetConsoleScreenBufferInfo(HANDLE handle)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(handle, &info) == FALSE)
    {
        return std::nullopt;
    }

    if (!DefaultColors)
    {
        DefaultColors = info.wAttributes & 255;
    }

    return info;
}

static bool IsKeyDownEvent(const INPUT_RECORD& ir) noexcept
{
    return ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown == TRUE;
}

static bool IsModKey(const INPUT_RECORD& ir) noexcept
{
    auto keyCode = ir.Event.KeyEvent.wVirtualKeyCode;
    return (keyCode >= VK_SHIFT && keyCode <= VK_MENU) || keyCode == VK_CAPITAL || keyCode == VK_NUMLOCK || keyCode == VK_SCROLL;
}

static bool IsAltKeyDown(const INPUT_RECORD& ir) noexcept
{
    return ir.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED);
}

std::shared_ptr<Stream> Console::OpenStandardInput()
{
    bool useFileAPIs = GetInputEncoding()->CodePage() != Encoding::Unicode().CodePage() || IsInputRedirected();
    return GetStandardFile(GetInputHandle(), FileAccess::Read, useFileAPIs);
}

std::shared_ptr<Stream> Console::OpenStandardOutput()
{
    bool useFileAPIs = GetOutputEncoding()->CodePage() != Encoding::Unicode().CodePage() || IsOutputRedirected();
    return GetStandardFile(GetOutputHandle(), FileAccess::Write, useFileAPIs);
}

std::shared_ptr<Stream> Console::OpenStandardError()
{
    bool useFileAPIs = GetOutputEncoding()->CodePage() != Encoding::Unicode().CodePage() || IsErrorRedirected();
    return GetStandardFile(GetErrorHandle(), FileAccess::Write, useFileAPIs);
}

bool Console::KeyAvailable()
{
    auto handle = GetInputHandle();
    INPUT_RECORD ir;
    DWORD numEventsRead;
   
    while (true)
    {
        if (PeekConsoleInput(handle, &ir, 1, &numEventsRead) == FALSE || numEventsRead == 0)
        {
            return false;
        }

        if (!IsKeyDownEvent(ir) || IsModKey(ir))
        {
            if (ReadConsoleInput(handle, &ir, 1, &numEventsRead) == FALSE)
            {
                return false;
            }
        }
        else
        {
            return true;
        }
    }
}

ConsoleKeyInfo Console::ReadKey(bool intercept)
{
    auto handle = GetInputHandle();
    INPUT_RECORD ir;
    DWORD numEventsRead;

    while (true)
    {
        if (ReadConsoleInputW(handle, &ir, 1, &numEventsRead) == FALSE)
        {
            return {};
        }

        auto keyCode = ir.Event.KeyEvent.wVirtualKeyCode;
        if (!IsKeyDownEvent(ir))
        {
            if (keyCode != VK_MENU)
            {
                continue;
            }
        }

        if (ir.Event.KeyEvent.uChar.UnicodeChar == 0)
        {
            if (IsModKey(ir))
            {
                continue;
            }
        }
        
        if (IsAltKeyDown(ir) && ((keyCode >= VK_NUMPAD0 && keyCode <= VK_NUMPAD9) || (keyCode == VK_CLEAR) || (keyCode == VK_INSERT) || (keyCode >= VK_PRIOR && keyCode <= VK_DOWN)))
        {
            continue;
        }

        break;
    }

    auto key = static_cast<char16_t>(ir.Event.KeyEvent.uChar.UnicodeChar);

    if (!intercept)
    {
        Write(key);
    }

    auto state = ir.Event.KeyEvent.dwControlKeyState;

    return ConsoleKeyInfo(key,
        static_cast<ConsoleKey>(ir.Event.KeyEvent.wVirtualKeyCode),
        state & SHIFT_PRESSED,
        state & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED),
        state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)
    );
}

bool Console::NumberLock()
{
    return GetKeyState(VK_NUMLOCK);
}

bool Console::CapsLock()
{
    return GetKeyState(VK_CAPITAL);
}

void Console::ResetColor()
{
    if (!DefaultColors)
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(GetOutputHandle(), &info) == FALSE)
        {
            return;
        }

        DefaultColors = info.wAttributes & 255;
    }

    SetConsoleTextAttribute(GetOutputHandle(), *DefaultColors);
}

bool Console::SetCursorSize(int32_t cursorSize)
{
    auto handle = GetOutputHandle();

    CONSOLE_CURSOR_INFO cci;
    if (GetConsoleCursorInfo(handle, &cci) == FALSE)
    {
        return false;
    }

    cci.dwSize = cursorSize;

    return !!SetConsoleCursorInfo(handle, &cci);
}

bool Console::SetCursorPosition(int32_t left, int32_t top)
{
    COORD coord{static_cast<SHORT>(left), static_cast<SHORT>(top)};
    return !!SetConsoleCursorPosition(GetOutputHandle(), coord);
}

bool Console::SetTreatControlCAsInput(bool value)
{
    auto handle = GetInputHandle();
    
    DWORD mode = 0;
    if (GetConsoleMode(handle, &mode) == FALSE)
    {
        return false;
    }

    if (value)
    {
        mode &= ~ENABLE_PROCESSED_INPUT;
    }
    else
    {
        mode |= ENABLE_PROCESSED_INPUT;
    }

    return !!SetConsoleMode(handle, mode);
}

bool Console::SetBackgroundColor(ConsoleColor color)
{
    auto handle = GetOutputHandle();
    auto info = GetConsoleScreenBufferInfo(handle);
    if (!info)
    {
        return false;
    }

    auto attrs = info->wAttributes;
    attrs &= ~(static_cast<WORD>(ConsoleColor::White) << 4);
    attrs |= (static_cast<WORD>(color) << 4);

    return !!SetConsoleTextAttribute(handle, attrs);
}

bool Console::SetForegroundColor(ConsoleColor color)
{
    auto handle = GetOutputHandle();

    auto info = GetConsoleScreenBufferInfo(handle);
    if (!info)
    {
        return false;
    }

    auto attrs = info->wAttributes;
    attrs &= ~static_cast<WORD>(ConsoleColor::White);
    attrs |= static_cast<WORD>(color);

    return !!SetConsoleTextAttribute(handle, attrs);
}

bool Console::SetBufferWidth(int32_t width)
{
    return SetBufferSize(width, GetBufferHeight());
}

bool Console::SetBufferHeight(int32_t height)
{
    return SetBufferSize(GetBufferWidth(), height);
}

bool Console::SetBufferSize(int32_t width, int32_t height)
{
    COORD size{static_cast<SHORT>(width), static_cast<SHORT>(height)};
    return !!SetConsoleScreenBufferSize(GetOutputHandle(), size);
}

bool Console::SetWindowPosition(int32_t left, int32_t top)
{
    auto handle = GetOutputHandle();
    auto info = GetConsoleScreenBufferInfo(handle);
    if (!info)
    {
        return false;
    }

    auto srWindow = info->srWindow;
    srWindow.Bottom -= srWindow.Top - top;
    srWindow.Right -= srWindow.Left - left;
    srWindow.Left = left;
    srWindow.Top = top;

    return !!SetConsoleWindowInfo(handle, true, &srWindow);
}

bool Console::SetWindowSize(int32_t width, int32_t height)
{
    auto handle = GetOutputHandle();
    auto info = GetConsoleScreenBufferInfo(handle);
    if (!info)
    {
        return false;
    }

    SMALL_RECT srWindow = info->srWindow;
    srWindow.Bottom = srWindow.Top + height - 1;
    srWindow.Right = srWindow.Left + width - 1;

    return !!SetConsoleWindowInfo(handle, TRUE, &srWindow);
}

bool Console::SetCursorVisible(bool visible)
{
    auto handle = GetOutputHandle();

    CONSOLE_CURSOR_INFO cci;
    if (GetConsoleCursorInfo(handle, &cci) == FALSE)
    {
        return false;
    }

    cci.bVisible = visible;

    return !!SetConsoleCursorInfo(handle, &cci);
}

bool Console::SetTitle(std::u16string_view title)
{
    std::wstring wideCharTitle(reinterpret_cast<const wchar_t*>(title.data()), title.length());
    return !!SetConsoleTitleW(wideCharTitle.c_str());
}

bool Console::SetCursorLeft(int32_t left)
{
    return SetCursorPosition(left, GetCursorTop());
}

bool Console::SetCursorTop(int32_t top)
{
    return SetCursorPosition(GetCursorLeft(), top);
}

int32_t Console::GetCursorSize()
{
    CONSOLE_CURSOR_INFO cci;
    if (GetConsoleCursorInfo(GetOutputHandle(), &cci) == FALSE)
    {
        return 0;
    }

    return static_cast<int32_t>(cci.dwSize);
}

bool Console::GetTreatControlCAsInput()
{
    DWORD mode = 0;
    if (GetConsoleMode(GetInputHandle(), &mode) == FALSE)
    {
        return false;
    }

    return (mode & ENABLE_PROCESSED_INPUT) == 0;
}

ConsoleColor Console::GetBackgroundColor()
{
    auto info = GetConsoleScreenBufferInfo(GetOutputHandle());
    if (!info)
    {
        return ConsoleColor::Black;
    }

    return static_cast<ConsoleColor>((info->wAttributes & (static_cast<WORD>(ConsoleColor::White) << 4)) >> 4);
}

ConsoleColor Console::GetForegroundColor()
{
    auto info = GetConsoleScreenBufferInfo(GetOutputHandle());
    if (!info)
    {
        return ConsoleColor::Gray;
    }

    return static_cast<ConsoleColor>(info->wAttributes & static_cast<WORD>(ConsoleColor::White));
}

int32_t Console::GetBufferWidth()
{
    auto info = GetConsoleScreenBufferInfo(GetOutputHandle());
    if (!info)
    {
        return 0;
    }

    return info->dwSize.X;
}

int32_t Console::GetBufferHeight()
{
    auto info = GetConsoleScreenBufferInfo(GetOutputHandle());
    if (!info)
    {
        return 0;
    }

    return info->dwSize.Y;
}

int32_t Console::GetWindowLeft()
{
    auto info = GetConsoleScreenBufferInfo(GetOutputHandle());
    if (!info)
    {
        return 0;
    }

    return info->srWindow.Left;
}

int32_t Console::GetWindowTop()
{
    auto info = GetConsoleScreenBufferInfo(GetOutputHandle());
    if (!info)
    {
        return 0;
    }

    return info->srWindow.Top;
}

int32_t Console::GetWindowWidth()
{
    auto info = GetConsoleScreenBufferInfo(GetOutputHandle());
    if (!info)
    {
        return 0;
    }

    return info->srWindow.Right - info->srWindow.Left + 1;
}

int32_t Console::GetWindowHeight()
{
    auto info = GetConsoleScreenBufferInfo(GetOutputHandle());
    if (!info)
    {
        return 0;
    }

    return info->srWindow.Bottom - info->srWindow.Top + 1;
}

bool Console::GetCursorVisible()
{
    CONSOLE_CURSOR_INFO info;
    if (GetConsoleCursorInfo(GetOutputHandle(), &info) == FALSE)
    {
        return false;
    }

    return info.bVisible;
}

std::u16string Console::GetTitle()
{
    std::array<wchar_t, 2048> buffer{};
    size_t length = GetConsoleTitleW(buffer.data(), buffer.size());

    return std::u16string(reinterpret_cast<const char16_t*>(buffer.data()), length);
}

int32_t Console::GetCursorLeft()
{
    auto info = GetConsoleScreenBufferInfo(GetOutputHandle());
    if (!info)
    {
        return 0;
    }

    return info->dwCursorPosition.X;
}

int32_t Console::GetCursorTop()
{
    auto info = GetConsoleScreenBufferInfo(GetOutputHandle());
    if (!info)
    {
        return 0;
    }

    return info->dwCursorPosition.Y;
}

int32_t Console::LargestWindowWidth()
{
    return static_cast<int32_t>(GetLargestConsoleWindowSize(GetOutputHandle()).X);
}

int32_t Console::LargestWindowHeight()
{
    return static_cast<int32_t>(GetLargestConsoleWindowSize(GetOutputHandle()).Y);
}

bool Console::IsInputRedirected()
{
    return IsHandleRedirected(GetInputHandle());
}

bool Console::IsOutputRedirected()
{
    return IsHandleRedirected(GetOutputHandle());
}

bool Console::IsErrorRedirected()
{
    return IsHandleRedirected(GetErrorHandle());
}

void Console::Beep(int32_t frequency, int32_t duration)
{
    ::Beep(frequency, duration);
}

bool Console::Clear()
{
    auto handle = GetOutputHandle();
    auto info = GetConsoleScreenBufferInfo(handle);
    if (!info)
    {
        return false;
    }

    auto length = info->dwSize.X * info->dwSize.Y;
    COORD coord{};
    DWORD writtenCharNum;
    if (FillConsoleOutputCharacterW(handle, L' ', length, coord, &writtenCharNum) == FALSE)
    {
        return false;
    }

    writtenCharNum = 0;
    if (FillConsoleOutputAttribute(handle, info->wAttributes, length, coord, &writtenCharNum) == FALSE)
    {
        return false;
    }

    return !!SetConsoleCursorPosition(handle, coord);
}

int32_t Console::GetDefaultConsoleInputCP()
{
    return static_cast<int32_t>(GetConsoleCP());
}

int32_t Console::GetDefaultConsoleOutputCP()
{
    return static_cast<int32_t>(GetConsoleOutputCP());
}

CS2CPP_NAMESPACE_END
