#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"
#include "Abytek/JSONOptions.hpp"
#include "Abytek/JSONContext.hpp"
#include "Abytek/JSONConvertChecks.hpp"
#include "Abytek/JSONConvertDeclaration.hpp"


namespace Abytek
{
    struct I_JSONSerializable;
    
    namespace Internal::JSONSerializable
    {
        template<typename __F, typename = void>
        struct TH_PropertyCompare
        {
            static B8 Check(const __F& A, const __F& B)
            {
                return false;
            }
        };
        template<typename __F>
        struct TH_PropertyCompare<
            __F, 
            std::void_t<
                decltype(std::declval<const __F&>() == std::declval<const __F&>())
            >
        >
        {
            static B8 Check(const __F& A, const __F& B)
            {
                return A == B;
            }
        };
    }
    
    template<typename __F_Value>
    static constexpr B8 IsJSONSerializable_ByReflection()
    {
        return (
            IsReflectionBaseOf<I_JSONSerializable, std::remove_const_t<__F_Value>>()
            && !std::is_same_v<I_JSONSerializable, std::remove_const_t<__F_Value>>
            && !std::is_enum_v<std::remove_const_t<__F_Value>>
        );
    }
    
    struct ABYTEK_BASE_OBJECT_API I_JSONSerializable
    {
        using F_Metadata_ValueToBoostJSONValue = TF_Function<F_FeedbackStatus(const void* DataPtr, boost::json::value& JSONValue, F_JSONContext& JSONContext)>;
        using F_Metadata_BoostJSONValueToValue = TF_Function<F_FeedbackStatus(void* DataPtr, const boost::json::value& JSONValue, F_JSONContext& JSONContext)>;
        using F_Metadata_JSONProperty_Compare = TF_Function<B8(const void* APtr, const void* BPtr)>;
        static F_Name GetMetadataElementName_ValueToBoostJSONValue()
        {
            return ABYTEK_NAME("Abytek::ValueToBoostJSONValue");
        }
        static F_Name GetMetadataElementName_BoostJSONValueToValue()
        {
            return ABYTEK_NAME("Abytek::BoostJSONValueToValue");
        }
        static F_Name GetMetadataElementName_Serializable_Auto()
        {
            return ABYTEK_NAME("Abytek::JSONSerializable_Auto");
        }
        static F_Name GetMetadataElementName_Serializable_DevelopmentBuild()
        {
            return ABYTEK_NAME("Abytek::JSONSerializable_DevelopmentBuild");
        }
        static F_Name GetMetadataElementName_Serializable_NonDevelopmentBuild()
        {
            return ABYTEK_NAME("Abytek::JSONSerializable_NonDevelopmentBuild");
        }
        static F_Name GetMetadataElementName_JSONProperty_Compare()
        {
            return ABYTEK_NAME("Abytek::JSONProperty_Compare");
        }
        
