#pragma once

#include "Abytek/ReflectionItem.hpp"
#include "Abytek/ReflectionData.hpp"
#include "Abytek/ReflectionContext.hpp"
#include "Abytek/ReflectionTypeHandleChecker.hpp"


namespace Abytek
{
    struct F_ReflectionTypeBuildParams : F_ReflectionItemBuildParams
    {
        E_ReflectionTypeFlag TypeFlags = E_ReflectionTypeFlag::NONE;
        F_ReflectionTypeAdditionalInfo AdditionalInfo;
        E_ReflectionPrimitiveType PrimitiveType = E_ReflectionPrimitiveType::NONE;
        F_ReflectionTypeDesc Desc;
        B8 IsRightValueReference = false;
    };
    class ABYTEK_BASE_REFLECTION_API F_ReflectionType : public A_ReflectionItem
    {
    public:
        E_ReflectionTypeFlag TypeFlags = E_ReflectionTypeFlag::NONE;
        F_ReflectionTypeAdditionalInfo AdditionalInfo;
        E_ReflectionPrimitiveType PrimitiveType = E_ReflectionPrimitiveType::NONE;
        F_ReflectionTypeDesc Desc;
        B8 IsRightValueReference = false;
        
        TW<F_ReflectionType> InsideHandle;
        
    public:
        F_ReflectionType(const F_GBMObjectConfig& GBMConfig, const F_ReflectionTypeBuildParams& BuildParams);
        virtual ~F_ReflectionType() override;
        
    public:
        template<typename __F_Object = A_Object>
        TU<__F_Object> CreateDefaultUniqueObject() const
        {
            return AdditionalInfo.MakeUniqueDefaultObject().FastCast<__F_Object>();
        }
        template<typename __F_Object = A_Object>
        TS<__F_Object> CreateDefaultSharedObject() const
        {
            return AdditionalInfo.MakeSharedDefaultObject().FastCast<__F_Object>();
        }
        template<typename __F_Object = A_Object>
        TS<__F_Object> CreateDefaultRAObject() const
        {
            return AdditionalInfo.MakeRADefaultObject().FastCast<__F_Object>();
        }
        
    public:
        B8 HasBase(const TW_Valid<F_ReflectionType>& X) const;
        B8 SupportImplicitPolymorphismCast(const TW_Valid<F_ReflectionType>& X) const;
        
