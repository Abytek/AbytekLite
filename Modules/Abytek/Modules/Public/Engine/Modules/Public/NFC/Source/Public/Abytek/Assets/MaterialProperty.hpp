#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/Assets/MaterialCommon.hpp"
#include "Abytek/Assets/Texture.hpp"


namespace Abytek
{
    class F_Texture;
    
    struct ABYTEK_ENGINE_NFC_API A_MaterialPropertyBase
    {
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(A_MaterialPropertyBase);
        
        F_Name Name;
        
        virtual ~A_MaterialPropertyBase() = default;
        
        virtual B8 IsStatic() const { return false; }
        
    protected:
        static A_MaterialPropertyBase MakeBase(const F_Name& InName)
        {
            A_MaterialPropertyBase Result;
            Result.Name = InName;
            return ABYTEK_MOVE(Result);
        }
        
        friend B8 operator == (const A_MaterialPropertyBase& A, const A_MaterialPropertyBase& B)
        {
            return (
                (A.Name == B.Name)  
            );
        }
        friend B8 operator != (const A_MaterialPropertyBase& A, const A_MaterialPropertyBase& B)
        {
            return !(A == B);
        }
    };
    template<typename __F_Value>
    struct TF_MaterialPropertyScalar : A_MaterialPropertyBase
    {
        using F_Value = __F_Value;
        
        ABYTEK_BEGIN_REFLECTOR(A_MaterialPropertyBase)
        ABYTEK_END_REFLECTOR(TF_MaterialPropertyScalar)
        {
            auto ValueType = ReflectionType->ReflectReferenced<F_Value>();
            ABYTEK_REFLECT_COMMAND(
                OnReflectCanonicals,
                [=]()
                {
                    if (auto ValueTypeCanonical = ValueType->GetCanonical())
                    {
                        ReflectionType->SetCanonical(
                            ABYTEK_TEXT("Abytek::TF_MaterialPropertyScalar<")
                            + *ValueTypeCanonical
                            + ABYTEK_TEXT(">")
                        );
                    }
                }
            );
        
            ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(DefaultValue);
            ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(PermutationValues);
        }
        
        F_Value DefaultValue;
        TF_Vector<F_Value> PermutationValues;
        
        B8 IsStatic() const override
        {
            return !PermutationValues.empty();
        }
        
        static F_MaterialPermutationHashCode GetValueHashCode(const F_Value& Value)
        {
            return boost::hash<F_Value>()(Value);
        }
        
        static TF_MaterialPropertyScalar Make(const F_Name& InName, const F_Value& InDefaultValue = F_Value(0), const TF_Vector<F_Value>& InPermutationValues = {})
        {
            TF_MaterialPropertyScalar Result;
            static_cast<A_MaterialPropertyBase&>(Result) = A_MaterialPropertyBase::MakeBase(InName);
            Result.DefaultValue = InDefaultValue;
            Result.PermutationValues = InPermutationValues;
            return ABYTEK_MOVE(Result);
        }
        
        friend B8 operator == (const TF_MaterialPropertyScalar& A, const TF_MaterialPropertyScalar& B)
        {
            return (
                (static_cast<const A_MaterialPropertyBase&>(A) == static_cast<const A_MaterialPropertyBase&>(B))  
                && (A.DefaultValue == B.DefaultValue)  
                && (A.PermutationValues == B.PermutationValues)  
            );
        }
        friend B8 operator != (const TF_MaterialPropertyScalar& A, const TF_MaterialPropertyScalar& B)
        {
            return !(A == B);
        }
    };
    struct ABYTEK_ENGINE_NFC_API F_MaterialPropertyTexture : A_MaterialPropertyBase
    {
        using F_Value = TS<F_Texture>;
        
        ABYTEK_BEGIN_REFLECTOR(A_MaterialPropertyBase)
        ABYTEK_END_REFLECTOR(F_MaterialPropertyTexture);
        
        F_Value DefaultValue;
        
        static F_MaterialPropertyTexture Make(const F_Name& InName, const F_Value& InDefaultValue = {})
        {
            F_MaterialPropertyTexture Result;
            static_cast<A_MaterialPropertyBase&>(Result) = A_MaterialPropertyBase::MakeBase(InName);
            Result.DefaultValue = InDefaultValue;
            return ABYTEK_MOVE(Result);
        }
        
