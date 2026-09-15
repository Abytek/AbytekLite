#pragma once

#include "Abytek/ReflectionCommon.hpp"
#include "Abytek/ReflectionData.hpp"
#include "Abytek/ReflectionQueues.hpp"
#include "Abytek/ReflectorBase.hpp"
#include "Abytek/ReflectionContext.hpp"
#include "Abytek/ReflectionItem.hpp"
#include "Abytek/ReflectionSession.hpp"
#include "Abytek/ReflectionType.hpp"
#include "Abytek/ReflectionTypeTemplateImplementations.hpp"
#include "Abytek/ReflectionAlias.hpp"
#include "Abytek/ReflectionDataBuilder.hpp"
#include "Abytek/ReflectionGenBase.hpp"
#include "Abytek/ReflectionMacros_Base.hpp"
#include "Abytek/ReflectionMacros_Gen.hpp"
#include "Abytek/ReflectionMacros_Command.hpp"
#include "Abytek/SoftReflectionItem.hpp"
#include "Abytek/SoftReflectionType.hpp"

namespace Abytek
{
    template<typename __F_Type>
    struct TF_EmptyReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = (
            (std::is_const_v<__F_Type> || std::is_pointer_v<__F_Type> || std::is_reference_v<__F_Type> || IsReflectionTypeHandle<__F_Type>())
            ? E_ReflectMode::INLINE
            : E_ReflectMode::LINKED
        );
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_EmptyReflector, __F_Type);
    };
    template<typename __F_Type>
    ABYTEK_REFLECT(ABYTEK_MA(TF_EmptyReflector<__F_Type>))
    {
        if constexpr (std::is_same_v<__F_Type, B8>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::B8"));
        }
        else if constexpr (std::is_same_v<__F_Type, U8>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::U8"));
        }
        else if constexpr (std::is_same_v<__F_Type, U16>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::U16"));
        }
        else if constexpr (std::is_same_v<__F_Type, U32>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::U32"));
        }
        else if constexpr (std::is_same_v<__F_Type, U64>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::U64"));
        }
        else if constexpr (std::is_same_v<__F_Type, I8>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::I8"));
        }
        else if constexpr (std::is_same_v<__F_Type, I16>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::I16"));
        }
        else if constexpr (std::is_same_v<__F_Type, I32>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::I32"));
        }
        else if constexpr (std::is_same_v<__F_Type, I64>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::I64"));
        }
        else if constexpr (std::is_same_v<__F_Type, F_GeneralTypeHashCode>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_GeneralTypeHashCode"));
        }
        else if constexpr (std::is_same_v<__F_Type, F_Char>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_Char"));
        }
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        else if constexpr (std::is_same_v<__F_Type, F_TextChar>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_TextChar"));
        }
#endif
        else if constexpr (std::is_same_v<__F_Type, F32>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F32"));
        }
        else if constexpr (std::is_same_v<__F_Type, F64>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F64"));
        }
        else if constexpr (std::is_same_v<__F_Type, Sz>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::Sz"));
        }
        else if constexpr (std::is_same_v<__F_Type, PDiff>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::PDiff"));
        }
        else if constexpr (std::is_same_v<__F_Type, A_ReflectionItem>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_ReflectionItem"));
        }
        else if constexpr (std::is_same_v<__F_Type, F_ReflectionType>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ReflectionType"));
        }
        else if constexpr (std::is_same_v<__F_Type, F_ReflectionAlias>)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ReflectionAlias"));
        }
        else if constexpr (std::is_const_v<__F_Type>)
        {
            ABYTEK_REFLECT_COMMAND(
                OnReflectCanonicals,
                [=]()
                {
                    auto Payload = ReflectionType->ReflectReferenced<__F_Type>();
                    if (auto PayloadCanonical = Payload->GetCanonical())
                    {
                        ReflectionType->SetCanonical(*PayloadCanonical + ABYTEK_TEXT(" const"));
                    }
                }
            );
        }
        else if constexpr (std::is_pointer_v<__F_Type>)
        {
            ABYTEK_REFLECT_COMMAND(
                OnReflectCanonicals,
                [=]()
                {
                    auto Payload = ReflectionType->ReflectReferenced<__F_Type>();
                    if (auto PayloadCanonical = Payload->GetCanonical())
                    {
                        ReflectionType->SetCanonical(*PayloadCanonical + ABYTEK_TEXT("*"));
                    }
                }
            );
        }
        else if constexpr (std::is_rvalue_reference_v<__F_Type>)
        {
            ABYTEK_REFLECT_COMMAND(
                OnReflectCanonicals,
                [=]()
                {
                    auto Payload = ReflectionType->ReflectReferenced<__F_Type>();
                    if (auto PayloadCanonical = Payload->GetCanonical())
                    {
                        ReflectionType->SetCanonical(*PayloadCanonical + ABYTEK_TEXT("&&"));
                    }
                }
            );
        }
        else if constexpr (std::is_lvalue_reference_v<__F_Type>)
        {
            ABYTEK_REFLECT_COMMAND(
                OnReflectCanonicals,
                [=]()
                {
                    auto Payload = ReflectionType->ReflectReferenced<__F_Type>();
                    if (auto PayloadCanonical = Payload->GetCanonical())
                    {
                        ReflectionType->SetCanonical(*PayloadCanonical + ABYTEK_TEXT("&"));
                    }
                }
            );
        }
        else if constexpr (IsReflectionTypeHandle<__F_Type>())
        {
            ABYTEK_REFLECT_COMMAND(
                OnReflectCanonicals,
                [=]()
                {
                    auto Payload = ReflectionType->ReflectReferenced<__F_Type>();
                    if (auto PayloadCanonical = Payload->GetCanonical())
                    {
                        ReflectionType->SetCanonical(ABYTEK_TEXT("Abytek::TF_ReflectionTypeHandle<") + *PayloadCanonical + ABYTEK_TEXT(">"));
                    }
                }
            );
        }
        else
        {
            ABYTEK_LOG_FATAL() << TypeFullName<__F_Type>() << " is invalid to reflect, TF_EmptyReflector is for primitive/const/pointer/reference type only";
        }
        
        if constexpr (std::is_same_v<__F_Type, F_TextChar>)
        {
            ABYTEK_REFLECT_ALIAS(ABYTEK_NAME("Abytek::F_TextChar"));
        }
        if constexpr (std::is_same_v<__F_Type, Sz>)
        {
            ABYTEK_REFLECT_ALIAS(ABYTEK_NAME("Abytek::Sz"));
        }
        if constexpr (std::is_same_v<__F_Type, PDiff>)
        {
            ABYTEK_REFLECT_ALIAS(ABYTEK_NAME("Abytek::PDiff"));
        }
    }
    
    template<typename __F_PassedObject, class __F_ObjectPointerConfig>
    struct TF_TWReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_TWReflector, ObjectSmartPointerTemplates::TW<__F_PassedObject, __F_ObjectPointerConfig>);
    };
    template<typename __F_PassedObject, class __F_ObjectPointerConfig>
    ABYTEK_REFLECT(ABYTEK_MA(TF_TWReflector<__F_PassedObject, __F_ObjectPointerConfig>))
    {
        ReflectionType->ReflectReferenced<__F_PassedObject>();
    }
    template<typename __F_PassedObject, class __F_ObjectPointerConfig>
    struct TH_ResolveReflector<ObjectSmartPointerTemplates::TW<__F_PassedObject, __F_ObjectPointerConfig>>
    {
        using F = TF_TWReflector<__F_PassedObject, __F_ObjectPointerConfig>;
    };
    
    template<typename __F_PassedObject, class __F_Allocator, class __F_ObjectPointerConfig>
    struct TF_TUReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_TUReflector, ObjectSmartPointerTemplates::TU<__F_PassedObject, __F_Allocator, __F_ObjectPointerConfig>);
    };
    template<typename __F_PassedObject, class __F_Allocator, class __F_ObjectPointerConfig>
    ABYTEK_REFLECT(ABYTEK_MA(TF_TUReflector<__F_PassedObject, __F_Allocator, __F_ObjectPointerConfig>))
    {
        ReflectionType->ReflectReferenced<__F_PassedObject>();
    }
    template<typename __F_PassedObject, class __F_Allocator, class __F_ObjectPointerConfig>
    struct TH_ResolveReflector<ObjectSmartPointerTemplates::TU<__F_PassedObject, __F_Allocator, __F_ObjectPointerConfig>>
    {
        using F = TF_TUReflector<__F_PassedObject, __F_Allocator, __F_ObjectPointerConfig>;
    };
    
    template<typename __F_PassedObject, class __F_Allocator, class __F_ObjectPointerConfig>
    struct TF_TSReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_TSReflector, ObjectSmartPointerTemplates::TS<__F_PassedObject, __F_Allocator, __F_ObjectPointerConfig>);
    };
    template<typename __F_PassedObject, class __F_Allocator, class __F_ObjectPointerConfig>
    ABYTEK_REFLECT(ABYTEK_MA(TF_TSReflector<__F_PassedObject, __F_Allocator, __F_ObjectPointerConfig>))
    {
        ReflectionType->ReflectReferenced<__F_PassedObject>();
    }
    template<typename __F_PassedObject, class __F_Allocator, class __F_ObjectPointerConfig>
    struct TH_ResolveReflector<ObjectSmartPointerTemplates::TS<__F_PassedObject, __F_Allocator, __F_ObjectPointerConfig>>
    {
        using F = TF_TSReflector<__F_PassedObject, __F_Allocator, __F_ObjectPointerConfig>;
    };

    namespace Internal
    {
        template<typename __F_Functor>
        struct TH_RawFunctionReflectDependencies
        {
            static void Invoke(const TW_Valid<F_ReflectionSession>& ReflectionSession, const TW_Valid<F_ReflectionType>& ReflectionType)
            {
            }
        };
        template<typename __F_Return, typename... __F_Args>
        struct TH_RawFunctionReflectDependencies<__F_Return(__F_Args...)>
        {
            static void Invoke(const TW_Valid<F_ReflectionSession>& ReflectionSession, const TW_Valid<F_ReflectionType>& ReflectionType)
            {
                ReflectionType->ReflectReferenced<__F_Return>();
                TW<F_ReflectionType> DependentReflectionTypeHandles[] = { TW<F_ReflectionType> {}, ReflectionType->ReflectReferenced<__F_Args>()... };
            }
        };
    }
    template<typename __F_Functor>
    struct TF_RawFunctionReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_RawFunctionReflector, __F_Functor);
    };
    template<typename __F_Functor>
    ABYTEK_REFLECT(ABYTEK_MA(TF_RawFunctionReflector<__F_Functor>))
    {
        Internal::TH_RawFunctionReflectDependencies<__F_Functor>::Invoke(ReflectionSession, ReflectionType);
    }
    template<typename __F_Return, typename... __F_Args>
    struct TH_ResolveReflector<__F_Return(__F_Args...)>
    {
        using F = TF_RawFunctionReflector<__F_Return(__F_Args...)>;
    };
    
    template<class __F_StringDatabase>
    struct TF_StringIdReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_StringIdReflector, TF_StringId<__F_StringDatabase>);
    };
    template<class __F_StringDatabase>
    ABYTEK_REFLECT(ABYTEK_MA(TF_StringIdReflector<__F_StringDatabase>))
    {
    }
    template<class __F_StringDatabase>
    struct TH_ResolveReflector<TF_StringId<__F_StringDatabase>>
    {
        using F = TF_StringIdReflector<__F_StringDatabase>;
    };
}

#include "Abytek/CoreContainerReflectors.hpp"