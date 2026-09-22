#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"


namespace Abytek
{
    struct I_BinarySerializable;
    
    template<typename __F_Value>
    static constexpr B8 IsBinarySerializable_ByReflection()
    {
        return (
            IsReflectionBaseOf<I_BinarySerializable, std::remove_const_t<__F_Value>>()
            && !std::is_same_v<I_BinarySerializable, std::remove_const_t<__F_Value>>
            && !std::is_enum_v<std::remove_const_t<__F_Value>>
        );
    }
    
    namespace Internal::BinarySerializable
    {
        template<B8 __Support, typename __F_Value>
        struct TH_AddMetadataElement_Serialize;
        template<typename __F_Value>
        struct TH_AddMetadataElement_Serialize<false, __F_Value>
        {
            template<typename __F_Functor>
            static void Invoke(F_ReflectionMetadata& Metadata, const F_Name& MetadataElementName)
            {
                Metadata.Add(
                    MetadataElementName,    
                    __F_Functor(
                        [](F_ArchiveReadWriteView& View, const void* DataPtr) -> F_FeedbackStatus
                        {
                            return F_FeedbackStatus::MakeFailed(
                                ABYTEK_TEXT("Type does not support serialization: ") 
                                + ToText(TypeFullName<__F_Value>())
                            );
                        }
                    )
                );
            }
        };
        template<typename __F_Value>
        struct TH_AddMetadataElement_Serialize<true, __F_Value>
        {
            template<typename __F_Functor>
            static void Invoke(F_ReflectionMetadata& Metadata, const F_Name& MetadataElementName)
            {
                Metadata.Add(
                    MetadataElementName,    
                    __F_Functor(
                        [](F_ArchiveReadWriteView& View, const void* DataPtr) -> F_FeedbackStatus
                        {
                            using F_NonReferenceMember = std::remove_reference_t<__F_Value>;
                            const F_NonReferenceMember& Target = *(const F_NonReferenceMember*)DataPtr;
                            return View << Target;
                        }
                    )
                );
            }
        };
        
        template<B8 __Support, typename __F_Value>
        struct TH_AddMetadataElement_Deserialize;
        template<typename __F_Value>
        struct TH_AddMetadataElement_Deserialize<false, __F_Value>
        {
            template<typename __F_Functor>
            static void Invoke(F_ReflectionMetadata& Metadata, const F_Name& MetadataElementName)
            {
                Metadata.Add(
                    MetadataElementName,    
                    __F_Functor(
                        [](F_ArchiveReadOnlyView& View, void* DataPtr) -> F_FeedbackStatus
                        {
                            return F_FeedbackStatus::MakeFailed(
                                ABYTEK_TEXT("Type does not support deserialization: ") 
                                + ToText(TypeFullName<__F_Value>())
                            );
                        }
                    )
                );
            }
        };
        template<typename __F_Value>
        struct TH_AddMetadataElement_Deserialize<true, __F_Value>
        {
            template<typename __F_Functor>
            static void Invoke(F_ReflectionMetadata& Metadata, const F_Name& MetadataElementName)
            {
                Metadata.Add(
                    MetadataElementName,    
                    __F_Functor(
                        [](F_ArchiveReadOnlyView& View, void* DataPtr) -> F_FeedbackStatus
                        {
                            using F_NonReferenceMember = std::remove_reference_t<__F_Value>;
                            F_NonReferenceMember& Target = *(F_NonReferenceMember*)DataPtr;
                            return View >> Target;
                        }
                    )
                );
            }
        };
    }
    
    struct ABYTEK_BASE_SERIALIZABLE_API I_BinarySerializable 
    {
        using F_Metadata_Serialize = TF_Function<F_FeedbackStatus(F_ArchiveReadWriteView& View, const void* DataPtr)>;
        using F_Metadata_Deserialize = TF_Function<F_FeedbackStatus(F_ArchiveReadOnlyView& View, void* DataPtr)>;
        static F_Name GetMetadataElementName_Serialize()
        {
            return ABYTEK_NAME("Abytek::BinarySerialize");
        }
        static F_Name GetMetadataElementName_Deserialize()
        {
            return ABYTEK_NAME("Abytek::BinaryDeserialize");
        }
        static F_Name GetMetadataElementName_Serializable_Auto()
        {
            return ABYTEK_NAME("Abytek::BinarySerializable_Auto");
        }
        static F_Name GetMetadataElementName_Serializable_DevelopmentBuild()
        {
            return ABYTEK_NAME("Abytek::BinarySerializable_DevelopmentBuild");
        }
        static F_Name GetMetadataElementName_Serializable_NonDevelopmentBuild()
        {
            return ABYTEK_NAME("Abytek::BinarySerializable_NonDevelopmentBuild");
        }
        
