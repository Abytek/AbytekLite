#pragma once

#include "Abytek/ReflectionCommon.hpp"


namespace Abytek
{
    class F_ReflectionContext;
    class F_ReflectionType;
    
    struct H_ReflectGenBase
    {
        static constexpr B8 RequireStatic_Interface = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector>
        static void Interface(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType
        )
        {
        }
        static constexpr B8 RequireStatic_Private = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector>
        static void Private(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType
        )
        {
        }
        static constexpr B8 RequireStatic_Public = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector>
        static void Public(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType
        )
        {
        }
        
        static constexpr B8 RequireStatic_Method_Interface = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member, auto __MemberP>
        static void Method_Interface(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionMethod& ReflectionMethod
        )
        {
        }
        static constexpr B8 RequireStatic_Method_Private = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member, auto __MemberP>
        static void Method_Private(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionMethod& ReflectionMethod
        )
        {
        }
        static constexpr B8 RequireStatic_Method_Public = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member, auto __MemberP>
        static void Method_Public(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionMethod& ReflectionMethod
        )
        {
        }
        
        static constexpr B8 RequireStatic_MethodStatic_Interface = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member>
        static void MethodStatic_Interface(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionMethod& ReflectionMethod
        )
        {
        }
        static constexpr B8 RequireStatic_MethodStatic_Private = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member>
        static void MethodStatic_Private(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionMethod& ReflectionMethod
        )
        {
        }
        static constexpr B8 RequireStatic_MethodStatic_Public = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member>
        static void MethodStatic_Public(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionMethod& ReflectionMethod
        )
        {
        }
        
        static constexpr B8 RequireStatic_Property_Interface = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member, Sz __MemberOffset>
        static void Property_Interface(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionProperty& ReflectionProperty
        )
        {
        }
        static constexpr B8 RequireStatic_Property_Private = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member, Sz __MemberOffset>
        static void Property_Private(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionProperty& ReflectionProperty
        )
        {
        }
        static constexpr B8 RequireStatic_Property_Public = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member, Sz __MemberOffset>
        static void Property_Public(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionProperty& ReflectionProperty
        )
        {
        }
        
        static constexpr B8 RequireStatic_PropertyStatic_Interface = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member>
        static void PropertyStatic_Interface(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionProperty& ReflectionProperty
        )
        {
        }
        static constexpr B8 RequireStatic_PropertyStatic_Private = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member>
        static void PropertyStatic_Private(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionProperty& ReflectionProperty
        )
        {
        }
        static constexpr B8 RequireStatic_PropertyStatic_Public = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector, typename __F_Member>
        static void PropertyStatic_Public(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionProperty& ReflectionProperty
        )
        {
        }
        
        static constexpr B8 RequireStatic_EnumValue_Interface = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector>
        static void EnumValue_Interface(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionEnumValue& ReflectionEnumValue
        )
        {
        }
        static constexpr B8 RequireStatic_EnumValue_Private = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector>
        static void EnumValue_Private(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionEnumValue& ReflectionEnumValue
        )
        {
        }
        static constexpr B8 RequireStatic_EnumValue_Public = false;
        template<B8 __IsSelf, typename __F_SelfOrInherited, typename __F_Reflector>
        static void EnumValue_Public(
            const TW_Valid<F_ReflectionSession>& ReflectionSession,
            const TW_Valid<F_ReflectionType>& ReflectionType,
            F_ReflectionEnumValue& ReflectionEnumValue
        )
        {
        }
    };
}