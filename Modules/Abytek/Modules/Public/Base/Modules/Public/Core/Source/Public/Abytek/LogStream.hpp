#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    enum class E_LogType : U8
    {
        NONE,
        LT_INFO,
        LT_VERBOSE,
        LT_VERY_VERBOSE,
        LT_WARNING,
        LT_FATAL,
        LT_ERROR
    };
    
    /**
     * @brief Structure representing a new line in log output
     */
    struct F_NewLine
    {
    };

    /**
     * @brief Fake char structure for type disambiguation in log stream operators
     */
    struct F_FakeChar
    {
    };

    /**
     * @brief Fake text char structure for type disambiguation in log stream operators
     */
    struct F_FakeTextChar
    {
    };

    /**
     * @brief Fake size structure for type disambiguation in log stream operators
     */
    struct F_FakeSz
    {
        Sz Value;
    };
    using F_SzForSpecifications = std::conditional_t<
        std::is_same_v<
            Sz,
            TF_UInt<sizeof(Sz)>
        >,
        F_FakeSz,
        Sz
    >;

    /**
     * @brief Fake pointer difference structure for type disambiguation in log stream operators
     */
    struct F_FakePDiff
    {
        PDiff Value;
    };
    using F_PDiffForSpecifications = std::conditional_t<
        std::is_same_v<
            PDiff,
            TF_Int<sizeof(PDiff)>
        >,
        F_FakePDiff,
        PDiff
    >;

    /**
     * @brief Structure containing header information for log messages
     */
    struct F_LogHeader
    {
        const F_TextChar* FilePathCStr = 0;    ///< Path to the file where the log was generated
        U32 Line = ABYTEK_U32_MAX;             ///< Line number where the log was generated
        const F_TextChar* Prefix = 0;          ///< Optional prefix for the log message
        const F_TextChar* Suffix = 0;          ///< Optional suffix for the log message
        const F_TextChar* Body = ABYTEK_TEXT("LOG"); ///< Body text for the log header
    };

    /**
     * @brief Enumeration of color values for log output
     */
    enum class E_LogColorValue
    {
        BLACK = 0x0,     ///< Black color
        RED = 0x1,       ///< Red color
        GREEN = 0x2,     ///< Green color
        YELLOW = 0x3,    ///< Yellow color
        BLUE = 0x4,      ///< Blue color
        MAGNETA = 0x5,   ///< Magenta color
        CYAN = 0x6,      ///< Cyan color
        WHITE = 0x7      ///< White color
    };

    /**
     * @brief Enumeration for color brightness settings
     */
    enum class E_LogColorBrightness
    {
        DARK = 0x0,      ///< Dark/normal color
        BRIGHT = 0x1     ///< Bright/bold color
    };

    /**
     * @brief Enumeration for color application target (foreground or background)
     */
    enum class E_LogColorTarget
    {
        FOREGROUND = 0x0, ///< Apply color to foreground text
        BACKGROUND = 0x1  ///< Apply color to background
    };

    /**
     * @brief Structure for updating log text color
     */
    struct F_UpdateLogColor
    {
        E_LogColorValue Value = E_LogColorValue::WHITE;           ///< Color value
        E_LogColorBrightness Brightness = E_LogColorBrightness::BRIGHT; ///< Brightness setting
        E_LogColorTarget Target = E_LogColorTarget::FOREGROUND;   ///< Target (foreground/background)

        /**
         * @brief Creates a preset for low-light text
         * @return Color setting for low-light text
         */
        static F_UpdateLogColor LowLight()
        {
            return {
                E_LogColorValue::WHITE,
                E_LogColorBrightness::DARK,
                E_LogColorTarget::FOREGROUND
            };
        }

        /**
         * @brief Creates a preset for string values
         * @return Color setting for string values
         */
        static F_UpdateLogColor String()
        {
            return {
                E_LogColorValue::GREEN,
                E_LogColorBrightness::BRIGHT,
                E_LogColorTarget::FOREGROUND
            };
        }

        /**
         * @brief Creates a preset for integer values
         * @return Color setting for integer values
         */
        static F_UpdateLogColor Int()
        {
            return {
                E_LogColorValue::CYAN,
                E_LogColorBrightness::BRIGHT,
                E_LogColorTarget::FOREGROUND
            };
        }

        /**
         * @brief Creates a preset for unsigned integer values
         * @return Color setting for unsigned integer values
         */
        static F_UpdateLogColor UInt()
        {
            return Int();
        }

        /**
         * @brief Creates a preset for field names
         * @return Color setting for field names
         */
        static F_UpdateLogColor Field()
        {
            return {
                E_LogColorValue::WHITE,
                E_LogColorBrightness::BRIGHT,
                E_LogColorTarget::FOREGROUND
            };
        }
    };

    /**
     * @brief Structure for resetting log color to default
     */
    struct F_ResetLogColor
    {
    };


    /**
     * @brief Abstract base class for log stream implementations
     * 
     * Provides a consistent interface for logging different types of data
     * to various output targets.
     */
    class ABYTEK_BASE_CORE_API A_LogStream
    {
    protected:
        /**
         * @brief Protected constructor, only derived classes can instantiate
         */
        A_LogStream();

    public:
        /**
         * @brief Virtual destructor
         */
        virtual ~A_LogStream();

    public:
        A_LogStream(const A_LogStream&) = delete;
        A_LogStream& operator = (const A_LogStream&) = delete;

    public:
        /**
         * @brief Adds a new line to the log output
         */
        virtual void AddNewLine() = 0;

        /**
         * @brief Placeholder for size type to avoid ambiguity
         */
        ABYTEK_FORCE_INLINE void AddSz(F_FakeSz) {}

        /**
         * @brief Placeholder for pointer difference type to avoid ambiguity
         */
        ABYTEK_FORCE_INLINE void AddPDiff(F_FakePDiff) {}

        /**
         * @brief Placeholder for char type to avoid ambiguity
         */
        ABYTEK_FORCE_INLINE void AddChar(F_FakeChar) {}

#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        /**
         * @brief Placeholder for text char type to avoid ambiguity
         */
        ABYTEK_FORCE_INLINE void AddTextChar(F_FakeTextChar) {}
#endif

        /**
         * @brief Adds an unsigned 8-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddU8(U8 Value) = 0;

        /**
         * @brief Adds an unsigned 16-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddU16(U16 Value) = 0;

        /**
         * @brief Adds an unsigned 32-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddU32(U32 Value) = 0;

        /**
         * @brief Adds an unsigned 64-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddU64(U64 Value) = 0;

        /**
         * @brief Adds a signed 8-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddI8(I8 Value) = 0;

        /**
         * @brief Adds a signed 16-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddI16(I16 Value) = 0;

        /**
         * @brief Adds a signed 32-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddI32(I32 Value) = 0;

        /**
         * @brief Adds a signed 64-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddI64(I64 Value) = 0;

        /**
         * @brief Adds a void pointer to the log
         * @param Value The pointer value to add
         */
        virtual void AddVoidP(const void* Value) = 0;

        /**
         * @brief Adds a size value to the log
         * @param Value The size value to add
         */
        virtual void AddSz(Sz Value) = 0;

        /**
         * @brief Adds a pointer difference value to the log
         * @param Value The pointer difference value to add
         */
        virtual void AddPDiff(PDiff Value) = 0;

        /**
         * @brief Adds a 32-bit floating point value to the log
         * @param Value The value to add
         */
        virtual void AddF32(F32 Value) = 0;

        /**
         * @brief Adds a 64-bit floating point value to the log
         * @param Value The value to add
         */
        virtual void AddF64(F64 Value) = 0;

        /**
         * @brief Adds a character to the log
         * @param Value The character to add
         */
        virtual void AddChar(F_Char Value) = 0;

#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        /**
         * @brief Adds a wide character to the log
         * @param Value The wide character to add
         */
        virtual void AddTextChar(F_TextChar Value) = 0;
#endif

        /**
         * @brief Adds a C-string to the log
         * @param Value The C-string to add
         */
        virtual void AddCStr(const F_Char* Value) = 0;

#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        /**
         * @brief Adds a wide C-string to the log
         * @param Value The wide C-string to add
         */
        virtual void AddTextCStr(const F_TextChar* Value) = 0;
#endif

        /**
         * @brief Adds a log header to the log
         * @param Value The log header to add
         */
        virtual void AddLogHeader(const F_LogHeader& Value) = 0;

        /**
         * @brief Updates the log text color
         * @param Value The color settings to apply
         */
        virtual void UpdateLogColor(const F_UpdateLogColor& Value) = 0;

        /**
         * @brief Resets the log text color to default
         */
        virtual void ResetLogColor() = 0;
        
        virtual void SetLogType(E_LogType LogType) {}
        virtual void Flush() {}
    };


    /**
     * @brief Stream operator for adding a new line to a log stream
     * @param LogStream The log stream to add to
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, F_NewLine)
    {
        LogStream.AddNewLine();
        return LogStream;
    }

#pragma region Primitive Types
    /**
     * @brief Stream operator for adding an unsigned 8-bit integer to a log stream
     * @param LogStream The log stream to add to
     * @param Value The value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, U8 Value)
    {
        LogStream.AddU8(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding an unsigned 16-bit integer to a log stream
     * @param LogStream The log stream to add to
     * @param Value The value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, U16 Value)
    {
        LogStream.AddU16(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding an unsigned 32-bit integer to a log stream
     * @param LogStream The log stream to add to
     * @param Value The value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, U32 Value)
    {
        LogStream.AddU32(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding an unsigned 64-bit integer to a log stream
     * @param LogStream The log stream to add to
     * @param Value The value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, U64 Value)
    {
        LogStream.AddU64(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding a signed 8-bit integer to a log stream
     * @param LogStream The log stream to add to
     * @param Value The value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, I8 Value)
    {
        LogStream.AddI8(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding a signed 16-bit integer to a log stream
     * @param LogStream The log stream to add to
     * @param Value The value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, I16 Value)
    {
        LogStream.AddI16(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding a signed 32-bit integer to a log stream
     * @param LogStream The log stream to add to
     * @param Value The value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, I32 Value)
    {
        LogStream.AddI32(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding a signed 64-bit integer to a log stream
     * @param LogStream The log stream to add to
     * @param Value The value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, I64 Value)
    {
        LogStream.AddI64(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding a pointer to a log stream
     * @param LogStream The log stream to add to
     * @param Value The pointer value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, const void* Value)
    {
        LogStream.AddVoidP(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding a size value to a log stream
     * 
     * This overload handles type disambiguation with conditional types
     * 
     * @param LogStream The log stream to add to
     * @param Value The size value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (
        A_LogStream& LogStream,
        F_SzForSpecifications Value
    )
    {
        using namespace Abytek;
        LogStream.AddSz(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding a pointer difference value to a log stream
     * 
     * This overload handles type disambiguation with conditional types
     * 
     * @param LogStream The log stream to add to
     * @param Value The pointer difference value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (
        A_LogStream& LogStream,
        F_PDiffForSpecifications Value
    )
    {
        using namespace Abytek;
        LogStream.AddPDiff(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding a 32-bit float to a log stream
     * @param LogStream The log stream to add to
     * @param Value The value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, F32 Value)
    {
        LogStream.AddF32(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding a 64-bit float to a log stream
     * @param LogStream The log stream to add to
     * @param Value The value to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, F64 Value)
    {
        LogStream.AddF64(Value);
        return LogStream;
    }
#pragma endregion

#pragma region Char, String Types
    /**
     * @brief Stream operator for adding a character to a log stream
     * @param LogStream The log stream to add to
     * @param Value The character to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, F_Char Value)
    {
        LogStream.AddChar(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding a C-string to a log stream
     * @param LogStream The log stream to add to
     * @param Value The C-string to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, const F_Char* Value)
    {
        LogStream.AddCStr(Value);
        return LogStream;
    }
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
    /**
     * @brief Stream operator for adding a wide character to a log stream
     * @param LogStream The log stream to add to
     * @param Value The wide character to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, F_TextChar Value)
    {
        LogStream.AddTextChar(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for adding a wide C-string to a log stream
     * @param LogStream The log stream to add to
     * @param Value The wide C-string to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, const F_TextChar* Value)
    {
        LogStream.AddTextCStr(Value);
        return LogStream;
    }
#endif
#pragma endregion

    /**
     * @brief Stream operator for adding a log header to a log stream
     * @param LogStream The log stream to add to
     * @param Value The log header to add
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, const F_LogHeader& Value)
    {
        LogStream.AddLogHeader(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for updating log color
     * @param LogStream The log stream to update
     * @param Value The color settings to apply
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, const F_UpdateLogColor& Value)
    {
        LogStream.UpdateLogColor(Value);
        return LogStream;
    }

    /**
     * @brief Stream operator for resetting log color
     * @param LogStream The log stream to reset
     * @return Reference to the log stream for chaining
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, F_ResetLogColor)
    {
        LogStream.ResetLogColor();
        return LogStream;
    }
    
    inline A_LogStream& operator << (A_LogStream& LogStream, E_LogType LogType)
    {
        LogStream.SetLogType(LogType);
        return LogStream;
    }
}