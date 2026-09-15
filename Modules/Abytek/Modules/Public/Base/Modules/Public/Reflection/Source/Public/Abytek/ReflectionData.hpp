#pragma once

#include "Abytek/ReflectionCommon.hpp"


namespace Abytek
{
    class F_ReflectionType;
    
    struct F_ReflectionMetadataElement
    {
        F_Name Key;
        F_Any Value;
    };
    class ABYTEK_BASE_REFLECTION_API F_ReflectionMetadata
    {
    private:
        TF_Vector<F_ReflectionMetadataElement> _Elements;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetElements() const noexcept
        {
            return _Elements;
        }
        
    public:
        F_ReflectionMetadata() = default;
        F_ReflectionMetadata(const F_ReflectionMetadata& X) = default;
        F_ReflectionMetadata& operator = (const F_ReflectionMetadata& X) = default;
        F_ReflectionMetadata(F_ReflectionMetadata&& X) = default;
        F_ReflectionMetadata& operator = (F_ReflectionMetadata&& X) = default;
        
    public:
        B8 HasElement(const F_Name& Key) const noexcept
        {
            for (const auto& Element : _Elements)
            {
                if (Element.Key == Key)
                {
                    return true;
                }
            }
            return false;
        }
        const F_ReflectionMetadataElement* FindElement(const F_Name& Key) const noexcept
        {
            for (const auto& Element : _Elements)
            {
                if (Element.Key == Key)
                {
                    return &Element;
                }
            }
            return nullptr;
        }
        F_ReflectionMetadataElement* FindElement(const F_Name& Key) noexcept
        {
            for (auto& Element : _Elements)
            {
                if (Element.Key == Key)
                {
                    return &Element;
                }
            }
            return nullptr;
        }
        const F_ReflectionMetadataElement& GetElement(const F_Name& Key) const noexcept
        {
            for (const auto& Element : _Elements)
            {
                if (Element.Key == Key)
                {
                    return Element;
                }
            }
            ABYTEK_LOG_FATAL() << "Not found metadata key: " << Key;
            return MakeNullReference<const F_ReflectionMetadataElement>();
        }
        F_ReflectionMetadataElement& GetElement(const F_Name& Key) noexcept
        {
            for (auto& Element : _Elements)
            {
                if (Element.Key == Key)
                {
                    return Element;
                }
            }
            ABYTEK_LOG_FATAL() << "Not found metadata key: " << Key;
            return MakeNullReference<F_ReflectionMetadataElement>();
        }
        F_Any* Find(const F_Name& Key) noexcept
        {
            for (auto& Element : _Elements)
            {
                if (Element.Key == Key)
                {
                    return &Element.Value;
                }
            }
            return nullptr;
        }
        const F_Any& Get(const F_Name& Key) const noexcept
        {
            for (const auto& Element : _Elements)
            {
                if (Element.Key == Key)
                {
                    return Element.Value;
                }
            }
            ABYTEK_LOG_FATAL() << "Not found metadata key: " << Key;
            return MakeNullReference<const F_Any>();
        }
        F_Any& Get(const F_Name& Key) noexcept
        {
            for (auto& Element : _Elements)
            {
                if (Element.Key == Key)
                {
                    return Element.Value;
                }
            }
            ABYTEK_LOG_FATAL() << "Not found metadata key: " << Key;
            return MakeNullReference<F_Any>();
        }
        F_Any& Add(const F_ReflectionMetadataElement& Element)
        {
            ABYTEK_BASE_REFLECTION_ASSERT(!HasElement(Element.Key)) << "Already added element with key: " << Element.Key;
            _Elements.push_back(Element);
            return _Elements.back().Value;
        }
        F_Any& Add(F_ReflectionMetadataElement&& Element)
        {
            ABYTEK_BASE_REFLECTION_ASSERT(!HasElement(Element.Key)) << "Already added element with key: " << Element.Key;
            _Elements.push_back(ABYTEK_MOVE(Element));
            return _Elements.back().Value;
        }
        F_Any& Add(const F_Name& Key, const F_Any& Value)
        {
            return Add(F_ReflectionMetadataElement { Key, Value });
        }
        F_Any& Add(const F_Name& Key, F_Any&& Value)
        {
            return Add(F_ReflectionMetadataElement { Key, ABYTEK_MOVE(Value) });
        }
        F_Any& operator [] (const F_Name& Key) noexcept
        {
            if (auto ElementHandle = FindElement(Key))
            {
                return ElementHandle->Value;
            }
            return Add({ Key });
        }
        const F_Any& operator [] (const F_Name& Key) const noexcept
        {
            return Get(Key);
        }
        void Remove(const F_Name& Key)
        {
            for (auto It = _Elements.begin(); It != _Elements.end(); ++It)
            {
                if (It->Key == Key)
                {
                    _Elements.erase(It);
                    return;
                }
            }
            ABYTEK_LOG_FATAL() << "Failed to remove metadata element: " << Key;
        }
    };

