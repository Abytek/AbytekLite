#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data2_F32.hpp"
#include "Abytek/Types/Data2_I64.hpp"
#include "Abytek/Types/Data2_I32.hpp"
#include "Abytek/Types/Data2_I16.hpp"
#include "Abytek/Types/Data2_I8.hpp"
#include "Abytek/Types/Data2_U64.hpp"
#include "Abytek/Types/Data2_U32.hpp"
#include "Abytek/Types/Data2_U16.hpp"
#include "Abytek/Types/Data2_U8.hpp"


namespace Abytek
{
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>::TF_Data(typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>& TF_Data<2, 1, U64, __F_Config>::operator = (typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>::TF_Data(typename TF_RebindElement<F32>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>& TF_Data<2, 1, U64, __F_Config>::operator = (typename TF_RebindElement<F32>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>::TF_Data(typename TF_RebindElement<U32>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>& TF_Data<2, 1, U64, __F_Config>::operator = (typename TF_RebindElement<U32>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>::TF_Data(typename TF_RebindElement<U16>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>& TF_Data<2, 1, U64, __F_Config>::operator = (typename TF_RebindElement<U16>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>::TF_Data(typename TF_RebindElement<I8>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>& TF_Data<2, 1, U64, __F_Config>::operator = (typename TF_RebindElement<I8>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>::TF_Data(typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>& TF_Data<2, 1, U64, __F_Config>::operator = (typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>::TF_Data(typename TF_RebindElement<I64>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>& TF_Data<2, 1, U64, __F_Config>::operator = (typename TF_RebindElement<I64>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>::TF_Data(typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 1, U64, __F_Config>& TF_Data<2, 1, U64, __F_Config>::operator = (typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        
        return *this;
    }
}