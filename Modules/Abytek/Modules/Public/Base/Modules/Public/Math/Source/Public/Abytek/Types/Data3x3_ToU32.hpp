#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data3x3_F32.hpp"
#include "Abytek/Types/Data3x3_I64.hpp"
#include "Abytek/Types/Data3x3_I32.hpp"
#include "Abytek/Types/Data3x3_I16.hpp"
#include "Abytek/Types/Data3x3_I8.hpp"
#include "Abytek/Types/Data3x3_U64.hpp"
#include "Abytek/Types/Data3x3_U32.hpp"
#include "Abytek/Types/Data3x3_U16.hpp"
#include "Abytek/Types/Data3x3_U8.hpp"


namespace Abytek
{
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>::TF_Data(typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B),
        C(Other.C)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>& TF_Data<3, 3, U32, __F_Config>::operator = (typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        C = Other.C;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>::TF_Data(typename TF_RebindElement<F32>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B),
        C(Other.C)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>& TF_Data<3, 3, U32, __F_Config>::operator = (typename TF_RebindElement<F32>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        C = Other.C;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>::TF_Data(typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B),
        C(Other.C)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>& TF_Data<3, 3, U32, __F_Config>::operator = (typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        C = Other.C;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>::TF_Data(typename TF_RebindElement<I8>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B),
        C(Other.C)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>& TF_Data<3, 3, U32, __F_Config>::operator = (typename TF_RebindElement<I8>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        C = Other.C;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>::TF_Data(typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B),
        C(Other.C)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>& TF_Data<3, 3, U32, __F_Config>::operator = (typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        C = Other.C;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>::TF_Data(typename TF_RebindElement<I64>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B),
        C(Other.C)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>& TF_Data<3, 3, U32, __F_Config>::operator = (typename TF_RebindElement<I64>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        C = Other.C;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>::TF_Data(typename TF_RebindElement<U64>::F_PassedArgument Other) noexcept :
        A(Other.A),
        B(Other.B),
        C(Other.C)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 3, U32, __F_Config>& TF_Data<3, 3, U32, __F_Config>::operator = (typename TF_RebindElement<U64>::F_PassedArgument Other) noexcept
    {
        A = Other.A;
        B = Other.B;
        C = Other.C;
        
        return *this;
    }
}