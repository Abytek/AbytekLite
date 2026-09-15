#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data2x2_F32.hpp"
#include "Abytek/Types/Data2x2_I64.hpp"
#include "Abytek/Types/Data2x2_I32.hpp"
#include "Abytek/Types/Data2x2_I16.hpp"
#include "Abytek/Types/Data2x2_I8.hpp"
#include "Abytek/Types/Data2x2_U64.hpp"
#include "Abytek/Types/Data2x2_U32.hpp"
#include "Abytek/Types/Data2x2_U16.hpp"
#include "Abytek/Types/Data2x2_U8.hpp"


namespace Abytek
{
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>::TF_Data(typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>& TF_Data<2, 2, U16, __F_Config>::operator = (typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>::TF_Data(typename TF_RebindElement<F32>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>& TF_Data<2, 2, U16, __F_Config>::operator = (typename TF_RebindElement<F32>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>::TF_Data(typename TF_RebindElement<U32>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>& TF_Data<2, 2, U16, __F_Config>::operator = (typename TF_RebindElement<U32>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>::TF_Data(typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>& TF_Data<2, 2, U16, __F_Config>::operator = (typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>::TF_Data(typename TF_RebindElement<I8>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>& TF_Data<2, 2, U16, __F_Config>::operator = (typename TF_RebindElement<I8>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>::TF_Data(typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>& TF_Data<2, 2, U16, __F_Config>::operator = (typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>::TF_Data(typename TF_RebindElement<I64>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>& TF_Data<2, 2, U16, __F_Config>::operator = (typename TF_RebindElement<I64>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>::TF_Data(typename TF_RebindElement<U64>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<2, 2, U16, __F_Config>& TF_Data<2, 2, U16, __F_Config>::operator = (typename TF_RebindElement<U64>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        
        return *this;
    }
}