    public:
        F_ReflectionMetadata& GetMetadata()
        {
            return AdditionalInfo.Metadata;
        }
        B8 IsAbstract() const
        {
            return AdditionalInfo.IsAbstract;
        }
        B8 RequireStatic() const
        {
            return AdditionalInfo.RequireStatic;
        }
        const auto& GetBases() const
        {
            return AdditionalInfo.Bases;
        }
        auto& GetProperties()
        {
            return AdditionalInfo.Properties;
        }
        auto& GetProperty(const F_Name& PropertyName)
        {
            for (auto& Property : AdditionalInfo.Properties)
            {
                if (Property.Name == PropertyName)
                {
                    return Property;
                }
            }
            ABYTEK_LOG_FATAL() << "Failed to get property: " << PropertyName;
            return MakeNullReference<F_ReflectionProperty>();
        }
        template<typename __F_Callback>
        void ForEachRepresentation(__F_Callback&& Callback)
        {
            TF_Queue<TW<F_ReflectionType>> Types;
            Types.Push(ABYTEK_WTHIS());
            TW<F_ReflectionType> Type;
            while (Types.TryPop(Type))
            {
                if (!Callback(Type))
                {
                    return;
                }
                for (const auto& Base : Type->GetBases())
                {
                    Types.Push(Base.Type);
                }
            }
        }
        template<typename __F_Callback>
        void ForEachRepresentationOnInstance(__F_Callback&& Callback, void* InstancePtr)
        {
            struct F_Representation
            {
                TW<F_ReflectionType> Type;
                void* InstancePtr;
            };
            U32 Index = 0;
            TF_Queue<F_Representation> Representations;
            Representations.Push({ ABYTEK_WTHIS(), InstancePtr });
            F_Representation Representation;
            while (Representations.TryPop(Representation))
            {
                if (!Callback(Representation.Type, Representation.InstancePtr, Index))
                {
                    return;
                }
                ++Index;
                for (const auto& Base : Representation.Type->GetBases())
                {
                    if (!Base.CastFunction)
                    {
                        continue;
                    }
                    F_Representation BaseRepresentation;
                    BaseRepresentation.Type = Base.Type;
                    BaseRepresentation.InstancePtr = Base.CastFunction(Representation.InstancePtr);
                    Representations.Push(BaseRepresentation);
                }
            }
        }
        F_FeedbackStatus CastInstancePtr(void* RawInstancePtr, U32 TargetIndex, void*& OutInstancePtr)
        {
            B8 Casted = false;
            ForEachRepresentationOnInstance(
                [TargetIndex, &OutInstancePtr, &Casted](const TW<F_ReflectionType>& Type, void* InstancePtr, U32 Index)
                {
                    if (TargetIndex == Index)
                    {
                        OutInstancePtr = InstancePtr;
                        Casted = true;
                        return false;
                    }
                    return true;
                },
                RawInstancePtr
            );
            if (!Casted)
            {    
                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Failed to cast instance at index ") + ToText(TargetIndex) + ABYTEK_TEXT(" in type: ") + *GetFullName());
            }
            return F_FeedbackStatus::MakeSucceeded();
        }
        auto& GetMethods()
        {
            return AdditionalInfo.Methods;
        }
        auto& GetEnumValues()
        {
            return AdditionalInfo.EnumValues;
        }
        F_GeneralTypeHashCode GetHashCode() const
        {
            return Desc.HashCode;
        }
        const F_Name& GetFullName() const
        {
            return Desc.FullName;
        }
        U16 GetSize() const
        {
            return Desc.Size;
        }
        
    public:
        template<typename __F_Type>
        TW_Valid<F_ReflectionType> ReflectReferenced();
    };
    
    template<typename __F>
    struct TF_ReflectionTypeHandle
    {
        using F = __F;
        
        TW<F_ReflectionType> Type;
        
        static B8 CheckType(const TW_Valid<F_ReflectionType>& InType)
        {
            return InType->SupportImplicitPolymorphismCast(InType->GetContext()->GetType<__F>());
        }
        
        TF_ReflectionTypeHandle(const TW_Valid<F_ReflectionContext>& Context) :
            Type(Context->GetType<__F>())
        {
        }
        template<typename __F_Config>
        TF_ReflectionTypeHandle(const ObjectSmartPointerTemplates::TW<F_ReflectionType, __F_Config>& InType) :
            Type(InType)
        {
            if (InType)
            {
                ABYTEK_BASE_REFLECTION_ASSERT(CheckType(InType)) 
                    << "Type mismatch: " << F_NewLine {} 
                << "\t- Base: " << TypeFullName<__F>() << F_NewLine {}
                << "\t- Target: " << InType->Desc.FullName;
            }
        }
        
        TF_ReflectionTypeHandle() = default;
        TF_ReflectionTypeHandle(const TF_ReflectionTypeHandle& X) = default;
        TF_ReflectionTypeHandle& operator = (const TF_ReflectionTypeHandle& X) = default;
        TF_ReflectionTypeHandle(TF_ReflectionTypeHandle&& X) = default;
        TF_ReflectionTypeHandle& operator = (TF_ReflectionTypeHandle&& X) = default;
        