    /**
     * @brief Represents a reflected variable's type information
     */
    struct F_ReflectionVariable
    {
        F_GeneralTypeHashCode TypeHashCode = 0;  // Hash code of the variable's type

        TF_Function<void(void*)> MakeDefaultValue;  // Function to create a default value for this variable type
        F_ReflectionVariable& SetMakeDefaultValue(TF_Function<void(void*)>&& Value)
        {
            MakeDefaultValue = boost::move(Value);
            return *this;
        }
    };

    /**
     * @brief A named reflected variable
     */
    struct F_ReflectionNamedVariable : public F_ReflectionVariable
    {
        F_Name Name;
    };

    /**
     * @brief Function flags to describe reflected function properties
     */
    enum class E_ReflectionFunctionFlag
    {
        NONE = 0x0,
        CONSTANT = 0x1,      // Function is marked const
        CONSTEXPR = 0x2,     // Function is constexpr
        NOEXCEPT = 0x4,      // Function is noexcept
        RIGHT_VALUE = 0x8,   // Function accepts rvalue references
        LEFT_VALUE = 0x10,   // Function accepts lvalue references
        
        DEFAULT = RIGHT_VALUE | LEFT_VALUE
    };

    /**
     * @brief Represents a function signature with inputs and outputs
     */
    struct F_ReflectionFunctionSignature
    {
        using F_Variable = F_ReflectionVariable;
        using F_NamedVariable = F_ReflectionNamedVariable;

        ContainerTemplates::TF_Vector<F_NamedVariable> NamedInputs;  // Function parameters
        TF_Optional<F_Variable> Output;  // Function return value (if any)
        
        B8 IsStatic = false;  // Whether the function is static
    };

    /**
     * @brief Represents a reflected function
     */
    struct F_ReflectionFunction
    {
        using F_FunctionSignature = F_ReflectionFunctionSignature;

        F_FunctionSignature Signature;  // Function signature
        F_ReflectionFunction& SetSignature(const F_FunctionSignature& Value)
        {
            Signature = Value;
            return *this;
        }
        F_ReflectionFunction& SetSignature(F_FunctionSignature&& Value)
        {
            Signature = boost::move(Value);
            return *this;
        }

        PDiff Address = 0;  // Function address
        F_GeneralTypeHashCode TypeHashCode = 0;  // Type hash code of the function
        F_GeneralTypeHashCode CallerTypeHashCode = 0;  // Caller type hash code
        
        E_ReflectionFunctionFlag Flags = E_ReflectionFunctionFlag::DEFAULT;  // Function flags
        F_ReflectionFunction& SetFlags(E_ReflectionFunctionFlag Value)
        {
            Flags = Value;
            return *this;
        }

        /**
         * @brief Invoke the reflected function
         * 
         * @tparam __F_Return Return type
         * @tparam __F_Args Argument types
         * @param Args Function arguments
         * @return __F_Return Function return value
         */
        template<typename __F_Return, typename... __F_Args>
        __F_Return Invoke(__F_Args... Args) const
        {
            static_assert(CallerTypeHashCode == Abytek::H_GeneralTypeHashCode::StaticValue<__F_Return(__F_Args...)>, "caller type not match");
            return ((__F_Return(*)(__F_Args...))Address)(ABYTEK_FORWARD(Args)...);
        }
    };
    struct F_ReflectionNamedFunction : F_ReflectionFunction
    {
        F_Name Name;
    };
    struct F_ReflectionProperty : F_ReflectionNamedVariable
    {
        F_ReflectionMetadata Metadata;