        ABYTEK_BEGIN_REFLECTOR()
            ABYTEK_REFLECT_GEN_PROPERTY_INTERFACE()
            {
                if (!IsJSONSerializable<__F_Member>())
                {
                    return;
                }
            
                auto& Metadata = ReflectionProperty.Metadata;
                Metadata.Add(
                    GetMetadataElementName_ValueToBoostJSONValue(),    
                    F_Metadata_ValueToBoostJSONValue(
                        [](const void* DataPtr, boost::json::value& JSONValue, F_JSONContext& JSONContext) -> F_FeedbackStatus
                        {
                            using F_NonReferenceMember = std::remove_reference_t<__F_Member>;
                            const F_NonReferenceMember& Target = *(const F_NonReferenceMember*)DataPtr;
                            ABYTEK_FEEDBACK_STATUS_CHECK(TH_JSONConvert<F_NonReferenceMember>::ValueToBoostJSONValue(Target, JSONValue, JSONContext));
                            return F_FeedbackStatus::MakeSucceeded();
                        }
                    )
                );
                Metadata.Add(
                    GetMetadataElementName_BoostJSONValueToValue(),    
                    F_Metadata_BoostJSONValueToValue(
                        [](void* DataPtr, const boost::json::value& JSONValue, F_JSONContext& JSONContext) -> F_FeedbackStatus
                        {
                            using F_NonReferenceMember = std::remove_reference_t<__F_Member>;
                            F_NonReferenceMember& Target = *(F_NonReferenceMember*)DataPtr;
                            ABYTEK_FEEDBACK_STATUS_CHECK(TH_JSONConvert<F_NonReferenceMember>::BoostJSONValueToValue(Target, JSONValue, JSONContext));
                            return F_FeedbackStatus::MakeSucceeded();
                        }
                    )
                );
                Metadata.Add(
                    GetMetadataElementName_JSONProperty_Compare(),    
                    F_Metadata_JSONProperty_Compare(
                        [](const void* APtr, const void* BPtr) -> B8
                        {
                            using F_NonReferenceMember = std::remove_reference_t<__F_Member>;
                            F_NonReferenceMember& ATarget = *(F_NonReferenceMember*)APtr;
                            F_NonReferenceMember& BTarget = *(F_NonReferenceMember*)BPtr;
                            return Internal::JSONSerializable::TH_PropertyCompare<std::remove_const_t<__F_Member>>::Check(ATarget, BTarget);
                        }
                    )
                );
            }
            ABYTEK_REFLECT_GEN_INTERFACE()
            {
                auto& Metadata = ReflectionType->GetMetadata();
                Metadata.Add(
                    GetMetadataElementName_ValueToBoostJSONValue(),    
                    F_Metadata_ValueToBoostJSONValue(
                        [ReflectionType](const void* DataPtr, boost::json::value& JSONValue, F_JSONContext& JSONContext) -> F_FeedbackStatus
                        {
                            B8 IsTopLevel = JSONContext.IsTopLevel;
                            JSONContext.IsTopLevel = false;
                            
                            boost::json::object JSONObject;
                            auto ProcessProperty = [&](const TW<F_ReflectionType>& PropertyType, const F_ReflectionProperty& Property, void* InstancePtr, U32 InstanceIndex) -> F_FeedbackStatus
                            {
                                const auto& PropertyMetadata = Property.Metadata;
                                auto PropertyMetadataElementName_ValueToBoostJSONValue = GetMetadataElementName_ValueToBoostJSONValue();
                                {
                                    B8 HasAllRequiredTags = true;
                                    for (const auto& RequiredTag : JSONContext.Options->RequiredPropertyTags)
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
                                
                                if (JSONContext.Options->SkipNonDirtyProperties && JSONContext.IsTopLevel)
                                {
                                    if (!JSONContext.Options->CDO)
                                    {
                                        return F_FeedbackStatus::MakeFailed(
                                            ABYTEK_TEXT("Requires CDO JSON options to check skipping non-dirty properties")  
                                        );
                                    }
                                    
                                    void* CDOInstancePtr = nullptr;
                                    ABYTEK_FEEDBACK_STATUS_CHECK(
                                        ReflectionType->CastInstancePtr(JSONContext.Options->CDO, InstanceIndex, CDOInstancePtr)
                                    );
                                    
                                    const void* CDOPropertyTarget = &Property.ReferenceAccess<U8>(CDOInstancePtr);
                                    const void* PropertyTarget = &Property.ReferenceAccess<U8>(InstancePtr);
                                    
                                    if (auto MetadataElementHandle_JSONProperty_Compare = PropertyMetadata.FindElement(GetMetadataElementName_JSONProperty_Compare()))
                                    {
                                        const auto& JSONProperty_Compare = AnyCast<F_Metadata_JSONProperty_Compare>(
                                            MetadataElementHandle_JSONProperty_Compare->Value    
                                        );
                                        if (JSONProperty_Compare(CDOPropertyTarget, PropertyTarget))
                                        {
                                            return F_FeedbackStatus::MakeSucceeded();
                                        }
                                    }
                                    else
                                    {
                                        return F_FeedbackStatus::MakeFailed(
                                            ABYTEK_TEXT("Missing JSON property compare metadata: ") + *Property.Name  
                                        );
                                    }
                                }
                                
                                B8 IsSerializable_Auto = PropertyMetadata.HasElement(GetMetadataElementName_Serializable_Auto());
                                B8 IsSerializable_DevelopmentBuild = PropertyMetadata.HasElement(GetMetadataElementName_Serializable_DevelopmentBuild());
                                B8 IsSerializable_NonDevelopmentBuild = PropertyMetadata.HasElement(GetMetadataElementName_Serializable_NonDevelopmentBuild());
                                if (
                                    (JSONContext.Options->HasDevelopmentBuild && IsSerializable_DevelopmentBuild)
                                    || (!JSONContext.Options->HasDevelopmentBuild && IsSerializable_NonDevelopmentBuild)
                                    || (
                                        IsSerializable_Auto 
                                        && (
                                            (IsTopLevel && JSONContext.Options->HasDevelopmentBuild)
                                            || !IsTopLevel
                                        )
                                    )
                                )
                                {
                                    const auto& PropertyMetadataElement_ValueToBoostJSONValue = PropertyMetadata.Get(
                                        PropertyMetadataElementName_ValueToBoostJSONValue
                                    );
                                    const auto& PropertyMetadata_ValueToBoostJSONValue = AnyCast<F_Metadata_ValueToBoostJSONValue>(
                                        PropertyMetadataElement_ValueToBoostJSONValue
                                    );
                                    
                                    boost::json::value PropertyJSONValue;
                                    const void* PropertyTarget = &Property.ReferenceAccess<U8>(InstancePtr);
                                    ABYTEK_FEEDBACK_STATUS_CHECK(
                                        PropertyMetadata_ValueToBoostJSONValue(
                                            PropertyTarget,
                                            PropertyJSONValue,
                                            JSONContext
                                        )
                                    );
                                    
                                    JSONObject[boost::json::string(ToString(*Property.Name).c_str())] = PropertyJSONValue;
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
                                            if (auto Status = ProcessProperty(Type, Property, InstancePtr, InstanceIndex); !Status)
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
                            
                            JSONValue = boost::json::value(JSONObject);
                            return F_FeedbackStatus::MakeSucceeded();
                        }
                    )
                );
                Metadata.Add(
                    GetMetadataElementName_BoostJSONValueToValue(),    
                    F_Metadata_BoostJSONValueToValue(
                        [ReflectionType](void* DataPtr, const boost::json::value& JSONValue, F_JSONContext& JSONContext) -> F_FeedbackStatus
                        {
                            B8 IsTopLevel = JSONContext.IsTopLevel;
                            JSONContext.IsTopLevel = false;
                            
                            if (!JSONValue.is_object())
                            {
                                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Requires json object"));
                            }
                            const boost::json::object& JSONObject = JSONValue.as_object();
                            auto ProcessProperty = [&](const F_ReflectionProperty& Property, void* InstancePtr) -> F_FeedbackStatus
                            {
                                if (
                                    auto PropertyJSONValueHandle = JSONObject.find(boost::json::string(ToString(*Property.Name).c_str()));
                                    PropertyJSONValueHandle != JSONObject.end()
                                )
                                {
                                    const auto& PropertyMetadata = Property.Metadata;
                                    auto PropertyMetadataElementName_BoostJSONValueToValue = GetMetadataElementName_BoostJSONValueToValue();
                                    {
                                        B8 HasAllRequiredTags = true;
                                        for (const auto& RequiredTag : JSONContext.Options->RequiredPropertyTags)
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
                                        (JSONContext.Options->HasDevelopmentBuild && IsSerializable_DevelopmentBuild)
                                        || (!JSONContext.Options->HasDevelopmentBuild && IsSerializable_NonDevelopmentBuild)
                                        || (
                                            IsSerializable_Auto 
                                            && (
                                                (IsTopLevel && JSONContext.Options->HasDevelopmentBuild)
                                                || !IsTopLevel
                                            )
                                        )
                                    )
                                    {
                                        const auto& PropertyMetadataElement_BoostJSONValueToValue = PropertyMetadata.Get(
                                            PropertyMetadataElementName_BoostJSONValueToValue
                                        );
                                        const auto& PropertyMetadata_BoostJSONValueToValue = AnyCast<F_Metadata_BoostJSONValueToValue>(
                                            PropertyMetadataElement_BoostJSONValueToValue
                                        );
                                    
                                        void* PropertyTarget = &Property.ReferenceAccess<U8>(InstancePtr);
                                        ABYTEK_FEEDBACK_STATUS_CHECK(
                                            PropertyMetadata_BoostJSONValueToValue(
                                                PropertyTarget,
                                                PropertyJSONValueHandle->value(),
                                                JSONContext
                                            )
                                        );
                                    }
                                };
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
        ABYTEK_END_REFLECTOR(I_JSONSerializable);
    };
}

#define ABYTEK_REFLECT_PROPERTY_ENABLE_JSON_SERIALIZABLE_AUTO(MemberName) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    auto& Property = ReflectionType->GetProperty(ABYTEK_NAME(#MemberName)); \
                    auto MetadataElementName = Abytek::I_JSONSerializable::GetMetadataElementName_Serializable_Auto(); \
                    if (!Property.Metadata.HasElement(MetadataElementName)) \
                    { \
                        Property.Metadata.Add(MetadataElementName, F_Any()); \
                    } \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY_ENABLE_JSON_SERIALIZABLE_DEV(MemberName) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    auto& Property = ReflectionType->GetProperty(ABYTEK_NAME(#MemberName)); \
                    auto MetadataElementName = Abytek::I_JSONSerializable::GetMetadataElementName_Serializable_DevelopmentBuild(); \
                    if (!Property.Metadata.HasElement(MetadataElementName)) \
                    { \
                        Property.Metadata.Add(MetadataElementName, F_Any()); \
                    } \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY_ENABLE_JSON_SERIALIZABLE_NON_DEV(MemberName) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    auto& Property = ReflectionType->GetProperty(ABYTEK_NAME(#MemberName)); \
                    auto MetadataElementName = Abytek::I_JSONSerializable::GetMetadataElementName_Serializable_NonDevelopmentBuild(); \
                    if (!Property.Metadata.HasElement(MetadataElementName)) \
                    { \
                        Property.Metadata.Add(MetadataElementName, F_Any()); \
                    } \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY_ENABLE_JSON_SERIALIZABLE(MemberName) \
                ABYTEK_REFLECT_PROPERTY_ENABLE_JSON_SERIALIZABLE_DEV() \
                ABYTEK_REFLECT_PROPERTY_ENABLE_JSON_SERIALIZABLE_NON_DEV()

#define ABYTEK_REFLECT_PROPERTY_DISABLE_JSON_SERIALIZABLE_AUTO(MemberName) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    auto& Property = ReflectionType->GetProperty(ABYTEK_NAME(#MemberName)); \
                    auto MetadataElementName = Abytek::I_JSONSerializable::GetMetadataElementName_Serializable_Auto(); \
                    if (!Property.Metadata.HasElement(MetadataElementName)) \
                    { \
                        Property.Metadata.Remove(MetadataElementName); \
                    } \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY_DISABLE_JSON_SERIALIZABLE_DEV(MemberName) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    auto& Property = ReflectionType->GetProperty(ABYTEK_NAME(#MemberName)); \
                    auto MetadataElementName = Abytek::I_JSONSerializable::GetMetadataElementName_Serializable_DevelopmentBuild(); \
                    if (!Property.Metadata.HasElement(MetadataElementName)) \
                    { \
                        Property.Metadata.Remove(MetadataElementName); \
                    } \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY_DISABLE_JSON_SERIALIZABLE_NON_DEV(MemberName) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    auto& Property = ReflectionType->GetProperty(ABYTEK_NAME(#MemberName)); \
                    auto MetadataElementName = Abytek::I_JSONSerializable::GetMetadataElementName_Serializable_NonDevelopmentBuild(); \
                    if (!Property.Metadata.HasElement(MetadataElementName)) \
                    { \
                        Property.Metadata.Remove(MetadataElementName); \
                    } \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY_DISABLE_JSON_SERIALIZABLE(MemberName) \
                ABYTEK_REFLECT_PROPERTY_DISABLE_JSON_SERIALIZABLE_DEV() \
                ABYTEK_REFLECT_PROPERTY_DISABLE_JSON_SERIALIZABLE_NON_DEV()

#define ABYTEK_REFLECT_PROPERTY_JSON_SERIALIZABLE_ADVANCED(MemberName, ...) \
            ABYTEK_REFLECT_PROPERTY_ADVANCED(MemberName, __VA_ARGS__) \
            ABYTEK_REFLECT_PROPERTY_ENABLE_JSON_SERIALIZABLE(MemberName)

#define ABYTEK_REFLECT_PROPERTY_JSON_SERIALIZABLE(MemberName) \
            ABYTEK_REFLECT_PROPERTY(MemberName) \
            ABYTEK_REFLECT_PROPERTY_ENABLE_JSON_SERIALIZABLE(MemberName)