#pragma once

#include "Abytek/BoostContainerTemplates.hpp"


namespace Abytek
{
    template<typename __F>
    using TF_Hash = boost::hash<__F>;

    template<typename __F>
    ABYTEK_FORCE_INLINE size_t GetHashValue(const __F& X)
    {
        return TF_Hash<__F>()(X);
    }

    constexpr ABYTEK_FORCE_INLINE U64 HashCombineU64(U64 A, U64 B)
    {
        uint64_t h = A;
        h ^= B + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        return h;
    }
    
    template<typename __F>
    struct TF_HashBasedLess 
    {
        B8 operator()(const __F& Left, const __F& Right) const noexcept
        {
            using F_Hash = TF_Hash<__F>;
            return F_Hash()(Left) < F_Hash()(Right);
        }
    };
}