        ABYTEK_BEGIN_REFLECTOR()
            ABYTEK_REFLECT_GEN_PROPERTY_INTERFACE()
            {
                auto& Metadata = ReflectionProperty.Metadata;
                Internal::BinarySerializable::TH_AddMetadataElement_Serialize<CanWriteArchive<__F_Member>(), __F_Member>::template Invoke<F_Metadata_Serialize>(
                    Metadata,
                    GetMetadataElementName_Serialize()
                );
                Internal::BinarySerializable::TH_AddMetadataElement_Deserialize<CanReadArchive<__F_Member>(), __F_Member>::template Invoke<F_Metadata_Deserialize>(
                    Metadata,
                    GetMetadataElementName_Deserialize()
                );
            }
            ABYTEK_REFLECT_GEN_INTERFACE()
            {
                auto& Metadata = ReflectionType->GetMetadata();
                Metadata.Add(
                    GetMetadataElementName_Serialize(),    
                    F_Metadata_Serialize(
                        [ReflectionType](F_ArchiveReadWriteView& View, const void* DataPtr) -> F_FeedbackStatus
                        {
                            B8 IsTopLevel = View.IsTopLevel;
                            View.IsTopLevel = false;
                            
                            auto ProcessProperty = [&](const F_ReflectionProperty& Property, void* InstancePtr, U32 InstanceIndex) -> F_FeedbackStatus
                            {
                                const auto& PropertyMetadata = Property.Metadata;
                                auto PropertyMetadataElementName_Serialize = GetMetadataElementName_Serialize();
                                {
                                    B8 HasAllRequiredTags = true;
                                    for (const auto& RequiredTag : View.RequiredPropertyTags)
                                    {
                                        if (!PropertyMetadata.HasElement(RequiredTag))
                                        {
                                            HasAllRequiredTags = false;
                                            break;
                                        }
                                    }
                                    if (!HasAllRequiredTags)
                                    {
                                        return F_FeedbackStatus::MakeSucceeded();
                                    }
                                }
                                
                                B8 IsSerializable_Auto = PropertyMetadata.HasElement(GetMetadataElementName_Serializable_Auto());
                                B8 IsSerializable_DevelopmentBuild = PropertyMetadata.HasElement(GetMetadataElementName_Serializable_DevelopmentBuild());
                                B8 IsSerializable_NonDevelopmentBuild = PropertyMetadata.HasElement(GetMetadataElementName_Serializable_NonDevelopmentBuild());
                                if (
                                    PropertyMetadata.HasElement(PropertyMetadataElementName_Serialize)
                                    && (
                                        (View.HasDevelopmentBuild && IsSerializable_DevelopmentBuild)
                                        || (!View.HasDevelopmentBuild && IsSerializable_NonDevelopmentBuild)
                                        || (
                                            IsSerializable_Auto 
                                            && (
                                                (IsTopLevel && !View.HasDevelopmentBuild)
                                                || !IsTopLevel
                                            )
                                        )
                                    )
                                )
                                {
                                    const auto& PropertyMetadataElement_Serialize = PropertyMetadata.Get(
                                        PropertyMetadataElementName_Serialize
                                    );
                                    const auto& PropertyMetadata_Serialize = AnyCast<F_Metadata_Serialize>(
                                        PropertyMetadataElement_Serialize
                                    );
                                    
                                    boost::json::value PropertyJSONValue;
                                    const void* PropertyTarget = &Property.ReferenceAccess<U8>(InstancePtr);
                                    ABYTEK_FEEDBACK_STATUS_CHECK(
                                        PropertyMetadata_Serialize(
                                            View,
                                            PropertyTarget
                                        )
                                    );
                                }
                                return F_FeedbackStatus::MakeSucceeded();
                            };
                            
                            {
                                F_FeedbackStatus AllPropertiesStatus = F_FeedbackStatus::MakeSucceeded();
                                ReflectionType->ForEachRepresentationOnInstance(
                                    [&](const TW_Valid<F_ReflectionType>& Type, void* InstancePtr, U32 InstanceIndex) -> B8
                                    {
                                        for (const auto& Property : Type->GetProperties())
                                        {
                                            if (auto Status = ProcessProperty(Property, InstancePtr, InstanceIndex); !Status)
                                            {
                                                AllPropertiesStatus = Status;
                                                return false; 
                                            }
                                        }
                                        return true;
                                    },
                                    (void*)DataPtr
                                );
                                ABYTEK_FEEDBACK_STATUS_CHECK(AllPropertiesStatus);
                            }
                            
                            return F_FeedbackStatus::MakeSucceeded();
                        }
                    )
                );
                Metadata.Add(
                    GetMetadataElementName_Deserialize(),    
                    F_Metadata_Deserialize(
                        [ReflectionType](F_ArchiveReadOnlyView& View, void* DataPtr) -> F_FeedbackStatus
                        {
                            B8 IsTopLevel = View.IsTopLevel;
                            View.IsTopLevel = false;
                            
                            auto ProcessProperty = [&](const F_ReflectionProperty& Property, void* InstancePtr) -> F_FeedbackStatus
                            {
                                const auto& PropertyMetadata = Property.Metadata;
                                auto PropertyMetadataElementName_Deserialize = GetMetadataElementName_Deserialize();
                                {
                                    B8 HasAllRequiredTags = true;
                                    for (const auto& RequiredTag : View.RequiredPropertyTags)
                                    {
                                        if (!PropertyMetadata.HasElement(RequiredTag))
                                        {
                                            HasAllRequiredTags = false;
                                            break;
                                        }
                                    }
                                    if (!HasAllRequiredTags)
                                    {
                                        return F_FeedbackStatus::MakeSucceeded();
                                    }
                                }
                                
                                B8 IsSerializable_Auto = PropertyMetadata.HasElement(GetMetadataElementName_Serializable_Auto());
                                B8 IsSerializable_DevelopmentBuild = PropertyMetadata.HasElement(GetMetadataElementName_Serializable_DevelopmentBuild());
                                B8 IsSerializable_NonDevelopmentBuild = PropertyMetadata.HasElement(GetMetadataElementName_Serializable_NonDevelopmentBuild());
                                if (
                                    PropertyMetadata.HasElement(PropertyMetadataElementName_Deserialize)
                                    && (
                                        (View.HasDevelopmentBuild && IsSerializable_DevelopmentBuild)
                                        || (!View.HasDevelopmentBuild && IsSerializable_NonDevelopmentBuild)
                                        || (
                                            IsSerializable_Auto 
                                            && (
                                                (IsTopLevel && !View.HasDevelopmentBuild)
                                                || !IsTopLevel
                                            )
                                        )
                                    )
                                )
                                {
                                    const auto& PropertyMetadataElement_Deserialize = PropertyMetadata.Get(
                                        PropertyMetadataElementName_Deserialize
                                    );
                                    const auto& PropertyMetadata_Deserialize = AnyCast<F_Metadata_Deserialize>(
                                        PropertyMetadataElement_Deserialize
                                    );
                                
                                    void* PropertyTarget = &Property.ReferenceAccess<U8>(InstancePtr);
                                    ABYTEK_FEEDBACK_STATUS_CHECK(
                                        PropertyMetadata_Deserialize(
                                            View,
                                            PropertyTarget
                                        )
                                    );
                                }
                                return F_FeedbackStatus::MakeSucceeded();
                            };
                            
                            {
                                F_FeedbackStatus AllPropertiesStatus = F_FeedbackStatus::MakeSucceeded();
                                ReflectionType->ForEachRepresentationOnInstance(
                                    [&](const TW_Valid<F_ReflectionType>& Type, void* InstancePtr, U32 InstanceIndex) -> B8
                                    {
                                        for (const auto& Property : Type->GetProperties())
                                        {
                                            if (auto Status = ProcessProperty(Property, InstancePtr); !Status)
                                            {
                                                AllPropertiesStatus = Status;
                                                return false; 
                                            }
                                        }
                                        return true;
                                    },
                                    DataPtr
                                );
                                ABYTEK_FEEDBACK_STATUS_CHECK(AllPropertiesStatus);
                            }
                            
                            return F_FeedbackStatus::MakeSucceeded();
                        }
                    )
                );
            }
        ABYTEK_END_REFLECTOR(I_BinarySerializable);
    };
    
