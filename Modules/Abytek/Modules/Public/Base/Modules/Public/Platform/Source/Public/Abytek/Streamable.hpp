#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


namespace Abytek
{
    struct ABYTEK_BASE_PLATFORM_API I_Streamable
    {
    public:
        virtual PDiff GetOffset() const = 0;
        
        /**
         * @brief Gets the size of the file
         * @return The file size in bytes, or none on error
         */
        virtual TF_Optional<Sz> GetSize() const = 0;
        
        /**
         * @brief Seeks to a specific position in the file
         * @param NewOffset The new position to seek to
         * @return True if successful, false otherwise
         */
        virtual B8 Seek(PDiff NewOffset) = 0;
        
        /**
         * @brief Reads binary data from the file
         * @param OutData Span to store the read data
         * @return The number of bytes read, or none on error
         */
        virtual TF_Optional<Sz> ReadBinary(const TF_Span<U8>& OutData) = 0;
        
        /**
         * @brief Writes binary data to the file
         * @param Data Span containing the data to write
         * @return The number of bytes written, or none on error
         */
        virtual TF_Optional<Sz> WriteBinary(const TF_Span<U8>& Data) = 0;
        
    public:
        /**
         * @brief Seeks to the end of the file
         * @return True if successful, false otherwise
         */
        B8 Ate();
        
        /**
         * @brief Reads binary data from the file
         * @param OutData Span to store the read data
         * @return true if succeeded
         */
        B8 ReadBinaryExact(const TF_Span<U8>& OutData);
        
        /**
         * @brief Writes binary data to the file
         * @param Data Span containing the data to write
         * @return true if succeeded
         */
        B8 WriteBinaryExact(const TF_Span<U8>& Data);
        
        /**
         * @brief Reads a typed value from the file
         * @tparam __F The type of value to read
         * @param OutData Reference to store the read value
         * @return The number of bytes read, or none on error
         */
        template<typename __F>
        TF_Optional<Sz> Read(__F& OutData)
        {
            return ReadBinary({ (U8*)&OutData, sizeof(__F) });
        }
        
        /**
         * @brief Writes a typed value to the file
         * @tparam __F The type of value to write
         * @param OutData The value to write
         * @return The number of bytes written, or none on error
         */
        template<typename __F>
        TF_Optional<Sz> Write(const __F& OutData)
        {
            return WriteBinary({ (U8*)&OutData, sizeof(__F) });
        }

    public:
        TF_Optional<F_String> ReadStringExclusive();
        B8 WriteStringExclusive(const F_String& Data);
        B8 WriteString(const F_String& Data);
        B8 WriteChar(const F_Char& Char);
        TF_Optional<F_Text> ReadTextExclusive();
        B8 WriteTextExclusive(const F_Text& Text);
        B8 WriteText(const F_Text& Text);
        B8 WriteTextChar(const F_TextChar& TextChar);
    };
}