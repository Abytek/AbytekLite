#pragma once

#include "Abytek/ReflectionCommon.hpp"


#define ABYTEK_IMPLEMENT_REFLECT(Reflector, ...) \
            static inline void ReflectType( \
                const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType \
            ) { \
                ReflectBaseTypes<Reflector>(ReflectionSession, ReflectionType); \
                OnReflect(ReflectionSession, ReflectionType); \
                ReflectGen_Type_Private<F_Reflected, F_Reflector>(ReflectionSession, ReflectionType); \
            }

#define ABYTEK_DECLARE_ON_REFLECT(Reflector) \
                static void OnReflect( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType \
                )

/**
 * @brief Implements the OnReflect method for a reflector
 * 
 * This macro is used to implement the OnReflect method that was declared with ABYTEK_DECLARE_ON_REFLECT.
 * It provides the reflection implementation for a specific type.
 * 
 * @param ... The reflector class to implement OnReflect for
 */
#define ABYTEK_REFLECT(...) void __VA_ARGS__::OnReflect( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType \
                )

#define ABYTEK_REFLECT_INLINE(...) inline ABYTEK_REFLECT(__VA_ARGS__)

#define ABYTEK_IMPLEMENT_REFLECTOR_ADVANCED(Reflector, ...) \
            ABYTEK_PUBLIC_KEYWORD \
                using F_Reflector = Reflector; \
                using F_Reflected = __VA_ARGS__; \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector> \
                static void ReflectGen_Type_PublicRecursive( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType \
                ) \
                { \
                    ReflectGen_Type_Bases<__F_SelfOrInherited, __F_IndirectReflector>(ReflectionSession, ReflectionType); \
                    H_ReflectGen::template Public<true, __F_SelfOrInherited, __F_IndirectReflector>(ReflectionSession, ReflectionType); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector> \
                static void ReflectGen_Type_Interface( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType \
                ) \
                { \
                    H_ReflectGen::template Interface<true, __F_SelfOrInherited, __F_IndirectReflector>(ReflectionSession, ReflectionType); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector> \
                static void ReflectGen_Type_Private( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType \
                ) \
                { \
                    ReflectGen_Type_PublicRecursive<__F_SelfOrInherited, __F_IndirectReflector>(ReflectionSession, ReflectionType); \
                    H_ReflectGen::template Private<true, __F_SelfOrInherited, __F_IndirectReflector>(ReflectionSession, ReflectionType); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member, auto __MemberP> \
                static void ReflectGen_Method_PublicRecursive( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                ) \
                { \
                    ReflectGen_Method_Bases<__F_SelfOrInherited, __F_IndirectReflector, __F_Member, __MemberP>(ReflectionSession, ReflectionType, ReflectionMethod); \
                    H_ReflectGen::template Method_Public<true, __F_SelfOrInherited, __F_IndirectReflector, __F_Member, __MemberP>(ReflectionSession, ReflectionType, ReflectionMethod); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member, auto __MemberP> \
                static void ReflectGen_Method_Interface( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                ) \
                { \
                    H_ReflectGen::template Method_Interface<true, __F_SelfOrInherited, __F_IndirectReflector, __F_Member, __MemberP>(ReflectionSession, ReflectionType, ReflectionMethod); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member, auto __MemberP> \
                static void ReflectGen_Method_Private( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                ) \
                { \
                    ReflectGen_Method_PublicRecursive<__F_SelfOrInherited, __F_IndirectReflector, __F_Member, __MemberP>(ReflectionSession, ReflectionType, ReflectionMethod); \
                    H_ReflectGen::template Method_Private<true, __F_SelfOrInherited, __F_IndirectReflector, __F_Member, __MemberP>(ReflectionSession, ReflectionType, ReflectionMethod); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member> \
                static void ReflectGen_MethodStatic_PublicRecursive( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                ) \
                { \
                    ReflectGen_MethodStatic_Bases<__F_SelfOrInherited, __F_IndirectReflector, __F_Member>(ReflectionSession, ReflectionType, ReflectionMethod); \
                    H_ReflectGen::template MethodStatic_Public<true, __F_SelfOrInherited, __F_IndirectReflector, __F_Member>(ReflectionSession, ReflectionType, ReflectionMethod); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member> \
                static void ReflectGen_MethodStatic_Interface( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                ) \
                { \
                    H_ReflectGen::template MethodStatic_Interface<true, __F_SelfOrInherited, __F_IndirectReflector, __F_Member>(ReflectionSession, ReflectionType, ReflectionMethod); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member> \
                static void ReflectGen_MethodStatic_Private( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                ) \
                { \
                    ReflectGen_MethodStatic_PublicRecursive<__F_SelfOrInherited, __F_IndirectReflector, __F_Member>(ReflectionSession, ReflectionType, ReflectionMethod); \
                    H_ReflectGen::template MethodStatic_Private<true, __F_SelfOrInherited, __F_IndirectReflector, __F_Member>(ReflectionSession, ReflectionType, ReflectionMethod); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member, Sz __MemberOfset> \
                static void ReflectGen_Property_PublicRecursive( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                ) \
                { \
                    ReflectGen_Property_Bases<__F_SelfOrInherited, __F_IndirectReflector, __F_Member, __MemberOfset>(ReflectionSession, ReflectionType, ReflectionProperty); \
                    H_ReflectGen::template Property_Public<true, __F_SelfOrInherited, __F_IndirectReflector, __F_Member, __MemberOfset>(ReflectionSession, ReflectionType, ReflectionProperty); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member, Sz __MemberOfset> \
                static void ReflectGen_Property_Interface( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                ) \
                { \
                    H_ReflectGen::template Property_Interface<true, __F_SelfOrInherited, __F_IndirectReflector, __F_Member, __MemberOfset>(ReflectionSession, ReflectionType, ReflectionProperty); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member, auto __MemberOfset> \
                static void ReflectGen_Property_Private( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                ) \
                { \
                    ReflectGen_Property_PublicRecursive<__F_SelfOrInherited, __F_IndirectReflector, __F_Member, __MemberOfset>(ReflectionSession, ReflectionType, ReflectionProperty); \
                    H_ReflectGen::template Property_Private<true, __F_SelfOrInherited, __F_IndirectReflector, __F_Member, __MemberOfset>(ReflectionSession, ReflectionType, ReflectionProperty); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member> \
                static void ReflectGen_PropertyStatic_PublicRecursive( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                ) \
                { \
                    ReflectGen_PropertyStatic_Bases<__F_SelfOrInherited, __F_IndirectReflector, __F_Member>(ReflectionSession, ReflectionType, ReflectionProperty); \
                    H_ReflectGen::template PropertyStatic_Public<true, __F_SelfOrInherited, __F_IndirectReflector, __F_Member>(ReflectionSession, ReflectionType, ReflectionProperty); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member> \
                static void ReflectGen_PropertyStatic_Interface( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                ) \
                { \
                    H_ReflectGen::template PropertyStatic_Interface<true, __F_SelfOrInherited, __F_IndirectReflector, __F_Member>(ReflectionSession, ReflectionType, ReflectionProperty); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member> \
                static void ReflectGen_PropertyStatic_Private( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                ) \
                { \
                    ReflectGen_PropertyStatic_PublicRecursive<__F_SelfOrInherited, __F_IndirectReflector, __F_Member>(ReflectionSession, ReflectionType, ReflectionProperty); \
                    H_ReflectGen::template PropertyStatic_Private<true, __F_SelfOrInherited, __F_IndirectReflector, __F_Member>(ReflectionSession, ReflectionType, ReflectionProperty); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector> \
                static void ReflectGen_EnumValue_PublicRecursive( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionEnumValue& ReflectionEnumValue \
                ) \
                { \
                    ReflectGen_EnumValue_Bases<__F_SelfOrInherited, __F_IndirectReflector>(ReflectionSession, ReflectionType, ReflectionEnumValue); \
                    H_ReflectGen::template EnumValue_Public<true, __F_SelfOrInherited, __F_IndirectReflector>(ReflectionSession, ReflectionType, ReflectionEnumValue); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector> \
                static void ReflectGen_EnumValue_Interface( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionEnumValue& ReflectionEnumValue \
                ) \
                { \
                    H_ReflectGen::template EnumValue_Interface<true, __F_SelfOrInherited, __F_IndirectReflector>(ReflectionSession, ReflectionType, ReflectionEnumValue); \
                } \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector> \
                static void ReflectGen_EnumValue_Private( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionEnumValue& ReflectionEnumValue \
                ) \
                { \
                    ReflectGen_EnumValue_PublicRecursive<__F_SelfOrInherited, __F_IndirectReflector>(ReflectionSession, ReflectionType, ReflectionEnumValue); \
                    H_ReflectGen::template EnumValue_Private<true, __F_SelfOrInherited, __F_IndirectReflector>(ReflectionSession, ReflectionType, ReflectionEnumValue); \
                } \
                ABYTEK_EXPAND(ABYTEK_IMPLEMENT_REFLECT(Reflector, __VA_ARGS__)) \
                ABYTEK_EXPAND(ABYTEK_DECLARE_ON_REFLECT(Reflector))

namespace Abytek
{
    namespace Internal::IsOneOfReflectionBase
    {
        template<typename... Ts>
        struct TH_Checker;
        
        template<typename T, typename... Ts>
        struct TH : std::false_type {};

        template<typename T, typename U, typename... Ts>
        struct TH<T, U, Ts...>
            : std::conditional_t<
                std::is_same_v<T, U> || U::template HasReflectionBase<T>(),
                std::true_type,
                TH<T, Ts...>>
        {};
        
        template<typename... Ts>
        struct TH_Checker
        {
            template<typename T>
            static constexpr B8 Check()
            {
                return TH<T, Ts...>::value;
            }
        };
    }
    
    namespace Internal
    {
        template<typename T, typename = void>
        struct TH_IsReflectionBaseOf : std::false_type
        {
        };
        template<typename TBase, typename TChild>
        struct TH_IsReflectionBaseOf<
            std::pair<TBase, TChild>,
            std::void_t<
                decltype(TChild::template HasReflectionBase<TBase>())
            >
        > : std::bool_constant<
                TChild::template HasReflectionBase<TBase>()
            >
        {
        };
    }
    template<typename TBase, typename TChild>
    static constexpr bool IsReflectionBaseOf()
    {
        return Internal::TH_IsReflectionBaseOf<
            std::pair<TF_ResolveReflector<TBase>, TF_ResolveReflector<TChild>>
        >::value;
    }
}

/**
 * @brief Begin the definition of a reflector
 * 
 * This macro starts the declaration of a reflector by setting up the base type reflection.
 * 
 * @param ... Base reflector types that this reflector inherits from
 */
#define ABYTEK_BEGIN_REFLECTOR(...) \
            ABYTEK_PUBLIC_KEYWORD \
                template<typename __F> \
                static constexpr bool HasReflectionBase() \
                { \
                    return Abytek::Internal::IsOneOfReflectionBase::TH_Checker<__VA_ARGS__>::Check<__F>(); \
                } \
            ABYTEK_PRIVATE_KEYWORD \
                template<typename __F_IndirectReflector> \
                struct TH_ReflectBaseTypes \
                { \
                    template<typename __F_BaseReflector> \
                    static int InvokePerBase(const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType) \
                    { \
                        using F_Base = typename __F_BaseReflector::F_Reflected; \
                        static_assert(!std::is_same_v<F_Base, F_Reflected>, "Invalid base type to reflect"); \
                        Abytek::F_ReflectionTypeBase Base; \
                        Base.Type = ReflectionType->ReflectReferenced<F_Base>(); \
                        Base.CastFunction = Abytek::Internal::Reflection::TH_MakeCastFunction<F_Reflected, F_Base>::Make(); \
                        ReflectionType->AdditionalInfo.RequireStatic = ReflectionType->AdditionalInfo.RequireStatic || Base.Type->RequireStatic(); \
                        ReflectionType->AdditionalInfo.Bases.push_back(Base); \
                        ABYTEK_BASE_REFLECTION_ASSERT(Base.Type->HasBase(ReflectionType) != ReflectionType->HasBase(Base.Type)) \
                            << "Recursive type inheritance: " << Abytek::F_NewLine {} \
                            << "\t- Base: " << Abytek::TypeFullName<F_Base>() << Abytek::F_NewLine {} \
                            << "\t- Type: " << ReflectionType->GetFullName(); \
                        return 0; \
                    } \
                    template<typename... __F_BaseReflectors> \
                    static void Invoke(const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType) \
                    { \
                        int ___[] = { 0, InvokePerBase<__F_BaseReflectors>(ReflectionSession, ReflectionType)... }; \
                    } \
                }; \
                template<typename __F_IndirectReflector> \
                static void ReflectBaseTypes(const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType) \
                { \
                    TH_ReflectBaseTypes<__F_IndirectReflector>::template Invoke<__VA_ARGS__>(ReflectionSession, ReflectionType); \
                } \
            ABYTEK_PRIVATE_KEYWORD \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector> \
                struct TH_ReflectGen_Type_Bases \
                { \
                    template<typename __F_BaseReflector> \
                    static int InvokePerBase(const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType) \
                    { \
                        __F_BaseReflector::template ReflectGen_Type_PublicRecursive<__F_SelfOrInherited, __F_IndirectReflector>(ReflectionSession, ReflectionType); \
                        __F_BaseReflector::template ReflectGen_Type_Interface<__F_SelfOrInherited, __F_IndirectReflector>(ReflectionSession, ReflectionType); \
                        return 0; \
                    } \
                    template<typename... __F_BaseReflectors> \
                    static void Invoke(const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType) \
                    { \
                        int ___[] = { 0, InvokePerBase<__F_BaseReflectors>(ReflectionSession, ReflectionType)... }; \
                    } \
                }; \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector> \
                static void ReflectGen_Type_Bases(const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType) \
                { \
                    TH_ReflectGen_Type_Bases<__F_SelfOrInherited, __F_IndirectReflector>::template Invoke<__VA_ARGS__>(ReflectionSession, ReflectionType); \
                } \
            ABYTEK_PRIVATE_KEYWORD \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member, auto __MemberP> \
                struct TH_ReflectGen_Method_Bases \
                { \
                    template<typename __F_BaseReflector> \
                    static int InvokePerBase( \
                        const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                        const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                        Abytek::F_ReflectionMethod& ReflectionMethod \
                    ) \
                    { \
                        __F_BaseReflector::template ReflectGen_Method_PublicRecursive< \
                            __F_SelfOrInherited, \
                            __F_IndirectReflector, \
                            __F_Member, \
                            __MemberP \
                        >(ReflectionSession, ReflectionType, ReflectionMethod); \
                        __F_BaseReflector::template ReflectGen_Method_Interface< \
                            __F_SelfOrInherited, \
                            __F_IndirectReflector, \
                            __F_Member, \
                            __MemberP \
                        >(ReflectionSession, ReflectionType, ReflectionMethod); \
                        return 0; \
                    } \
                    template<typename... __F_BaseReflectors> \
                    static void Invoke( \
                        const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                        const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                        Abytek::F_ReflectionMethod& ReflectionMethod \
                    ) \
                    { \
                        int ___[] = { 0, InvokePerBase<__F_BaseReflectors>(ReflectionSession, ReflectionType, ReflectionMethod)... }; \
                    } \
                }; \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member, auto __MemberP> \
                static void ReflectGen_Method_Bases( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                ) \
                { \
                    TH_ReflectGen_Method_Bases<__F_SelfOrInherited, __F_IndirectReflector, __F_Member, __MemberP>::template Invoke<__VA_ARGS__>(ReflectionSession, ReflectionType, ReflectionMethod); \
                } \
            ABYTEK_PRIVATE_KEYWORD \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member> \
                struct TH_ReflectGen_MethodStatic_Bases \
                { \
                    template<typename __F_BaseReflector> \
                    static int InvokePerBase( \
                        const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                        const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                        Abytek::F_ReflectionMethod& ReflectionMethod \
                    ) \
                    { \
                        __F_BaseReflector::template ReflectGen_MethodStatic_PublicRecursive< \
                            __F_SelfOrInherited, \
                            __F_IndirectReflector, \
                            __F_Member \
                        >(ReflectionSession, ReflectionType, ReflectionMethod); \
                        __F_BaseReflector::template ReflectGen_MethodStatic_Interface< \
                            __F_SelfOrInherited, \
                            __F_IndirectReflector, \
                            __F_Member \
                        >(ReflectionSession, ReflectionType, ReflectionMethod); \
                        return 0; \
                    } \
                    template<typename... __F_BaseReflectors> \
                    static void Invoke( \
                        const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                        const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                        Abytek::F_ReflectionMethod& ReflectionMethod \
                    ) \
                    { \
                        int ___[] = { 0, InvokePerBase<__F_BaseReflectors>(ReflectionSession, ReflectionType, ReflectionMethod)... }; \
                    } \
                }; \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member> \
                static void ReflectGen_MethodStatic_Bases( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                ) \
                { \
                    TH_ReflectGen_MethodStatic_Bases<__F_SelfOrInherited, __F_IndirectReflector, __F_Member>::template Invoke<__VA_ARGS__>(ReflectionSession, ReflectionType, ReflectionMethod); \
                } \
            ABYTEK_PRIVATE_KEYWORD \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member, Sz __MemberOfset> \
                struct TH_ReflectGen_Property_Bases \
                { \
                    template<typename __F_BaseReflector> \
                    static int InvokePerBase( \
                        const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                        const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                        Abytek::F_ReflectionProperty& ReflectionProperty \
                    ) \
                    { \
                        __F_BaseReflector::template ReflectGen_Property_PublicRecursive< \
                            __F_SelfOrInherited, \
                            __F_IndirectReflector, \
                            __F_Member, \
                            __MemberOfset \
                        >(ReflectionSession, ReflectionType, ReflectionProperty); \
                        __F_BaseReflector::template ReflectGen_Property_Interface< \
                            __F_SelfOrInherited, \
                            __F_IndirectReflector, \
                            __F_Member, \
                            __MemberOfset \
                        >(ReflectionSession, ReflectionType, ReflectionProperty); \
                        return 0; \
                    } \
                    template<typename... __F_BaseReflectors> \
                    static void Invoke( \
                        const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                        const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                        Abytek::F_ReflectionProperty& ReflectionProperty \
                    ) \
                    { \
                        int ___[] = { 0, InvokePerBase<__F_BaseReflectors>(ReflectionSession, ReflectionType, ReflectionProperty)... }; \
                    } \
                }; \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member, Sz __MemberOfset> \
                static void ReflectGen_Property_Bases( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                ) \
                { \
                    TH_ReflectGen_Property_Bases<__F_SelfOrInherited, __F_IndirectReflector, __F_Member, __MemberOfset>::template Invoke<__VA_ARGS__>(ReflectionSession, ReflectionType, ReflectionProperty); \
                } \
            ABYTEK_PRIVATE_KEYWORD \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member> \
                struct TH_ReflectGen_PropertyStatic_Bases \
                { \
                    template<typename __F_BaseReflector> \
                    static int InvokePerBase( \
                        const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                        const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                        Abytek::F_ReflectionProperty& ReflectionProperty \
                    ) \
                    { \
                        __F_BaseReflector::template ReflectGen_PropertyStatic_PublicRecursive< \
                            __F_SelfOrInherited, \
                            __F_IndirectReflector, \
                            __F_Member \
                        >(ReflectionSession, ReflectionType, ReflectionProperty); \
                        __F_BaseReflector::template ReflectGen_PropertyStatic_Interface< \
                            __F_SelfOrInherited, \
                            __F_IndirectReflector, \
                            __F_Member \
                        >(ReflectionSession, ReflectionType, ReflectionProperty); \
                        return 0; \
                    } \
                    template<typename... __F_BaseReflectors> \
                    static void Invoke( \
                        const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                        const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                        Abytek::F_ReflectionProperty& ReflectionProperty \
                    ) \
                    { \
                        int ___[] = { 0, InvokePerBase<__F_BaseReflectors>(ReflectionSession, ReflectionType, ReflectionProperty)... }; \
                    } \
                }; \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector, typename __F_Member> \
                static void ReflectGen_PropertyStatic_Bases( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                ) \
                { \
                    TH_ReflectGen_PropertyStatic_Bases<__F_SelfOrInherited, __F_IndirectReflector, __F_Member>::template Invoke<__VA_ARGS__>(ReflectionSession, ReflectionType, ReflectionProperty); \
                } \
            ABYTEK_PRIVATE_KEYWORD \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector> \
                struct TH_ReflectGen_EnumValue_Bases \
                { \
                    template<typename __F_BaseReflector> \
                    static int InvokePerBase( \
                        const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                        const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                        Abytek::F_ReflectionEnumValue& ReflectionEnumValue \
                    ) \
                    { \
                        __F_BaseReflector::template ReflectGen_EnumValue_PublicRecursive< \
                            __F_SelfOrInherited, \
                            __F_IndirectReflector \
                        >(ReflectionSession, ReflectionType, ReflectionEnumValue); \
                        __F_BaseReflector::template ReflectGen_EnumValue_Interface< \
                            __F_SelfOrInherited, \
                            __F_IndirectReflector \
                        >(ReflectionSession, ReflectionType, ReflectionEnumValue); \
                        return 0; \
                    } \
                    template<typename... __F_BaseReflectors> \
                    static void Invoke( \
                        const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                        const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                        Abytek::F_ReflectionEnumValue& ReflectionEnumValue \
                    ) \
                    { \
                        int ___[] = { 0, InvokePerBase<__F_BaseReflectors>(ReflectionSession, ReflectionType, ReflectionEnumValue)... }; \
                    } \
                }; \
                template<typename __F_SelfOrInherited, typename __F_IndirectReflector> \
                static void ReflectGen_EnumValue_Bases( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionEnumValue& ReflectionEnumValue \
                ) \
                { \
                    TH_ReflectGen_EnumValue_Bases<__F_SelfOrInherited, __F_IndirectReflector>::template Invoke<__VA_ARGS__>(ReflectionSession, ReflectionType, ReflectionEnumValue); \
                } \
            ABYTEK_PUBLIC_KEYWORD \
                struct H_ReflectGen : Abytek::H_ReflectGenBase \
                {

/**
 * @brief End reflector definition with advanced configuration
 * 
 * This macro completes the reflector definition and implements the required reflection methods
 * with specific reflector and target type.
 * 
 * @param Reflector The reflector class
 * @param ... The target type(s) to reflect
 */
#define ABYTEK_END_REFLECTOR_ADVANCED(Reflector, ...) \
            }; \
            ABYTEK_EXPAND(ABYTEK_IMPLEMENT_REFLECTOR_ADVANCED(Reflector, __VA_ARGS__))

/**
 * @brief End reflector definition
 * 
 * This macro completes the reflector definition using the same class as both
 * the reflector and the target type.
 * 
 * @param ... The reflector class, which is also the target type
 */
#define ABYTEK_END_REFLECTOR(...) ABYTEK_EXPAND(ABYTEK_END_REFLECTOR_ADVANCED(__VA_ARGS__, __VA_ARGS__))


#define ABYTEK_DECLARE_ENUM_REFLECTOR_LOCALNS(Enum, ...) \
            template<> \
            struct TH_ResolveReflector<Enum> \
            { \
                struct F_Base \
                { \
                    static constexpr Abytek::E_ReflectMode DefaultReflectMode = Abytek::E_ReflectMode::INLINE; \
                     \
                    ABYTEK_BEGIN_REFLECTOR(__VA_ARGS__) \
                    ABYTEK_END_REFLECTOR(F_Base); \
                }; \
                struct F \
                { \
                    ABYTEK_BEGIN_REFLECTOR(Abytek::TF_ResolveReflector<Abytek::TF_UInt<sizeof(Enum)>>, F_Base) \
                    ABYTEK_END_REFLECTOR_ADVANCED(F, Enum); \
                }; \
            };
#define ABYTEK_DECLARE_ENUM_REFLECTOR(Enum, ...) \
            namespace Abytek \
            { \
                ABYTEK_DECLARE_ENUM_REFLECTOR_LOCALNS(Enum, __VA_ARGS__) \
            }

#define ABYTEK_ENUM_REFLECTOR_LOCALNS(Enum, ...) \
            ABYTEK_DECLARE_ENUM_REFLECTOR_LOCALNS(Enum, __VA_ARGS__) \
            ABYTEK_REFLECT_INLINE(Abytek::TH_ResolveReflector<Enum>::F_Base) {} \
            ABYTEK_REFLECT_INLINE(Abytek::TH_ResolveReflector<Enum>::F)
#define ABYTEK_ENUM_REFLECTOR(Enum, ...) \
            ABYTEK_DECLARE_ENUM_REFLECTOR(Enum, __VA_ARGS__) \
            ABYTEK_REFLECT_INLINE(Abytek::TH_ResolveReflector<Enum>::F_Base) {} \
            ABYTEK_REFLECT_INLINE(Abytek::TH_ResolveReflector<Enum>::F)