        template<typename __F_Other ABYTEK_REQUIRES(std::is_convertible_v<__F_Other*, __F*> && !std::is_same_v<__F, __F_Other>)>
        TF_ReflectionTypeHandle(const TF_ReflectionTypeHandle<__F_Other>& X) :
            TF_ReflectionTypeHandle(X.Type)
        {
        }
        template<typename __F_Other ABYTEK_REQUIRES(std::is_convertible_v<__F_Other*, __F*> && !std::is_same_v<__F, __F_Other>)>
        TF_ReflectionTypeHandle& operator = (const TF_ReflectionTypeHandle& X)
        {
            Type = X.Type;
            return *this;
        }
        template<typename __F_Other ABYTEK_REQUIRES(std::is_convertible_v<__F_Other*, __F*> && !std::is_same_v<__F, __F_Other>)>
        TF_ReflectionTypeHandle(TF_ReflectionTypeHandle<__F_Other>&& X) :
            TF_ReflectionTypeHandle(X.Type)
        {
            X.Reset();
        }
        template<typename __F_Other ABYTEK_REQUIRES(std::is_convertible_v<__F_Other*, __F*> && !std::is_same_v<__F, __F_Other>)>
        TF_ReflectionTypeHandle& operator = (TF_ReflectionTypeHandle&& X)
        {
            Type = X.Type;
            X.Reset();
            return *this;
        }
        
        template<class __F_Config>
        operator ObjectSmartPointerTemplates::TW<F_ReflectionType, __F_Config> () const noexcept
        {
            return Type;
        }
        
        template<typename __F_Other>
        ABYTEK_FORCE_INLINE TF_ReflectionTypeHandle<__F_Other> Cast() const noexcept
        {
            return TF_ReflectionTypeHandle<__F_Other>(Type);
        }
        template<typename __F_Other>
        ABYTEK_FORCE_INLINE TF_ReflectionTypeHandle<__F_Other> TryCast() const noexcept
        {
            if constexpr (std::is_convertible_v<__F_Other*, __F*>)
            {
                return TF_ReflectionTypeHandle<__F_Other>(Type);
            }
            return {};
        }
        
        void Reset()
        {
            Type = {};
        }
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return static_cast<B8>(Type);
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        ABYTEK_FORCE_INLINE F_ReflectionType& operator * () const noexcept
        {
            ABYTEK_BASE_REFLECTION_ASSERT(IsValid()) << "Accessing invalid reflected type";
            return *Type;
        }
        ABYTEK_FORCE_INLINE F_ReflectionType* operator -> () const noexcept
        {
            ABYTEK_BASE_REFLECTION_ASSERT(IsValid()) << "Accessing invalid reflected type";
            return Type.GetObjectRawP();
        }
        
        template<typename __F_Object = __F>
        TU<__F_Object> CreateDefaultUniqueObject() const
        {
            return Type->CreateDefaultUniqueObject<__F_Object>();
        }
        template<typename __F_Object = __F>
        TS<__F_Object> CreateDefaultSharedObject() const
        {
            return Type->CreateDefaultSharedObject<__F_Object>();
        }
        template<typename __F_Object = __F>
        TS<__F_Object> CreateDefaultRAObject() const
        {
            return Type->CreateDefaultRAObject<__F_Object>();
        }
    