        F_GeneralTypeHashCode ObjectTypeHashCode = 0;
        
        PDiff StaticAddress = 0;
        
        PDiff ReferenceAccessFunctorAddress = 0;
        PDiff ConstructDefaultFunctorAddress = 0;
        PDiff DestructFunctorAddress = 0;
        PDiff MoveConstructFunctorAddress = 0;
        PDiff CopyConstructFunctorAddress = 0;
        PDiff MoveAssignFunctorAddress = 0;
        PDiff CopyAssignFunctorAddress = 0;

        template<typename __F_Variable>
        std::remove_reference_t<__F_Variable>& ReferenceAccess(void* ObjectRawP) const
        {
            using F_Functor = std::remove_reference_t<__F_Variable>&(void*);
            return ((F_Functor*)ReferenceAccessFunctorAddress)(ObjectRawP);
        }
        void ConstructDefault(void* ObjectRawP) const
        {
            using F_Functor = void(void*);
            ((F_Functor*)ConstructDefaultFunctorAddress)(ObjectRawP);
        }
        void Destruct(void* ObjectRawP) const
        {
            using F_Functor = void(void*);
            ((F_Functor*)DestructFunctorAddress)(ObjectRawP);
        }
        template<class __F_UserClass, typename __F_Variable>
        void MoveConstruct(__F_UserClass* ObjectRawP, std::remove_reference_t<__F_Variable>&& X) const
        {
            using F_Functor = void(__F_UserClass*, std::remove_reference_t<__F_Variable>&&);
            ((F_Functor*)MoveConstructFunctorAddress)(ObjectRawP, boost::move(X));
        }
        template<class __F_UserClass, typename __F_Variable>
        void CopyConstruct(__F_UserClass* ObjectRawP, const std::remove_reference_t<__F_Variable>& X) const
        {
            using F_Functor = void(__F_UserClass*, const std::remove_reference_t<__F_Variable>&);
            ((F_Functor*)CopyConstructFunctorAddress)(ObjectRawP, X);
        }
        template<class __F_UserClass, typename __F_Variable>
        void MoveAssign(__F_UserClass* ObjectRawP, std::remove_reference_t<__F_Variable>&& X) const
        {
            using F_Functor = void(__F_UserClass*, std::remove_reference_t<__F_Variable>&&);
            ((F_Functor*)MoveAssignFunctorAddress)(ObjectRawP, boost::move(X));
        }
        template<class __F_UserClass, typename __F_Variable>
        void CopyAssign(__F_UserClass* ObjectRawP, const std::remove_reference_t<__F_Variable>& X) const
        {
            using F_Functor = void(__F_UserClass*, const std::remove_reference_t<__F_Variable>&);
            ((F_Functor*)CopyAssignFunctorAddress)(ObjectRawP, X);
        }

        template<typename __F_Variable>
        std::remove_reference_t<__F_Variable>& ReferenceAccess() const
        {
            using F_Functor = std::remove_reference_t<__F_Variable>&(PDiff StaticAddress);
            return ((F_Functor*)ReferenceAccessFunctorAddress)(StaticAddress);
        }
        void ConstructDefault() const
        {
            using F_Functor = void(PDiff StaticAddress);
            ((F_Functor*)ConstructDefaultFunctorAddress)(StaticAddress);
        }
        void Destruct() const
        {
            using F_Functor = void(PDiff StaticAddress);
            ((F_Functor*)DestructFunctorAddress)(StaticAddress);
        }
        template<typename __F_Variable>
        void MoveConstruct(std::remove_reference_t<__F_Variable>&& X) const
        {
            using F_Functor = void(PDiff StaticAddress, std::remove_reference_t<__F_Variable>&&);
            ((F_Functor*)MoveConstructFunctorAddress)(StaticAddress, boost::move(X));
        }
        template<typename __F_Variable>
        void CopyConstruct(const std::remove_reference_t<__F_Variable>& X) const
        {
            using F_Functor = void(PDiff StaticAddress, const std::remove_reference_t<__F_Variable>&);
            ((F_Functor*)CopyConstructFunctorAddress)(StaticAddress, X);
        }
        template<typename __F_Variable>
        void MoveAssign(std::remove_reference_t<__F_Variable>&& X) const
        {
            using F_Functor = void(PDiff StaticAddress, std::remove_reference_t<__F_Variable>&&);
            ((F_Functor*)MoveAssignFunctorAddress)(StaticAddress, boost::move(X));
        }
        template<typename __F_Variable>
        void CopyAssign(const std::remove_reference_t<__F_Variable>& X) const
        {
            using F_Functor = void(PDiff StaticAddress, const std::remove_reference_t<__F_Variable>&);
            ((F_Functor*)CopyAssignFunctorAddress)(StaticAddress, X);
        }
    };
    struct F_ReflectionMethod : F_ReflectionNamedFunction
    {
        F_ReflectionMetadata Metadata;
    };
    struct F_ReflectionEnumValue : F_ReflectionNamedVariable
    {
        F_ReflectionMetadata Metadata;
        
