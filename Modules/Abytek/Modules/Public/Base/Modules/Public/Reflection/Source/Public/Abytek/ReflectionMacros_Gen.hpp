#pragma once

#include "Abytek/ReflectionCommon.hpp"


#define ABYTEK_REFLECT_GEN_INTERFACE() \
                static constexpr B8 RequireStatic_Interface = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector> \
                static void Interface( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType \
                )
#define ABYTEK_REFLECT_GEN_PRIVATE() \
                static constexpr B8 RequireStatic_Private = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector> \
                static void Private( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType \
                )
#define ABYTEK_REFLECT_GEN_PUBLIC() \
                static constexpr B8 RequireStatic_Public = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector> \
                static void Public( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType \
                )

#define ABYTEK_REFLECT_GEN_METHOD_INTERFACE() \
                static constexpr B8 RequireStatic_Method_Interface = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member, auto __MemberP> \
                static void Method_Interface( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                )
#define ABYTEK_REFLECT_GEN_METHOD_PRIVATE() \
                static constexpr B8 RequireStatic_Method_Private = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member, auto __MemberP> \
                static void Method_Private( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                )
#define ABYTEK_REFLECT_GEN_METHOD_PUBLIC() \
                static constexpr B8 RequireStatic_Method_Public = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member, auto __MemberP> \
                static void Method_Public( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                )

#define ABYTEK_REFLECT_GEN_METHOD_STATIC_INTERFACE() \
                static constexpr B8 RequireStatic_MethodStatic_Interface = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member> \
                static void MethodStatic_Interface( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                )
#define ABYTEK_REFLECT_GEN_METHOD_STATIC_PRIVATE() \
                static constexpr B8 RequireStatic_MethodStatic_Private = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member> \
                static void MethodStatic_Private( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                )
#define ABYTEK_REFLECT_GEN_METHOD_STATIC_PUBLIC() \
                static constexpr B8 RequireStatic_MethodStatic_Public = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member> \
                static void MethodStatic_Public( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionMethod& ReflectionMethod \
                )

#define ABYTEK_REFLECT_GEN_PROPERTY_INTERFACE() \
                static constexpr B8 RequireStatic_Property_Interface = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member, Sz __MemberOfset> \
                static void Property_Interface( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                )
#define ABYTEK_REFLECT_GEN_PROPERTY_PRIVATE() \
                static constexpr B8 RequireStatic_Property_Private = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member, Sz __MemberOfset> \
                static void Property_Private( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                )
#define ABYTEK_REFLECT_GEN_PROPERTY_PUBLIC() \
                static constexpr B8 RequireStatic_Property_Public = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member, Sz __MemberOfset> \
                static void Property_Public( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                )

#define ABYTEK_REFLECT_GEN_PROPERTY_STATIC_INTERFACE() \
                static constexpr B8 RequireStatic_PropertyStatic_Interface = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member> \
                static void PropertyStatic_Interface( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                )
#define ABYTEK_REFLECT_GEN_PROPERTY_STATIC_PRIVATE() \
                static constexpr B8 RequireStatic_PropertyStatic_Private = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member> \
                static void PropertyStatic_Private( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                )
#define ABYTEK_REFLECT_GEN_PROPERTY_STATIC_PUBLIC() \
                static constexpr B8 RequireStatic_PropertyStatic_Public = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member> \
                static void PropertyStatic_Public( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionProperty& ReflectionProperty \
                )

#define ABYTEK_REFLECT_GEN_ENUM_VALUE_INTERFACE() \
                static constexpr B8 RequireStatic_EnumValue_Interface = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector> \
                static void EnumValue_Interface( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionEnumValue& ReflectionEnumValue \
                )
#define ABYTEK_REFLECT_GEN_ENUM_VALUE_PRIVATE() \
                static constexpr B8 RequireStatic_EnumValue_Private = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector> \
                static void EnumValue_Private( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionEnumValue& ReflectionEnumValue \
                )
#define ABYTEK_REFLECT_GEN_ENUM_VALUE_PUBLIC() \
                static constexpr B8 RequireStatic_EnumValue_Public = true; \
                template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector> \
                static void EnumValue_Public( \
                    const Abytek::TW_Valid<Abytek::F_ReflectionSession>& ReflectionSession, \
                    const Abytek::TW_Valid<Abytek::F_ReflectionType>& ReflectionType, \
                    Abytek::F_ReflectionEnumValue& ReflectionEnumValue \
                )