        friend B8 operator < (const TF_ReflectionTypeHandle& A, const TF_ReflectionTypeHandle& B) noexcept
        {
            return A.Type < B.Type;
        }
        friend B8 operator > (const TF_ReflectionTypeHandle& A, const TF_ReflectionTypeHandle& B) noexcept
        {
            return A.Type > B.Type;
        }
        friend B8 operator == (const TF_ReflectionTypeHandle& A, const TF_ReflectionTypeHandle& B) noexcept
        {
            return A.Type == B.Type;
        }
        friend B8 operator != (const TF_ReflectionTypeHandle& A, const TF_ReflectionTypeHandle& B) noexcept
        {
            return A.Type != B.Type;
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_ReflectionTypeHandle& Value)
        {
            F_Name Canonical;
            if (Value.Type)
            {
                Canonical = Value.Type->GetCanonical();
            }
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Canonical);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, TF_ReflectionTypeHandle& Value)
        {
            F_Name Key;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Key);
            if (Key)
            {
                auto Item = GlobalSearchReflectionItem(Key);
                TW<F_ReflectionType> Type;
                if (!Item.TryDynamicCast<F_ReflectionType>(Type))
                {
                    return F_FeedbackStatus::MakeFailed(
                        ABYTEK_TEXT("Invalid item being assigned to ")
                        + ToText(TypeFullName<TF_ReflectionTypeHandle>())
                        + ABYTEK_TEXT(": ")
                        + *Key
                    );
                }
                if (!CheckType(Type))
                {
                    return F_FeedbackStatus::MakeFailed(
                        ABYTEK_TEXT("Invalid type being assigned to ")
                        + ToText(TypeFullName<TF_ReflectionTypeHandle>())
                        + ABYTEK_TEXT(": ")
                        + *Key
                    );
                }
                Value = Type.FastCast<F_ReflectionType>();
            }
            else
            {
                Value = {};
            }
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    template<
        typename __F,
        typename __F_Reflector
    >
    F_ReflectionTypeBuildParams MakeReflectionTypeBuildParams(const TW_Valid<F_ReflectionContext>& Context)
    {
        using F_NoConst = std::remove_const_t<__F>;
        
        F_ReflectionTypeBuildParams Result;
        Result.Context = Context;
        Result.Desc = F_ReflectionTypeDesc::Make<__F>();
        Result.AdditionalInfo = F_ReflectionTypeAdditionalInfo::Make<__F, __F_Reflector>();

        if constexpr (std::is_const_v<__F>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::CONSTANT;
        }

        if constexpr (std::is_pointer_v<__F>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::POINTER;
        }

        if constexpr (std::is_reference_v<__F>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::REFERENCE;
            Result.IsRightValueReference = std::is_rvalue_reference_v<__F>;
        }

        if constexpr (std::is_function_v<F_NoConst>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::RAW_FUNCTION;
        }

        if constexpr (std::is_same_v<F_NoConst, U8>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::U8;
        }
        if constexpr (std::is_same_v<F_NoConst, U16>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::U16;
        }
        if constexpr (std::is_same_v<F_NoConst, U32>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::U32;
        }
        if constexpr (std::is_same_v<F_NoConst, U64>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::U64;
        }

        if constexpr (std::is_same_v<F_NoConst, I8>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::I8;
        }
        if constexpr (std::is_same_v<F_NoConst, I16>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::I16;
        }
        if constexpr (std::is_same_v<F_NoConst, I32>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::I32;
        }
        if constexpr (std::is_same_v<F_NoConst, I64>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::I64;
        }

        if constexpr (std::is_same_v<F_NoConst, B8>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::B8;
        }

        if constexpr (std::is_same_v<F_NoConst, F_Char>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::CHAR;
        }
        if constexpr (std::is_same_v<F_NoConst, F_TextChar>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::TEXT_CHAR;
        }

        if constexpr (std::is_same_v<F_NoConst, Sz>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::SZ;
        }
        if constexpr (std::is_same_v<F_NoConst, PDiff>)
        {
            Result.TypeFlags = E_ReflectionTypeFlag::PRIMITIVE;
            Result.PrimitiveType = E_ReflectionPrimitiveType::PDIFF;
        }

        Result.TypeFlags = E_ReflectionTypeFlag::DATA;    
        return ABYTEK_MOVE(Result);
    };
}

namespace boost
{
    template<typename __F>
    struct hash<Abytek::TF_ReflectionTypeHandle<__F>>
    {
        size_t operator () (const Abytek::TF_ReflectionTypeHandle<__F>& Value) const
        {
            return boost::hash<Abytek::TW<Abytek::F_ReflectionType>>()(Value.Type);
        }
    };
}