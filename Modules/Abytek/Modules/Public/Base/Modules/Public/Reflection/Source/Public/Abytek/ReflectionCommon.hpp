#pragma once

#include "Abytek/Base.Reflection.prerequisites.pch.hpp"
#include "Abytek/CrtUseObject.hpp"
#include "Abytek/GeneralTypeHashCode.hpp"


namespace Abytek
{
    class F_ReflectionSession;
    class F_ReflectionContext;
    class F_ReflectionType;
    
    namespace Internal::Reflection
    {
        struct F_Fake {};
        template<typename __F>
        using TF_NonReference = std::conditional_t<
            std::is_reference_v<__F>,
            F_Fake,
            __F
        >;
        template<typename __F>
        using TF_NonFunction = std::conditional_t<
            std::is_function_v<__F>,
            F_Fake,
            __F
        >;
        template<typename __F>
        using TF_MakeItAbleToBePointer = std::conditional_t<
            std::is_function_v<__F>,
            F_Fake,
            TF_NonReference<__F>
        >;
        
        template <
            typename From,
            typename To,
            typename = void
        >
        struct TH_MakeCastFunction
        {
            using Function = boost::function<void*(void*)>;
            static Function Make()
            {
                return {};
            }
        };
        template <typename From, typename To>
        struct TH_MakeCastFunction<
            From,
            To,
            std::enable_if_t<
                std::is_convertible<std::remove_const_t<From>*, std::remove_const_t<To>*>::value
            >
        >
        {
            using Function = boost::function<void*(void*)>;
            static Function Make()
            {
                return [](void* P) -> void*
                {
                    return static_cast<
                        std::remove_const_t<To>*
                    >(
                        (std::remove_const_t<From>*)P
                    );
                };
            }
        };
    }

    /**
     * @brief Type flags to describe reflected type properties
     */
    enum class E_ReflectionTypeFlag
    {
        NONE = 0x0,
        
        EXECUTABLE = 0x1,       // Type represents executable code
        DATA = 0x2,             // Type represents data
        
        PRIMITIVE = 0x4 | DATA, // Type is a primitive data type
        CONSTANT = 0x8 | DATA,  // Type is constant
        
        RAW_FUNCTION = 0x10 | EXECUTABLE // Type is a raw function
    };

    /**
     * @brief Enumeration of primitive types supported by the reflection system
     */
    enum class E_ReflectionPrimitiveType
    {
        NONE = 0,
        
        U8 = 1,
        U16 = 2,
        U32 = 3,
        U64 = 4,

        I8 = 5,
        I16 = 6,
        I32 = 7,
        I64 = 8,

        B8 = 9,

        CHAR = (
            std::is_same_v<F_Char, Abytek::U8>
            ? E_ReflectionPrimitiveType::U8
            : 10
        ),
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        TEXT_CHAR = (
            std::is_same_v<F_TextChar, Abytek::U16>
            ? E_ReflectionPrimitiveType::U16
            : 11
        ),
#else
        TEXT_CHAR = CHAR,
#endif

        SZ = (
            std::is_same_v<Abytek::Sz, Abytek::U64>
            ? E_ReflectionPrimitiveType::U64
            : 12
        ),
        PDIFF = (
            std::is_same_v<Abytek::PDiff, Abytek::I64>
            ? E_ReflectionPrimitiveType::I64
            : 13
        ) ,

        POINTER = 14,
        REFERENCE = 15
    };

    /**
     * @brief Descriptor for a reflected type containing essential type information
     */
    struct F_ReflectionTypeDesc
    {
        F_GeneralTypeHashCode HashCode = H_GeneralTypeHashCode::MakeInvalid();         // Unique hash code identifying the type
        F_Name FullName; // Full name of the type
        U16 Size = 0;             // Size of the type in bytes
        
        template<typename __F>
        static auto Make()
        {
            F_ReflectionTypeDesc Result;

            constexpr B8 IsRawFunction = std::is_function_v<__F>;
            using F_SafeSizeTarget = std::conditional_t<
                IsRawFunction,
                U8,
                __F
            >;

            Result.HashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F>;
            Result.FullName = ToText(TypeFullName<__F>());
            Result.Size = IsRawFunction ? 0 : sizeof(F_SafeSizeTarget);
        
            return boost::move(Result);
        };
    };

    struct F_LinkedReflectType
    {
        const F_Char* FullName = nullptr;
        TW<F_ReflectionType> (*Callback)(const TW_Valid<F_ReflectionSession>& ReflectionSession);
        
        TW_Valid<F_ReflectionType> operator () (const TW_Valid<F_ReflectionSession>& ReflectionSession) const
        {
            return Callback(ReflectionSession);
        }
    };
}