    template<typename __F_Value, std::enable_if_t<IsBinarySerializable_ByReflection<__F_Value>(), int> = 0>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, __F_Value& Value) noexcept
    {
        auto ReflectionType = TF_ReflectionTypeHandle<__F_Value>(F_ReflectionContext::GetGlobal());
        const F_ReflectionMetadata& Metadata = ReflectionType->GetMetadata();
        if (!Metadata.HasElement(I_BinarySerializable::GetMetadataElementName_Deserialize()))
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not found deserialize metadata"));
        }
        const auto& Deserialize = AnyCast<I_BinarySerializable::F_Metadata_Deserialize>(
            Metadata.Get(I_BinarySerializable::GetMetadataElementName_Deserialize())
        );
        ABYTEK_FEEDBACK_STATUS_CHECK(Deserialize(View, &Value));
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Value, std::enable_if_t<IsBinarySerializable_ByReflection<__F_Value>(), int> = 0>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const __F_Value& Value) noexcept
    {
        auto ReflectionType = TF_ReflectionTypeHandle<__F_Value>(F_ReflectionContext::GetGlobal());
        const F_ReflectionMetadata& Metadata = ReflectionType->GetMetadata();
        if (!Metadata.HasElement(I_BinarySerializable::GetMetadataElementName_Serialize()))
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not found serialize metadata"));
        }
        const auto& Serialize = AnyCast<I_BinarySerializable::F_Metadata_Serialize>(
            Metadata.Get(I_BinarySerializable::GetMetadataElementName_Serialize())
        );
        ABYTEK_FEEDBACK_STATUS_CHECK(Serialize(View, &Value));
        return F_FeedbackStatus::MakeSucceeded();
    }
}

