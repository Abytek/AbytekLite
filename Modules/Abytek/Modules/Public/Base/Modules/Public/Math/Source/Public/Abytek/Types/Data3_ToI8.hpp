#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data3_F32.hpp"
#include "Abytek/Types/Data3_I64.hpp"
#include "Abytek/Types/Data3_I32.hpp"
#include "Abytek/Types/Data3_I16.hpp"
#include "Abytek/Types/Data3_I8.hpp"
#include "Abytek/Types/Data3_U64.hpp"
#include "Abytek/Types/Data3_U32.hpp"
#include "Abytek/Types/Data3_U16.hpp"
#include "Abytek/Types/Data3_U8.hpp"


namespace Abytek
{
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>::TF_Data(typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y),
        Z(Other.Z)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>& TF_Data<3, 1, I8, __F_Config>::operator = (typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        Z = Other.Z;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>::TF_Data(typename TF_RebindElement<F32>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y),
        Z(Other.Z)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>& TF_Data<3, 1, I8, __F_Config>::operator = (typename TF_RebindElement<F32>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        Z = Other.Z;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>::TF_Data(typename TF_RebindElement<U32>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y),
        Z(Other.Z)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>& TF_Data<3, 1, I8, __F_Config>::operator = (typename TF_RebindElement<U32>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        Z = Other.Z;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>::TF_Data(typename TF_RebindElement<U16>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y),
        Z(Other.Z)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>& TF_Data<3, 1, I8, __F_Config>::operator = (typename TF_RebindElement<U16>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        Z = Other.Z;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>::TF_Data(typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y),
        Z(Other.Z)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>& TF_Data<3, 1, I8, __F_Config>::operator = (typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        Z = Other.Z;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>::TF_Data(typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y),
        Z(Other.Z)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>& TF_Data<3, 1, I8, __F_Config>::operator = (typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        Z = Other.Z;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>::TF_Data(typename TF_RebindElement<I64>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y),
        Z(Other.Z)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>& TF_Data<3, 1, I8, __F_Config>::operator = (typename TF_RebindElement<I64>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        Z = Other.Z;
        
        return *this;
    }
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>::TF_Data(typename TF_RebindElement<U64>::F_PassedArgument Other) noexcept :
        X(Other.X),
        Y(Other.Y),
        Z(Other.Z)
    {}
    template<class __F_Config>
    ABYTEK_FORCE_INLINE TF_Data<3, 1, I8, __F_Config>& TF_Data<3, 1, I8, __F_Config>::operator = (typename TF_RebindElement<U64>::F_PassedArgument Other) noexcept
    {
        X = Other.X;
        Y = Other.Y;
        Z = Other.Z;
        
        return *this;
    }
}