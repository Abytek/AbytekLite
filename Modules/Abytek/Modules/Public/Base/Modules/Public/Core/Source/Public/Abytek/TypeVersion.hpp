#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/CrtUseAllocatorTypedefs.hpp"
#include "Abytek/HashBase.hpp"


namespace Abytek
{
    using F_TypeVersionHashCode = U64;
    
    template<typename __F>
    struct TF_TypeVersionHashCode;
    
    template<typename __F>
    constexpr F_TypeVersionHashCode GetTypeVersionHashCode()
    {
        return TF_TypeVersionHashCode<__F>::Get();
    }
    
    template<>
    struct TF_TypeVersionHashCode<B8>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::B8");
        }
    };
    
    template<>
    struct TF_TypeVersionHashCode<U8>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::U8");
        }
    };
    
    template<>
    struct TF_TypeVersionHashCode<U16>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::U16");
        }
    };
    
    template<>
    struct TF_TypeVersionHashCode<U32>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::U32");
        }
    };
    
    template<>
    struct TF_TypeVersionHashCode<U64>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::U64");
        }
    };
    
    template<>
    struct TF_TypeVersionHashCode<I8>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::I8");
        }
    };
    
    template<>
    struct TF_TypeVersionHashCode<I16>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::I16");
        }
    };
    
    template<>
    struct TF_TypeVersionHashCode<I32>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::I32");
        }
    };
    
    template<>
    struct TF_TypeVersionHashCode<I64>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::I64");
        }
    };
    
    template<>
    struct TF_TypeVersionHashCode<F_SzForSpecifications>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::Sz");
        }
    };
    template<>
    struct TF_TypeVersionHashCode<F_PDiffForSpecifications>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::PDiff");
        }
    };
    
    template<>
    struct TF_TypeVersionHashCode<F32>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::F32");
        }
    };
    
    template<>
    struct TF_TypeVersionHashCode<F64>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("Abytek::F64");
        }
    };
    
    template<>
    struct TF_TypeVersionHashCode<void>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            return HashStringView("void");
        }
    };
    
    template<typename __F>
    struct TF_TypeVersionHashCode<const __F>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("const"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F>());
            return Result;
        }
    };
    
    template<typename __F>
    struct TF_TypeVersionHashCode<__F*>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("*"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F>());
            return Result;
        }
    };
    
    template<typename __F>
    struct TF_TypeVersionHashCode<__F&&>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("&&"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F>());
            return Result;
        }
    };
    template<typename __F>
    struct TF_TypeVersionHashCode<__F&>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("&"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F>());
            return Result;
        }
    };
}