#define ABYTEK_REFLECT_PROPERTY_ENABLE_BINARY_SERIALIZABLE_AUTO(MemberName) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    auto& Property = ReflectionType->GetProperty(ABYTEK_NAME(#MemberName)); \
                    auto MetadataElementName = Abytek::I_BinarySerializable::GetMetadataElementName_Serializable_Auto(); \
                    if (!Property.Metadata.HasElement(MetadataElementName)) \
                    { \
                        Property.Metadata.Add(MetadataElementName, F_Any()); \
                    } \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY_ENABLE_BINARY_SERIALIZABLE_DEV(MemberName) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    auto& Property = ReflectionType->GetProperty(ABYTEK_NAME(#MemberName)); \
                    auto MetadataElementName = Abytek::I_BinarySerializable::GetMetadataElementName_Serializable_DevelopmentBuild(); \
                    if (!Property.Metadata.HasElement(MetadataElementName)) \
                    { \
                        Property.Metadata.Add(MetadataElementName, F_Any()); \
                    } \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY_ENABLE_BINARY_SERIALIZABLE_NON_DEV(MemberName) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    auto& Property = ReflectionType->GetProperty(ABYTEK_NAME(#MemberName)); \
                    auto MetadataElementName = Abytek::I_BinarySerializable::GetMetadataElementName_Serializable_NonDevelopmentBuild(); \
                    if (!Property.Metadata.HasElement(MetadataElementName)) \
                    { \
                        Property.Metadata.Add(MetadataElementName, F_Any()); \
                    } \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY_ENABLE_BINARY_SERIALIZABLE(MemberName) \
                ABYTEK_REFLECT_PROPERTY_ENABLE_BINARY_SERIALIZABLE_DEV() \
                ABYTEK_REFLECT_PROPERTY_ENABLE_BINARY_SERIALIZABLE_NON_DEV()

#define ABYTEK_REFLECT_PROPERTY_DISABLE_BINARY_SERIALIZABLE_AUTO(MemberName) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    auto& Property = ReflectionType->GetProperty(ABYTEK_NAME(#MemberName)); \
                    auto MetadataElementName = Abytek::I_BinarySerializable::GetMetadataElementName_Serializable_Auto(); \
                    if (!Property.Metadata.HasElement(MetadataElementName)) \
                    { \
                        Property.Metadata.Remove(MetadataElementName); \
                    } \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY_DISABLE_BINARY_SERIALIZABLE_DEV(MemberName) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    auto& Property = ReflectionType->GetProperty(ABYTEK_NAME(#MemberName)); \
                    auto MetadataElementName = Abytek::I_BinarySerializable::GetMetadataElementName_Serializable_DevelopmentBuild(); \
                    if (!Property.Metadata.HasElement(MetadataElementName)) \
                    { \
                        Property.Metadata.Remove(MetadataElementName); \
                    } \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY_DISABLE_BINARY_SERIALIZABLE_NON_DEV(MemberName) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    auto& Property = ReflectionType->GetProperty(ABYTEK_NAME(#MemberName)); \
                    auto MetadataElementName = Abytek::I_BinarySerializable::GetMetadataElementName_Serializable_NonDevelopmentBuild(); \
                    if (!Property.Metadata.HasElement(MetadataElementName)) \
                    { \
                        Property.Metadata.Remove(MetadataElementName); \
                    } \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY_DISABLE_BINARY_SERIALIZABLE(MemberName) \
                ABYTEK_REFLECT_PROPERTY_DISABLE_BINARY_SERIALIZABLE_DEV() \
                ABYTEK_REFLECT_PROPERTY_DISABLE_BINARY_SERIALIZABLE_NON_DEV()

#define ABYTEK_REFLECT_PROPERTY_BINARY_SERIALIZABLE_ADVANCED(MemberName, ...) \
            ABYTEK_REFLECT_PROPERTY_ADVANCED(MemberName, __VA_ARGS__) \
            ABYTEK_REFLECT_PROPERTY_ENABLE_BINARY_SERIALIZABLE(MemberName)

#define ABYTEK_REFLECT_PROPERTY_BINARY_SERIALIZABLE(MemberName) \
            ABYTEK_REFLECT_PROPERTY(MemberName) \
            ABYTEK_REFLECT_PROPERTY_ENABLE_BINARY_SERIALIZABLE(MemberName)