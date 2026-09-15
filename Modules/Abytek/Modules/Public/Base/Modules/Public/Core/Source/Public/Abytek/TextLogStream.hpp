#pragma once

#include "Abytek/LogStream.hpp"


namespace Abytek
{
    /**
     * @brief ANSI escape code for black foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BLACK ABYTEK_TEXT("\x1B[30m")
    /**
     * @brief ANSI escape code for red foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_RED ABYTEK_TEXT("\x1B[31m")
    /**
     * @brief ANSI escape code for green foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_GREEN ABYTEK_TEXT("\x1B[32m")
    /**
     * @brief ANSI escape code for yellow foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_YELLOW ABYTEK_TEXT("\x1B[33m")
    /**
     * @brief ANSI escape code for blue foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BLUE ABYTEK_TEXT("\x1B[34m")
    /**
     * @brief ANSI escape code for magenta foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_MAGNETA ABYTEK_TEXT("\x1B[35m")
    /**
     * @brief ANSI escape code for cyan foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_CYAN ABYTEK_TEXT("\x1B[36m")
    /**
     * @brief ANSI escape code for white foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_WHITE ABYTEK_TEXT("\x1B[37m")
    /**
     * @brief ANSI escape code for bright black foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_BLACK ABYTEK_TEXT("\x1B[90m")
    /**
     * @brief ANSI escape code for bright red foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_RED ABYTEK_TEXT("\x1B[91m")
    /**
     * @brief ANSI escape code for bright green foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_GREEN ABYTEK_TEXT("\x1B[92m")
    /**
     * @brief ANSI escape code for bright yellow foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_YELLOW ABYTEK_TEXT("\x1B[93m")
    /**
     * @brief ANSI escape code for bright blue foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_BLUE ABYTEK_TEXT("\x1B[94m")
    /**
     * @brief ANSI escape code for bright magenta foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_MAGNETA ABYTEK_TEXT("\x1B[95m")
    /**
     * @brief ANSI escape code for bright cyan foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_CYAN ABYTEK_TEXT("\x1B[96m")
    /**
     * @brief ANSI escape code for bright white foreground text
     */
#define ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_WHITE ABYTEK_TEXT("\x1B[97m")
    /**
     * @brief ANSI escape code for black background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BLACK  ABYTEK_TEXT("\x1B[40m")
    /**
     * @brief ANSI escape code for red background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_RED  ABYTEK_TEXT("\x1B[41m")
    /**
     * @brief ANSI escape code for green background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_GREEN  ABYTEK_TEXT("\x1B[42m")
    /**
     * @brief ANSI escape code for yellow background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_YELLOW  ABYTEK_TEXT("\x1B[43m")
    /**
     * @brief ANSI escape code for blue background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BLUE  ABYTEK_TEXT("\x1B[44m")
    /**
     * @brief ANSI escape code for magenta background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_MAGNETA  ABYTEK_TEXT("\x1B[45m")
    /**
     * @brief ANSI escape code for cyan background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_CYAN  ABYTEK_TEXT("\x1B[46m")
    /**
     * @brief ANSI escape code for white background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_WHITE  ABYTEK_TEXT("\x1B[47m")
    /**
     * @brief ANSI escape code for bright black background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_BLACK  ABYTEK_TEXT("\x1B[100m")
    /**
     * @brief ANSI escape code for bright red background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_RED  ABYTEK_TEXT("\x1B[101m")
    /**
     * @brief ANSI escape code for bright green background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_GREEN  ABYTEK_TEXT("\x1B[102m")
    /**
     * @brief ANSI escape code for bright yellow background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_YELLOW  ABYTEK_TEXT("\x1B[103m")
    /**
     * @brief ANSI escape code for bright blue background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_BLUE  ABYTEK_TEXT("\x1B[104m")
    /**
     * @brief ANSI escape code for bright magenta background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_MAGNETA  ABYTEK_TEXT("\x1B[105m")
    /**
     * @brief ANSI escape code for bright cyan background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_CYAN  ABYTEK_TEXT("\x1B[106m")
    /**
     * @brief ANSI escape code for bright white background
     */
#define ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_WHITE  ABYTEK_TEXT("\x1B[107m")
    /**
     * @brief ANSI escape code to reset all console colors
     */
#define ABYTEK_TEXT_LOG_STREAM_RESET_CONSOLE_COLOR  ABYTEK_TEXT("\033[0m")


