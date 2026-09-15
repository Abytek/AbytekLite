#pragma once

#include "Abytek/LogStream.hpp"


namespace Abytek
{
    /**
     * @brief A log stream that forwards logging operations to multiple target streams
     * 
     * This log stream implementation acts as a splitter/broadcaster, forwarding
     * all log operations to an array of target log streams.
     * 
     * @tparam __Size The number of target log streams to forward to
     */
    template<Sz __Size>
    class ABYTEK_BASE_CORE_API TF_SectionLogStream final : public A_LogStream
    {
    public:
        /// The configured size of the target log stream array
        static constexpr Sz Size = __Size;

    private:
        /// Array of pointers to target log streams
        A_LogStream* _TargetPs[Size];

    public:
        /**
         * @brief Gets the array of target log stream pointers
         * @return Array of log stream pointers
         */
        ABYTEK_FORCE_INLINE auto GetTargetPs() const noexcept { return _TargetPs; }

    public:
        /**
         * @brief Default constructor
         */
        TF_SectionLogStream() = default;
        
        /**
         * @brief Constructor with array of target log streams
         * @param TargetPs Array of pointers to target log streams
         */
        TF_SectionLogStream(A_LogStream* TargetPs[Size])
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx] = TargetPs[Idx];
            }
        }
        
        /**
         * @brief Copy constructor
         * @param X The source object to copy from
         */
        TF_SectionLogStream(const TF_SectionLogStream& X)
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx] = X._TargetPs[Idx];
            }
        }
        
        /**
         * @brief Copy assignment operator
         * @param X The source object to copy from
         * @return Reference to this object
         */
        TF_SectionLogStream& operator = (const TF_SectionLogStream& X)
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx] = X._TargetPs[Idx];
            }
            return *this;
        }
        
        /**
         * @brief Virtual destructor
         */
        virtual ~TF_SectionLogStream() override
        {
        }

    public:
        /**
         * @brief Adds a new line to all target log streams
         */
        virtual void AddNewLine() override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddNewLine();
            }
        }
        
        /**
         * @brief Adds an unsigned 8-bit integer to all target log streams
         * @param Value The value to add
         */
        virtual void AddU8(U8 Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddU8(Value);
            }
        }
        
        /**
         * @brief Adds an unsigned 16-bit integer to all target log streams
         * @param Value The value to add
         */
        virtual void AddU16(U16 Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddU16(Value);
            }
        }
        
        /**
         * @brief Adds an unsigned 32-bit integer to all target log streams
         * @param Value The value to add
         */
        virtual void AddU32(U32 Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddU32(Value);
            }
        }
        
        /**
         * @brief Adds an unsigned 64-bit integer to all target log streams
         * @param Value The value to add
         */
        virtual void AddU64(U64 Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddU64(Value);
            }
        }
        
        /**
         * @brief Adds a signed 8-bit integer to all target log streams
         * @param Value The value to add
         */
        virtual void AddI8(I8 Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddI8(Value);
            }
        }
        
        /**
         * @brief Adds a signed 16-bit integer to all target log streams
         * @param Value The value to add
         */
        virtual void AddI16(I16 Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddI16(Value);
            }
        }
        
        /**
         * @brief Adds a signed 32-bit integer to all target log streams
         * @param Value The value to add
         */
        virtual void AddI32(I32 Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddI32(Value);
            }
        }
        
        /**
         * @brief Adds a signed 64-bit integer to all target log streams
         * @param Value The value to add
         */
        virtual void AddI64(I64 Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddI64(Value);
            }
        }
        
        /**
         * @brief Adds a void pointer to all target log streams
         * @param Value The pointer value to add
         */
        virtual void AddVoidP(const void* Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddVoidP(Value);
            }
        }
        
        /**
         * @brief Adds a size value to all target log streams
         * @param Value The size value to add
         */
        virtual void AddSz(Sz Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddSz(Value);
            }
        }
        
        /**
         * @brief Adds a pointer difference value to all target log streams
         * @param Value The pointer difference value to add
         */
        virtual void AddPDiff(PDiff Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddPDiff(Value);
            }
        }
        
        /**
         * @brief Adds a 32-bit floating point value to all target log streams
         * @param Value The value to add
         */
        virtual void AddF32(F32 Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddF32(Value);
            }
        }
        
        /**
         * @brief Adds a 64-bit floating point value to all target log streams
         * @param Value The value to add
         */
        virtual void AddF64(F64 Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddF64(Value);
            }
        }
        
        /**
         * @brief Adds a character to all target log streams
         * @param Value The character to add
         */
        virtual void AddChar(F_Char Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddChar(Value);
            }
        }
        
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        /**
         * @brief Adds a wide character to all target log streams
         * @param Value The wide character to add
         */
        virtual void AddTextChar(F_TextChar Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddTextChar(Value);
            }
        }
#endif
        
        /**
         * @brief Adds a C-string to all target log streams
         * @param Value The C-string to add
         */
        virtual void AddCStr(const F_Char* Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddCStr(Value);
            }
        }
        
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        /**
         * @brief Adds a wide C-string to all target log streams
         * @param Value The wide C-string to add
         */
        virtual void AddTextCStr(const F_TextChar* Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddTextCStr(Value);
            }
        }
#endif
        
        /**
         * @brief Adds a log header to all target log streams
         * @param Value The log header to add
         */
        virtual void AddLogHeader(const F_LogHeader& Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->AddLogHeader(Value);
            }
        }
        
        /**
         * @brief Updates the log color in all target log streams
         * @param Value The color settings to apply
         */
        virtual void UpdateLogColor(const F_UpdateLogColor& Value) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->UpdateLogColor(Value);
            }
        }
        
        /**
         * @brief Resets the log color in all target log streams
         */
        virtual void ResetLogColor() override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->ResetLogColor();
            }
        }

        virtual void SetLogType(E_LogType InLogType) override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->SetLogType(InLogType);
            }
        }

        virtual void Flush() override
        {
            for(U32 Idx = 0; Idx < Size; ++Idx)
            {
                _TargetPs[Idx]->Flush();
            }
        }
    };

    /**
     * @brief Creates an array transfer log stream from multiple target log streams
     * 
     * This helper function creates an array transfer log stream that forwards log
     * operations to all of the provided target log streams.
     * 
     * @tparam __Fs Parameter pack of log stream types
     * @param Targets Target log streams to forward to
     * @return An array transfer log stream that forwards to all targets
     */
    template<typename... __Fs>
    auto MakeSectionLogStream(__Fs&&... Targets)
    {
        A_LogStream* TargetPs[sizeof...(__Fs)]{
            ((A_LogStream*)&Targets)...
        };
        return TF_SectionLogStream<
            sizeof...(__Fs)
        >(TargetPs);
    }
}