        F_Any Value;
    };
    
    struct F_ReflectionTypeBase
    {
        TW<F_ReflectionType> Type;
        TF_Function<void*(void*)> CastFunction;
    };
    
    struct F_ReflectionTypeAdditionalInfo
    {
        ContainerTemplates::TF_Vector<F_ReflectionProperty> Properties;
        ContainerTemplates::TF_Vector<F_ReflectionMethod> Methods;
        ContainerTemplates::TF_Vector<F_ReflectionEnumValue> EnumValues;

        F_ReflectionMetadata Metadata;
        
        TF_Function<TU<A_Object>()> MakeUniqueDefaultObject;
        TF_Function<TS<A_Object>()> MakeSharedDefaultObject;
        TF_Function<TS<A_Object>()> MakeRADefaultObject;
        TF_Function<void(void*)> ConstructDefault;
        TF_Function<void(void*)> Destruct;
        TF_Function<void(void* TargetP, void* XP)> MoveConstruct;
        TF_Function<void(void* TargetP, void* XP)> CopyConstruct;
        TF_Function<void(void* TargetP, void* XP)> MoveAssign;
        TF_Function<void(void* TargetP, void* XP)> CopyAssign;

        ContainerTemplates::TF_Vector<F_ReflectionTypeBase> Bases;
        
        B8 IsAbstract = false;
        B8 RequireStatic = false;
        
