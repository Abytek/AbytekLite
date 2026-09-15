/**
 * @file TransferLogStream.hpp
 * @brief Defines a log stream that forwards log messages to another log stream
 * 
 * This file implements a transfer log stream class that acts as a proxy to another
 * log stream. It forwards all log operations to a target log stream, allowing for
 * redirection of log output.
 */
#pragma once

#include "Abytek/LogStream.hpp"


namespace Abytek
{
    /**
     * @brief A log stream that forwards all operations to a target log stream
     * 
     * F_TransferLogStream acts as a proxy for another log stream. All logging
     * operations are forwarded to the target stream, allowing for redirection
     * of log output without changing the code that generates the logs.
     */
    class ABYTEK_BASE_CORE_API F_TransferLogStream : public A_LogStream
    {
    protected:
        A_LogStream* TargetP = 0;

    public:
        /**
         * @brief Get the pointer to the target log stream
         * @return Pointer to the target log stream
         */
        ABYTEK_FORCE_INLINE auto GetTargetP() const noexcept { return TargetP; }
        
        /**
         * @brief Get a reference to the target log stream
         * @return Constant reference to the target log stream
         */
        ABYTEK_FORCE_INLINE const A_LogStream& GetTarget() const noexcept { return *TargetP; }

    public:
        /**
         * @brief Default constructor
         */
        F_TransferLogStream() = default;
        
        /**
         * @brief Constructor with target log stream
         * @param Target The log stream to forward operations to
         */
        F_TransferLogStream(A_LogStream& Target);
        
        /**
         * @brief Virtual destructor
         */
        virtual ~F_TransferLogStream() override;

    public:
        /**
         * @brief Add a new line to the log
         * @note Forwards the operation to the target log stream
         */
        virtual void AddNewLine() override;
        
        /**
         * @brief Add an unsigned 8-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddU8(U8 Value) override;
        
        /**
         * @brief Add an unsigned 16-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddU16(U16 Value) override;
        
        /**
         * @brief Add an unsigned 32-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddU32(U32 Value) override;
        
        /**
         * @brief Add an unsigned 64-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddU64(U64 Value) override;
        
        /**
         * @brief Add a signed 8-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddI8(I8 Value) override;
        
        /**
         * @brief Add a signed 16-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddI16(I16 Value) override;
        
        /**
         * @brief Add a signed 32-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddI32(I32 Value) override;
        
        /**
         * @brief Add a signed 64-bit integer to the log
         * @param Value The value to add
         */
        virtual void AddI64(I64 Value) override;
        
        /**
         * @brief Add a void pointer to the log
         * @param Value The pointer to add
         */
        virtual void AddVoidP(const void* Value) override;
        
        /**
         * @brief Add a size type to the log
         * @param Value The size value to add
         */
        virtual void AddSz(Sz Value) override;
        
        /**
         * @brief Add a pointer difference type to the log
         * @param Value The pointer difference to add
         */
        virtual void AddPDiff(PDiff Value) override;
        
        /**
         * @brief Add a 32-bit floating point value to the log
         * @param Value The value to add
         */
        virtual void AddF32(F32 Value) override;
        
        /**
         * @brief Add a 64-bit floating point value to the log
         * @param Value The value to add
         */
        virtual void AddF64(F64 Value) override;
        
        /**
         * @brief Add a character to the log
         * @param Value The character to add
         */
        virtual void AddChar(F_Char Value) override;
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        /**
         * @brief Add a text character to the log
         * @param Value The text character to add
         */
        virtual void AddTextChar(F_TextChar Value) override;
#endif
        /**
         * @brief Add a C-style string to the log
         * @param Value The string to add
         */
        virtual void AddCStr(const F_Char* Value) override;
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        /**
         * @brief Add a text C-style string to the log
         * @param Value The text string to add
         */
        virtual void AddTextCStr(const F_TextChar* Value) override;
#endif
        /**
         * @brief Add a log header to the log
         * @param Value The log header to add
         */
        virtual void AddLogHeader(const F_LogHeader& Value) override;
        
        /**
         * @brief Update the log color
         * @param Value The color update information
         */
        virtual void UpdateLogColor(const F_UpdateLogColor& Value) override;
        
        /**
         * @brief Reset the log color to default
         */
        virtual void ResetLogColor() override;
    };
}