        friend B8 operator == (const F_MaterialPropertyTexture& A, const F_MaterialPropertyTexture& B)
        {
            return (
                (static_cast<const A_MaterialPropertyBase&>(A) == static_cast<const A_MaterialPropertyBase&>(B))  
                && (A.DefaultValue == B.DefaultValue)  
            );
        }
        friend B8 operator != (const F_MaterialPropertyTexture& A, const F_MaterialPropertyTexture& B)
        {
            return !(A == B);
        }
    };
    struct ABYTEK_ENGINE_NFC_API F_MaterialPropertyList
    {
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_MaterialPropertyList);
        
        TF_Vector<TF_MaterialPropertyScalar<B8>> Properties_B8;
        TF_Vector<TF_MaterialPropertyScalar<U32>> Properties_U32;
        TF_Vector<TF_MaterialPropertyScalar<I32>> Properties_I32;
        TF_Vector<TF_MaterialPropertyScalar<F32>> Properties_F32;
        TF_Vector<F_MaterialPropertyTexture> Properties_Texture;
        
        static F_MaterialPropertyList Combine(const F_MaterialPropertyList& A, const F_MaterialPropertyList& B);
        
        friend B8 operator == (const F_MaterialPropertyList& A, const F_MaterialPropertyList& B)
        {
            return (
                (A.Properties_B8 == B.Properties_B8)  
                && (A.Properties_U32 == B.Properties_U32)  
                && (A.Properties_I32 == B.Properties_I32)  
                && (A.Properties_F32 == B.Properties_F32)  
                && (A.Properties_Texture == B.Properties_Texture)  
            );
        }
        friend B8 operator != (const F_MaterialPropertyList& A, const F_MaterialPropertyList& B)
        {
            return !(A == B);
        }
    };
    
    struct ABYTEK_ENGINE_NFC_API F_MaterialPropertyListLayout
    {
        TF_Vector<U32> DynamicPropertyIndices_B8;
        TF_Vector<U32> DynamicPropertyIndices_U32;
        TF_Vector<U32> DynamicPropertyIndices_I32;
        TF_Vector<U32> DynamicPropertyIndices_F32;
        TF_Vector<U32> DynamicPropertyIndices_Texture;
        
        TF_Vector<U32> StaticPropertyIndices_B8;
        TF_Vector<U32> StaticPropertyIndices_U32;
        TF_Vector<U32> StaticPropertyIndices_I32;
        TF_Vector<U32> StaticPropertyIndices_F32;
        
        TF_Map<F_Name, U32> PropertyNameToPropertyIndex_B8;
        TF_Map<F_Name, U32> PropertyNameToPropertyIndex_U32;
        TF_Map<F_Name, U32> PropertyNameToPropertyIndex_I32;
        TF_Map<F_Name, U32> PropertyNameToPropertyIndex_F32;
        TF_Map<F_Name, U32> PropertyNameToPropertyIndex_Texture;
        
        static F_MaterialPropertyListLayout Make(const F_MaterialPropertyList& PropertyList);
        
#define ABYTEK_INTERNAL_ADD_FUNCTIONS_MATERIAL_PROPERTY(...) \
        B8 Has_ ## __VA_ARGS__(const F_Name& PropertyName) const \
        { \
            return PropertyNameToPropertyIndex_ ## __VA_ARGS__.find(PropertyName) != PropertyNameToPropertyIndex_ ## __VA_ARGS__.end(); \
        } \
        TF_Optional<U32> FindIndex_ ## __VA_ARGS__(const F_Name& PropertyName) const \
        { \
            auto It = PropertyNameToPropertyIndex_ ## __VA_ARGS__.find(PropertyName); \
            if (It == PropertyNameToPropertyIndex_ ## __VA_ARGS__.end()) \
            { \
                return {}; \
            } \
            return It->second; \
        } \
        U32 GetIndex_ ## __VA_ARGS__(const F_Name& PropertyName) const \
        { \
            auto It = PropertyNameToPropertyIndex_ ## __VA_ARGS__.find(PropertyName); \
            ABYTEK_ENGINE_NFC_ASSERT(It != PropertyNameToPropertyIndex_ ## __VA_ARGS__.end()) << "Not found property of type " << #__VA_ARGS__ << " with name: " << PropertyName; \
            return It->second; \
        }
        ABYTEK_INTERNAL_ADD_FUNCTIONS_MATERIAL_PROPERTY(B8);
        ABYTEK_INTERNAL_ADD_FUNCTIONS_MATERIAL_PROPERTY(U32);
        ABYTEK_INTERNAL_ADD_FUNCTIONS_MATERIAL_PROPERTY(I32);
        ABYTEK_INTERNAL_ADD_FUNCTIONS_MATERIAL_PROPERTY(F32);
        ABYTEK_INTERNAL_ADD_FUNCTIONS_MATERIAL_PROPERTY(Texture);
#undef ABYTEK_INTERNAL_ADD_FUNCTIONS_MATERIAL_PROPERTY
        
        template<typename __F_Callback>
        void ForEachPropertyIndices_B8(__F_Callback&& Callback)
        {
            for (auto Index : DynamicPropertyIndices_B8)
            {
                Callback(Index);
            }
            for (auto Index : StaticPropertyIndices_B8)
            {
                Callback(Index);
            }
        }
        template<typename __F_Callback>
        void ForEachPropertyIndices_U32(__F_Callback&& Callback)
        {
            for (auto Index : DynamicPropertyIndices_U32)
            {
                Callback(Index);
            }
            for (auto Index : StaticPropertyIndices_U32)
            {
                Callback(Index);
            }
        }
        template<typename __F_Callback>
        void ForEachPropertyIndices_I32(__F_Callback&& Callback)
        {
            for (auto Index : DynamicPropertyIndices_I32)
            {
                Callback(Index);
            }
            for (auto Index : StaticPropertyIndices_I32)
            {
                Callback(Index);
            }
        }
        template<typename __F_Callback>
        void ForEachPropertyIndices_F32(__F_Callback&& Callback)
        {
            for (auto Index : DynamicPropertyIndices_F32)
            {
                Callback(Index);
            }
            for (auto Index : StaticPropertyIndices_F32)
            {
                Callback(Index);
            }
        }
        template<typename __F_Callback>
        void ForEachPropertyIndices_Texture(__F_Callback&& Callback)
        {
            for (auto Index : DynamicPropertyIndices_Texture)
            {
                Callback(Index);
            }
        }
    };
    
    struct ABYTEK_ENGINE_NFC_API A_MaterialPropertyInstanceBase
    {
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(A_MaterialPropertyInstanceBase);
        
        F_Name Name;
        
    protected:
        static A_MaterialPropertyInstanceBase MakeBase(const A_MaterialPropertyBase& Property)
        {
            A_MaterialPropertyInstanceBase Result;
            Result.Name = Property.Name;
            return ABYTEK_MOVE(Result);
        }
        
        friend B8 operator == (const A_MaterialPropertyInstanceBase& A, const A_MaterialPropertyInstanceBase& B)
        {
            return (
                (A.Name == B.Name)  
            );
        }
        friend B8 operator != (const A_MaterialPropertyInstanceBase& A, const A_MaterialPropertyInstanceBase& B)
        {
            return !(A == B);
        }
    };
    template<typename __F_Value>
    struct TF_MaterialPropertyInstanceScalar : A_MaterialPropertyInstanceBase
    {
        using F_Value = __F_Value;
        
        ABYTEK_BEGIN_REFLECTOR(A_MaterialPropertyInstanceBase)
        ABYTEK_END_REFLECTOR(TF_MaterialPropertyInstanceScalar)
        {
            auto ValueType = ReflectionType->ReflectReferenced<F_Value>();
            ABYTEK_REFLECT_COMMAND(
                OnReflectCanonicals,
                [=]()
                {
                    if (auto ValueTypeCanonical = ValueType->GetCanonical())
                    {
                        ReflectionType->SetCanonical(
                            ABYTEK_TEXT("Abytek::TF_MaterialPropertyInstanceScalar<")
                            + *ValueTypeCanonical
                            + ABYTEK_TEXT(">")
                        );
                    }
                }
            );
        
            ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Value);
        }
        
        F_Value Value;
        
        static TF_MaterialPropertyInstanceScalar Make(const TF_MaterialPropertyScalar<F_Value>& Property)
        {
            TF_MaterialPropertyInstanceScalar Result;
            static_cast<A_MaterialPropertyInstanceBase&>(Result) = A_MaterialPropertyInstanceBase::MakeBase(Property);
            Result.Value = Property.DefaultValue;
            return ABYTEK_MOVE(Result);
        }
        
        friend B8 operator == (const TF_MaterialPropertyInstanceScalar& A, const TF_MaterialPropertyInstanceScalar& B)
        {
            return (
                (static_cast<const A_MaterialPropertyInstanceBase&>(A) == static_cast<const A_MaterialPropertyInstanceBase&>(B))  
                && (A.Value == B.Value)  
            );
        }
        friend B8 operator != (const TF_MaterialPropertyInstanceScalar& A, const TF_MaterialPropertyInstanceScalar& B)
        {
            return !(A == B);
        }
    };
    struct ABYTEK_ENGINE_NFC_API F_MaterialPropertyInstanceTexture : A_MaterialPropertyInstanceBase
    {
        ABYTEK_BEGIN_REFLECTOR(A_MaterialPropertyInstanceBase)
        ABYTEK_END_REFLECTOR(F_MaterialPropertyInstanceTexture);
        
        TS<F_Texture> Value;
        
        static F_MaterialPropertyInstanceTexture Make(const F_MaterialPropertyTexture& Property)
        {
            F_MaterialPropertyInstanceTexture Result;
            static_cast<A_MaterialPropertyInstanceBase&>(Result) = MakeBase(Property);
            Result.Value = Property.DefaultValue;
            return ABYTEK_MOVE(Result);
        }
        
        friend B8 operator == (const F_MaterialPropertyInstanceTexture& A, const F_MaterialPropertyInstanceTexture& B)
        {
            return (
                (static_cast<const A_MaterialPropertyInstanceBase&>(A) == static_cast<const A_MaterialPropertyInstanceBase&>(B))  
                && (A.Value == B.Value)  
            );
        }
        friend B8 operator != (const F_MaterialPropertyInstanceTexture& A, const F_MaterialPropertyInstanceTexture& B)
        {
            return !(A == B);
        }
    };
    struct ABYTEK_ENGINE_NFC_API F_MaterialPropertyInstanceList
    {
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_MaterialPropertyInstanceList);
        
        TF_Vector<TF_MaterialPropertyInstanceScalar<B8>> Properties_B8;
        TF_Vector<TF_MaterialPropertyInstanceScalar<U32>> Properties_U32;
        TF_Vector<TF_MaterialPropertyInstanceScalar<I32>> Properties_I32;
        TF_Vector<TF_MaterialPropertyInstanceScalar<F32>> Properties_F32;
        TF_Vector<F_MaterialPropertyInstanceTexture> Properties_Texture;
        
        friend B8 operator == (const F_MaterialPropertyInstanceList& A, const F_MaterialPropertyInstanceList& B)
        {
            return (
                (A.Properties_B8 == B.Properties_B8)  
                && (A.Properties_U32 == B.Properties_U32)  
                && (A.Properties_I32 == B.Properties_I32)  
                && (A.Properties_F32 == B.Properties_F32)  
                && (A.Properties_Texture == B.Properties_Texture)  
            );
        }
        friend B8 operator != (const F_MaterialPropertyInstanceList& A, const F_MaterialPropertyInstanceList& B)
        {
            return !(A == B);
        }
        
        F_MaterialPermutationHashCode CalculateHashCode(
            const F_MaterialPropertyList& PropertyList,
            const F_MaterialPropertyListLayout& PropertyListLayout
        ) const;
    };
    
    struct ABYTEK_ENGINE_NFC_API H_MaterialProperty
    {
        static F_MaterialPropertyInstanceList GenerateDefaultInstance(const F_MaterialPropertyList& PropertyList);
        static TF_Vector<F_MaterialPropertyInstanceList> GeneratePermutations(const F_MaterialPropertyList& PropertyList);
    };
}