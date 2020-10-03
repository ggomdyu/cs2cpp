#pragma once

#include "System.IO/Stream.h"
#include "System.IO/TextReader.h"
#include "System.IO/TextWriter.h"
#include "System/ConsoleColor.h"
#include "System/ConsoleKeyInfo.h"

CS2CPP_NAMESPACE_BEGIN

class Console final
{
public:
    Console() = delete;

public:
    static std::shared_ptr<Stream> OpenStandardInput();
    static std::shared_ptr<Stream> OpenStandardOutput();
    static std::shared_ptr<Stream> OpenStandardError();
    static std::shared_ptr<TextReader> In();
    static std::shared_ptr<TextWriter> Out();
    static std::shared_ptr<TextWriter> Error();
    static void SetInputEncoding(std::shared_ptr<Encoding> encoding);
    static void SetOutputEncoding(std::shared_ptr<Encoding> encoding);
    static bool KeyAvailable();
    static ConsoleKeyInfo ReadKey(bool intercept = false);
    static bool NumberLock();
    static bool CapsLock();
    static void ResetColor();
    static bool SetCursorSize(int32_t cursorSize);
    static bool SetCursorPosition(int32_t left, int32_t top);
    static bool SetTreatControlCAsInput(bool value);
    static bool SetBackgroundColor(ConsoleColor color);
    static bool SetForegroundColor(ConsoleColor color);
    static bool SetBufferWidth(int32_t width);
    static bool SetBufferHeight(int32_t height);
    static bool SetBufferSize(int32_t width, int32_t height);
    static bool SetWindowLeft(int32_t left);
    static bool SetWindowTop(int32_t top);
    static bool SetWindowWidth(int32_t width);
    static bool SetWindowHeight(int32_t height);
    static bool SetWindowPosition(int32_t left, int32_t top);
    static bool SetWindowSize(int32_t width, int32_t height);
    static bool SetCursorVisible(bool value);
    static bool SetTitle(std::u16string_view title);
    static bool SetCursorLeft(int32_t left);
    static bool SetCursorTop(int32_t top);
    static std::shared_ptr<Encoding> GetInputEncoding();
    static std::shared_ptr<Encoding> GetOutputEncoding();
    static int32_t GetCursorSize();
    static bool GetTreatControlCAsInput();
    static ConsoleColor GetBackgroundColor();
    static ConsoleColor GetForegroundColor();
    static int32_t GetBufferWidth();
    static int32_t GetBufferHeight();
    static int32_t GetWindowLeft();
    static int32_t GetWindowTop();
    static int32_t GetWindowWidth();
    static int32_t GetWindowHeight();
    static bool GetCursorVisible();
    static std::u16string GetTitle();
    static int32_t GetCursorLeft();
    static int32_t GetCursorTop();
    static int32_t LargestWindowWidth();
    static int32_t LargestWindowHeight();
    static bool IsInputRedirected();
    static bool IsOutputRedirected();
    static bool IsErrorRedirected();
    static void Beep();
    static void Beep(int32_t frequency, int32_t duration);
    static void MoveBufferArea(int32_t sourceLeft, int32_t sourceTop, int32_t sourceWidth, int32_t sourceHeight, int32_t targetLeft, int32_t targetTop, char sourceChar, ConsoleColor sourceForeColor, ConsoleColor sourceBackColor);
    static bool Clear();
    static void SetIn(std::shared_ptr<TextReader> newIn);
    static void SetOut(std::shared_ptr<TextWriter> newOut);
    static void SetError(std::shared_ptr<TextWriter> newError);
    static int32_t Read();
    static std::u16string ReadLine();
    static void Write(std::u16string_view value);
    static void Write(const char16_t* value);
    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    static void Write(T value);
    static void Write(bool value);
    static void WriteLine();
    static void WriteLine(std::u16string_view value);
    static void WriteLine(const char16_t* value);
    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    static void WriteLine(T value);
    static void WriteLine(bool value);

private:
    static std::shared_ptr<TextReader> CreateReader(std::shared_ptr<Stream> stream);
    static std::shared_ptr<TextWriter> CreateWriter(std::shared_ptr<Stream> stream);
    static int32_t GetDefaultConsoleInputCP();
    static int32_t GetDefaultConsoleOutputCP();

private:
    inline static std::shared_ptr<Encoding> inputEncoding_;
    inline static std::shared_ptr<Encoding> outputEncoding_;
    inline static std::shared_ptr<TextReader> in_;
    inline static std::shared_ptr<TextWriter> out_;
    inline static std::shared_ptr<TextWriter> error_;
};

template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int>>
void Console::Write(T value)
{
    Out()->Write(value);
}

template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int>>
void Console::WriteLine(T value)
{
    Out()->WriteLine(value);
}

CS2CPP_NAMESPACE_END