        template<
            typename __F_Type,
            typename __F_Reflector
        >
        static auto Make()
        {
            F_ReflectionTypeAdditionalInfo Result;

            using F_TypeIfAbleToBePointer = Internal::Reflection::TF_MakeItAbleToBePointer<__F_Type>;
            using F_TargetObject = std::conditional_t<
                std::is_base_of_v<A_Object, F_TypeIfAbleToBePointer>,
                F_TypeIfAbleToBePointer,
                F_FakeRAObject
            >;
            
            Result.IsAbstract = std::is_abstract_v<__F_Type>;
            Result.RequireStatic = (
                __F_Reflector::H_ReflectGen::RequireStatic_Interface
                || __F_Reflector::H_ReflectGen::RequireStatic_Private
                || __F_Reflector::H_ReflectGen::RequireStatic_Public
                
                || __F_Reflector::H_ReflectGen::RequireStatic_Method_Public
                || __F_Reflector::H_ReflectGen::RequireStatic_Method_Private
                || __F_Reflector::H_ReflectGen::RequireStatic_Method_Public
                
                || __F_Reflector::H_ReflectGen::RequireStatic_MethodStatic_Public
                || __F_Reflector::H_ReflectGen::RequireStatic_MethodStatic_Private
                || __F_Reflector::H_ReflectGen::RequireStatic_MethodStatic_Public
                
                || __F_Reflector::H_ReflectGen::RequireStatic_Property_Public
                || __F_Reflector::H_ReflectGen::RequireStatic_Property_Private
                || __F_Reflector::H_ReflectGen::RequireStatic_Property_Public
                
                || __F_Reflector::H_ReflectGen::RequireStatic_PropertyStatic_Public
                || __F_Reflector::H_ReflectGen::RequireStatic_PropertyStatic_Private
                || __F_Reflector::H_ReflectGen::RequireStatic_PropertyStatic_Public
                
                || __F_Reflector::H_ReflectGen::RequireStatic_EnumValue_Public
                || __F_Reflector::H_ReflectGen::RequireStatic_EnumValue_Private
                || __F_Reflector::H_ReflectGen::RequireStatic_EnumValue_Public
            );
            
            Result.MakeUniqueDefaultObject = []() -> TU<A_Object>
            {
                using U_Casted = TU<A_Object>::template TF_RebindObject<
                    std::conditional_t<
                        std::is_default_constructible_v<F_TargetObject> && std::is_destructible_v<F_TargetObject>,
                        F_TargetObject,
                        F_FakeRAObject
                    >
                >;
                return U_Casted()();
            };
            Result.MakeSharedDefaultObject = []() -> TS<A_Object>
            {
                using S_Casted = TS<A_Object>::template TF_RebindObject<
                    std::conditional_t<
                        std::is_default_constructible_v<F_TargetObject> && std::is_destructible_v<F_TargetObject>,
                        F_TargetObject,
                        F_FakeRAObject
                    >
                >;
                return S_Casted()();
            };
            Result.MakeRADefaultObject = []() -> TS<A_Object>
            {
                return RACreateShared<
                    std::conditional_t<
                        std::is_default_constructible_v<F_TargetObject> && std::is_destructible_v<F_TargetObject> 
                        && RACanCreateObject<F_TypeIfAbleToBePointer>,
                        F_TargetObject,
                        F_FakeRAObject
                    >
                >(); 
            };
            
            Result.ConstructDefault = [](void* ObjectRawP)
            {
                using F_Target = std::conditional_t<
                    std::is_default_constructible_v<__F_Type>,
                    F_TypeIfAbleToBePointer,
                    U64
                >;
                ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, F_TypeIfAbleToBePointer>) << "can't construct default object";
                
                new(ObjectRawP) F_Target {};
            };
            Result.Destruct = [](void* ObjectRawP)
            {
                using F_Target = std::conditional_t<
                    std::is_destructible_v<__F_Type>,
                    F_TypeIfAbleToBePointer,
                    U64
                >;
                ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, F_TypeIfAbleToBePointer>) << "can't destruct";
                        
                ((F_Target*)ObjectRawP)->~F_Target();
            };
            
            Result.MoveConstruct = [](void* TargetP, void* XP)
            {
                using F_Target = std::conditional_t<
                    std::is_move_constructible_v<__F_Type>,
                    F_TypeIfAbleToBePointer,
                    U64
                >;
                ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, F_TypeIfAbleToBePointer>) << "can't move construct object";
                
                new(TargetP) F_Target {
                    boost::move(*((F_Target*)XP))
                };
            };
            Result.CopyConstruct = [](void* TargetP, void* XP)
            {
                using F_Target = std::conditional_t<
                    std::is_copy_constructible_v<__F_Type>,
                    F_TypeIfAbleToBePointer,
                    U64
                >;
                ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, F_TypeIfAbleToBePointer>) << "can't copy construct object";
                
                new(TargetP) F_Target {
                    *((F_Target*)XP)
                };
            };
            Result.MoveAssign = [](void* TargetP, void* XP)
            {
                using F_Target = std::conditional_t<
                    std::is_move_assignable_v<__F_Type>,
                    F_TypeIfAbleToBePointer,
                    U64
                >;
                ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, F_TypeIfAbleToBePointer>) << "can't move object";
                
                *(((F_Target*)TargetP)) = boost::move(*((F_Target*)XP)); 
            };
            Result.CopyAssign = [](void* TargetP, void* XP)
            {
                using F_Target = std::conditional_t<
                    std::is_copy_assignable_v<__F_Type>,
                    F_TypeIfAbleToBePointer,
                    U64
                >;
                ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, F_TypeIfAbleToBePointer>) << "can't copy object";
                
                *(((F_Target*)TargetP)) = *((F_Target*)XP); 
            };
            
            return boost::move(Result);
        };
    };
}