    /**
     * @brief Log stream implementation that writes to a standard output text stream
     * 
     * This implementation of the log stream directs output to a standard text stream
     * such as std::cout or std::wcout, with support for ANSI color codes.
     */
    class ABYTEK_BASE_CORE_API A_TextLogStream : public A_LogStream
    {
    private:
        std::basic_stringstream<F_TextChar> _TextStream;
        E_LogType _LogType = E_LogType::NONE;

    public:
        /**
         * @brief Flag to enable/disable color output
         * 
         * When true, ANSI color codes are used to format the output.
         */
        B8 EnableColor = false;

    public:
        /**
         * @brief Gets the pointer to the output text stream
         * @return Pointer to the output text stream
         */
        ABYTEK_FORCE_INLINE const auto& GetTextStream() const noexcept { return _TextStream; }
        ABYTEK_FORCE_INLINE auto GetLogType() const noexcept { return _LogType; }

    protected:
        /**
         * @brief Constructor that takes a pointer to an output text stream
         * @param StdOutTextStreamP Pointer to the output text stream to use
         */
        A_TextLogStream();
        
    public:
        /**
         * @brief Virtual destructor
         */
        virtual ~A_TextLogStream() override;
     
    protected:
        virtual void Push(const F_TextChar* TextChars);

    public:
        /**
         * @brief Adds a new line to the output text stream
         */
        virtual void AddNewLine() override;
        
        /**
         * @brief Adds an unsigned 8-bit integer to the output text stream
         * @param Value The value to add
         */
        virtual void AddU8(U8 Value) override;
        
        /**
         * @brief Adds an unsigned 16-bit integer to the output text stream
         * @param Value The value to add
         */
        virtual void AddU16(U16 Value) override;
        
        /**
         * @brief Adds an unsigned 32-bit integer to the output text stream
         * @param Value The value to add
         */
        virtual void AddU32(U32 Value) override;
        
        /**
         * @brief Adds an unsigned 64-bit integer to the output text stream
         * @param Value The value to add
         */
        virtual void AddU64(U64 Value) override;
        
        /**
         * @brief Adds a signed 8-bit integer to the output text stream
         * @param Value The value to add
         */
        virtual void AddI8(I8 Value) override;
        
        /**
         * @brief Adds a signed 16-bit integer to the output text stream
         * @param Value The value to add
         */
        virtual void AddI16(I16 Value) override;
        
        /**
         * @brief Adds a signed 32-bit integer to the output text stream
         * @param Value The value to add
         */
        virtual void AddI32(I32 Value) override;
        
        /**
         * @brief Adds a signed 64-bit integer to the output text stream
         * @param Value The value to add
         */
        virtual void AddI64(I64 Value) override;
        
        /**
         * @brief Adds a void pointer to the output text stream
         * @param Value The pointer value to add
         */
        virtual void AddVoidP(const void* Value) override;
        
        /**
         * @brief Adds a size value to the output text stream
         * @param Value The size value to add
         */
        virtual void AddSz(Sz Value) override;
        
        /**
         * @brief Adds a pointer difference value to the output text stream
         * @param Value The pointer difference value to add
         */
        virtual void AddPDiff(PDiff Value) override;
        
        /**
         * @brief Adds a 32-bit floating point value to the output text stream
         * @param Value The value to add
         */
        virtual void AddF32(F32 Value) override;
        
        /**
         * @brief Adds a 64-bit floating point value to the output text stream
         * @param Value The value to add
         */
        virtual void AddF64(F64 Value) override;
        
        /**
         * @brief Adds a character to the output text stream
         * @param Value The character to add
         */
        virtual void AddChar(F_Char Value) override;
        
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        /**
         * @brief Adds a wide character to the output text stream
         * @param Value The wide character to add
         */
        virtual void AddTextChar(F_TextChar Value) override;
#endif
        
        /**
         * @brief Adds a C-string to the output text stream
         * @param Value The C-string to add
         */
        virtual void AddCStr(const F_Char* Value) override;
        
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        /**
         * @brief Adds a wide C-string to the output text stream
         * @param Value The wide C-string to add
         */
        virtual void AddTextCStr(const F_TextChar* Value) override;
#endif
        
        /**
         * @brief Adds a log header to the output text stream
         * @param Value The log header to add
         */
        virtual void AddLogHeader(const F_LogHeader& Value) override;
        
        /**
         * @brief Updates the log text color in the output text stream
         * @param Value The color settings to apply
         */
        virtual void UpdateLogColor(const F_UpdateLogColor& Value) override;
        
        /**
         * @brief Resets the log text color in the output text stream
         */
        virtual void ResetLogColor() override;
     
        virtual void SetLogType(E_LogType LogType) override;
     
        virtual void Flush() override;
    };
}