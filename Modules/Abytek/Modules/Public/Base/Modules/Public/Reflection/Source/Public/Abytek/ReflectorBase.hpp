#pragma once

#include "Abytek/ReflectionCommon.hpp"
#include "Abytek/ReflectionTypeHandleChecker.hpp"


namespace Abytek
{
    class A_ReflectionItem;
    class F_ReflectionType;
    class F_ReflectionAlias;
    
    /**
     * @brief Specifies the reflection mode to use when reflecting a type
     * 
     * INLINE: Directly embed reflection information within context
     * LINKED: Register linked reflection information that can be loaded on demand
     */
    enum class E_ReflectMode
    {
        INLINE,
        LINKED,
        
        DEFAULT = LINKED
    };

    template<typename __F_Type>
    struct TF_EmptyReflector;
    
    template<typename __F_Type>
    static constexpr B8 ShouldUseEmptyReflector()
    {
        if constexpr (std::is_same_v<__F_Type, B8>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, U8>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, U16>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, U32>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, U64>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, I8>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, I16>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, I32>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, I64>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, F_GeneralTypeHashCode>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, F_Char>)
        {
            return true;
        }
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        else if constexpr (std::is_same_v<__F_Type, F_TextChar>)
        {
            return true;
        }
#endif
        else if constexpr (std::is_same_v<__F_Type, F32>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, F64>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, Sz>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, PDiff>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, A_ReflectionItem>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, F_ReflectionType>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<__F_Type, F_ReflectionAlias>)
        {
            return true;
        }
        else if constexpr (std::is_const_v<__F_Type>)
        {
            return true;
        }
        else if constexpr (std::is_pointer_v<__F_Type>)
        {
            return true;
        }
        else if constexpr (std::is_rvalue_reference_v<__F_Type>)
        {
            return true;
        }
        else if constexpr (std::is_lvalue_reference_v<__F_Type>)
        {
            return true;
        }
        else if constexpr (IsReflectionTypeHandle<__F_Type>())
        {
            return true;
        }
        return false;
    }

    namespace Internal
    {
        template<typename __F_Reflector, typename = void>
        struct TH_ResolveSpecificDefaultReflector
        {
            using F = Internal::Reflection::TF_MakeItAbleToBePointer<__F_Reflector>;
        };
        template<typename __F_Reflector>
        struct TH_ResolveSpecificDefaultReflector<__F_Reflector, std::void_t<typename __F_Reflector::F_ManualReflector>>
        {
            using F = typename __F_Reflector::F_ManualReflector;
        };
    }
    template<typename __F_Type>
    using TF_DefaultReflector = std::conditional_t<
        !ShouldUseEmptyReflector<__F_Type>(),
        typename Internal::TH_ResolveSpecificDefaultReflector<__F_Type>::F,
        TF_EmptyReflector<__F_Type>
    >;
    
    template<typename __F_Type>
    struct TH_ResolveReflector
    {
        using F = TF_DefaultReflector<__F_Type>;
    };
    template<typename __F_Type>
    using TF_ResolveReflector = typename TH_ResolveReflector<__F_Type>::F;
    
    namespace Internal
    {
        template<typename __F_Reflector, typename = void>
        struct TH_ResolveDefaultReflectMode
        {
            static constexpr E_ReflectMode Value = E_ReflectMode::DEFAULT;
        };
        template<typename __F_Reflector>
        struct TH_ResolveDefaultReflectMode<__F_Reflector, std::void_t<decltype(__F_Reflector::DefaultReflectMode)>>
        {
            static constexpr E_ReflectMode Value = __F_Reflector::DefaultReflectMode;
        };
    }
    template<typename __F_Type>
    static constexpr E_ReflectMode ReflectorDefaultReflectMode = Internal::TH_ResolveDefaultReflectMode<
        TF_ResolveReflector<__F_Type>    
    >::Value;
}