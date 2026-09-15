#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Checks if a 64-bit unsigned integer is a power of two
     * @param Value The value to check
     * @return True if the value is a power of two, false otherwise
     */
    static ABYTEK_FORCE_INLINE B8 IsPowerOfTwo(U64 Value){

        return ((Value & (Value - 1)) == 0);
    }
    
    /**
     * @brief Checks if a 32-bit unsigned integer is a power of two
     * @param Value The value to check
     * @return True if the value is a power of two, false otherwise
     */
    static ABYTEK_FORCE_INLINE B8 IsPowerOfTwo(U32 Value){

        return ((Value & (Value - 1)) == 0);
    }
    
    /**
     * @brief Checks if a 16-bit unsigned integer is a power of two
     * @param Value The value to check
     * @return True if the value is a power of two, false otherwise
     */
    static ABYTEK_FORCE_INLINE B8 IsPowerOfTwo(U16 Value){

        return ((Value & (Value - 1)) == 0);
    }
    
    /**
     * @brief Checks if an 8-bit unsigned integer is a power of two
     * @param Value The value to check
     * @return True if the value is a power of two, false otherwise
     */
    static ABYTEK_FORCE_INLINE B8 IsPowerOfTwo(U8 Value){

        return ((Value & (Value - 1)) == 0);
    }
    
    /**
     * @brief Checks if a floating point value is a power of two
     * @param Value The value to check
     * @return True if the value is a power of two, false otherwise
     */
    static ABYTEK_FORCE_INLINE B8 IsPowerOfTwo(F32 Value){
            
        F32 Exponental = std::log(Value) / std::log(2.0f);
            
        return abs(Exponental - F32(I32(Exponental))) <= 0.0001f;
    }
    
    /**
     * @brief Rounds a floating point value down to the nearest power of two
     * @param Value The value to round down
     * @return The nearest power of two that is less than or equal to the input value
     */
    static ABYTEK_FORCE_INLINE F32 RoundDownToPowerOfTwo(F32 Value)
    {
        F32 Exponental = std::floor(std::log(Value) / std::log(2.0f));
            
        return std::pow(2.0f, Exponental);
    }
    
    /**
     * @brief Rounds a floating point value up to the nearest power of two
     * @param Value The value to round up
     * @return The nearest power of two that is greater than or equal to the input value
     */
    static ABYTEK_FORCE_INLINE F32 RoundUpToPowerOfTwo(F32 Value)
    {
        F32 Exponental = std::ceil(std::log(Value) / std::log(2.0f));
            
        return std::pow(2.0f, Exponental);
    }

    static ABYTEK_FORCE_INLINE U32 RoundUpToPowerOfTwo(U32 x)
    {
        if (x <= 1) return 1;

        x--;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        x++;

        return x;
    }
    static ABYTEK_FORCE_INLINE U64 RoundUpToPowerOfTwo(U64 x)
    {
        if (x <= 1) return 1;

        x--;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        x |= x >> 32;
        x++;

        return x;
    }

    static ABYTEK_FORCE_INLINE U32 RoundDownToPowerOfTwo(U32 x)
    {
        if (x == 0) return 0;

        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;

        return x - (x >> 1);
    }
    static ABYTEK_FORCE_INLINE U64 RoundDownToPowerOfTwo(U64 x)
    {
        if (x == 0) return 0;

        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        x |= x >> 32;

        return x - (x >> 1);
    }
    
    static ABYTEK_FORCE_INLINE U64 RoundUpDivide(U64 A, U64 B)
    {
        return (A + (B - 1)) / B;
    }
    static ABYTEK_FORCE_INLINE U64 RoundDownDivide(U64 A, U64 B)
    {
        return A / B;
    }
    
    ABYTEK_FORCE_INLINE U32 MurmurFinalize32(U32 Hash)
    {
        Hash ^= Hash >> 16;
        Hash *= 0x85ebca6b;
        Hash ^= Hash >> 13;
        Hash *= 0xc2b2ae35;
        Hash ^= Hash >> 16;
        return Hash;
    }
    ABYTEK_FORCE_INLINE U64 MurmurFinalize64(U64 Hash)
    {
        Hash ^= Hash >> 33;
        Hash *= 0xff51afd7ed558ccdull;
        Hash ^= Hash >> 33;
        Hash *= 0xc4ceb9fe1a85ec53ull;
        Hash ^= Hash >> 33;
        return Hash;
    }
    ABYTEK_FORCE_INLINE U32 Murmur32(std::initializer_list<U32> InitList)
    {
        U32 Hash = 0;
        for( auto Element : InitList )
        {
            Element *= 0xcc9e2d51;
            Element = ( Element << 15 ) | ( Element >> (32 - 15) );
            Element *= 0x1b873593;
    
            Hash ^= Element;
            Hash = ( Hash << 13 ) | ( Hash >> (32 - 13) );
            Hash = Hash * 5 + 0xe6546b64;
        }
        return MurmurFinalize32( Hash );
    }
}