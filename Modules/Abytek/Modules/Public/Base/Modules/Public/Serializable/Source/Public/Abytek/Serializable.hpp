#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"
#include "Abytek/Reflection.hpp"
#include "Abytek/JSONSerializable.hpp"
#include "Abytek/BinarySerializable.hpp"


namespace Abytek
{
    class A_SerializableObject;
    
    struct F_SerializableTracingOptions
    {
    };
    
    namespace Internal::Serializable
    {
        struct F_Data_GatherReferencedSerializableObjects
        {
            TF_Set<TS<A_SerializableObject>>* OutObjects = nullptr;
            F_SerializableTracingOptions TracingOptions;
        };
        struct F_Data_GatherReferencedSerializableObjectPaths
        {
            TF_Set<F_Name>* OutObjectPaths = nullptr;
            F_SerializableTracingOptions TracingOptions;
        };
        
        template<typename __F_Type, typename>
        struct TH_GatherReferencedSerializableObjects;
        template<typename __F_Type, typename>
        struct TH_GatherReferencedSerializableObjectPaths;
    }
    
    template<typename __F_Type>
    void GatherReferencedSerializableObjects(TF_Set<TS<A_SerializableObject>>& OutObjects, const __F_Type& Value, const F_SerializableTracingOptions& TracingOptions = {})
    {
        Internal::Serializable::F_Data_GatherReferencedSerializableObjects Data;
        Data.OutObjects = &OutObjects;
        Data.TracingOptions = TracingOptions;
        DataTraverse<Internal::Serializable::TH_GatherReferencedSerializableObjects>(Data, *(__F_Type*)&Value);
    }
    template<typename __F_Type>
    void GatherReferencedSerializableObjectPaths(TF_Set<F_Name>& OutObjectPaths, const __F_Type& Value, const F_SerializableTracingOptions& TracingOptions = {})
    {
        Internal::Serializable::F_Data_GatherReferencedSerializableObjectPaths Data;
        Data.OutObjectPaths = &OutObjectPaths;
        Data.TracingOptions = TracingOptions;
        DataTraverse<Internal::Serializable::TH_GatherReferencedSerializableObjectPaths>(Data, *(__F_Type*)&Value);
    }
    
    struct ABYTEK_BASE_SERIALIZABLE_API I_Serializable
    {
    public:
        using F_GatherReferencedObjectsFunction = TF_Function<void(
            void* DataPtr,
            Internal::Serializable::F_Data_GatherReferencedSerializableObjects& Data
        )>;
        using F_GatherReferencedObjectPathsFunction = TF_Function<void(
            void* DataPtr,
            Internal::Serializable::F_Data_GatherReferencedSerializableObjectPaths& Data
        )>;
        
    public:
        static F_Name GetMetadataElementName_GatherReferencedObjectsFunction()
        {
            return ABYTEK_NAME("Abytek::I_Serializable::GatherReferencedObjectsFunction");
        }
        static F_Name GetMetadataElementName_GatherReferencedObjectPathsFunction()
        {
            return ABYTEK_NAME("Abytek::I_Serializable::GatherReferencedObjectPathsFunction");
        }
        
    public:
        ABYTEK_BEGIN_REFLECTOR(I_JSONSerializable, I_BinarySerializable)
            ABYTEK_REFLECT_GEN_PROPERTY_INTERFACE()
            {
                auto& Metadata = ReflectionProperty.Metadata;
                Metadata.Add(
                    GetMetadataElementName_GatherReferencedObjectsFunction(),
                    F_GatherReferencedObjectsFunction(
                        [](
                            void* DataPtr,
                            Internal::Serializable::F_Data_GatherReferencedSerializableObjects& Data
                        ) 
                        {
                            GatherReferencedSerializableObjects<__F_Member>(*Data.OutObjects, *(__F_Member*)DataPtr, Data.TracingOptions);
                        }
                    )
                );
                Metadata.Add(
                    GetMetadataElementName_GatherReferencedObjectPathsFunction(),
                    F_GatherReferencedObjectPathsFunction(
                        [](
                            void* DataPtr,
                            Internal::Serializable::F_Data_GatherReferencedSerializableObjectPaths& Data
                        ) 
                        {
                            GatherReferencedSerializableObjectPaths<__F_Member>(*Data.OutObjectPaths, *(__F_Member*)DataPtr, Data.TracingOptions);
                        }
                    )
                );
            }
            ABYTEK_REFLECT_GEN_INTERFACE()
            {
                auto& Metadata = ReflectionType->GetMetadata();
                Metadata.Add(
                    GetMetadataElementName_GatherReferencedObjectsFunction(),
                    F_GatherReferencedObjectsFunction(
                        [ReflectionType](
                            void* DataPtr,
                            Internal::Serializable::F_Data_GatherReferencedSerializableObjects& Data
                        ) 
                        {
                            auto ProcessProperty = [&](const F_ReflectionProperty& Property, void* InstancePtr)
                            {
                                const auto& PropertyMetadata = Property.Metadata;
                                const auto& PropertyMetadataElement = PropertyMetadata.Get(
                                    GetMetadataElementName_GatherReferencedObjectsFunction()    
                                );
                                const auto& PropertyGatherReferencedObjectsFunction = AnyCast<
                                    F_GatherReferencedObjectsFunction
                                >(PropertyMetadataElement);
                                void* PropertyTarget = &Property.ReferenceAccess<U8>(InstancePtr);
                                PropertyGatherReferencedObjectsFunction(PropertyTarget, Data);
                            };
                            
                            {
                                ReflectionType->ForEachRepresentationOnInstance(
                                    [&](const TW_Valid<F_ReflectionType>& Type, void* InstancePtr, U32 InstanceIndex) -> B8
                                    {
                                        for (const auto& Property : Type->GetProperties())
                                        {
                                            ProcessProperty(Property, InstancePtr);
                                        }
                                        return true;
                                    },
                                    DataPtr
                                );
                            }
                        }
                    )
                );
                Metadata.Add(
                    GetMetadataElementName_GatherReferencedObjectPathsFunction(),
                    F_GatherReferencedObjectPathsFunction(
                        [ReflectionType](
                            void* DataPtr,
                            Internal::Serializable::F_Data_GatherReferencedSerializableObjectPaths& Data
                        ) 
                        {
                            auto ProcessProperty = [&](const F_ReflectionProperty& Property, void* InstancePtr)
                            {
                                const auto& PropertyMetadata = Property.Metadata;
                                const auto& PropertyMetadataElement = PropertyMetadata.Get(
                                    GetMetadataElementName_GatherReferencedObjectPathsFunction()    
                                );
                                const auto& PropertyGatherReferencedObjectPathsFunction = AnyCast<
                                    F_GatherReferencedObjectPathsFunction
                                >(PropertyMetadataElement);
                                void* PropertyTarget = &Property.ReferenceAccess<U8>(InstancePtr);
                                PropertyGatherReferencedObjectPathsFunction(PropertyTarget, Data);
                            };
                            
                            {
                                ReflectionType->ForEachRepresentationOnInstance(
                                    [&](const TW_Valid<F_ReflectionType>& Type, void* InstancePtr, U32 InstanceIndex) -> B8
                                    {
                                        for (const auto& Property : Type->GetProperties())
                                        {
                                            ProcessProperty(Property, InstancePtr);
                                        }
                                        return true;
                                    },
                                    DataPtr
                                );
                            }
                        }
                    )
                );
            }
        ABYTEK_END_REFLECTOR(I_Serializable);
    };
    
    namespace Internal::Serializable
    {
        template<typename __F_Type, typename = void>
        struct TH_GatherReferencedSerializableObjects
        {
            static B8 Invoke(F_Data_GatherReferencedSerializableObjects& Data, __F_Type& Value)
            {
                return true;
            }
        };
        template<typename __F_Type>
        struct TH_GatherReferencedSerializableObjects<__F_Type, std::enable_if_t<IsReflectionBaseOf<I_Serializable, __F_Type>()>>
        {
            static B8 Invoke(F_Data_GatherReferencedSerializableObjects& Data, __F_Type& Value)
            {
                auto ReflectionType = TF_ReflectionTypeHandle<__F_Type>(F_ReflectionContext::GetGlobal());
                const F_ReflectionMetadata& Metadata = ReflectionType->GetMetadata();
                const auto& MetadataElement = Metadata.Get(
                    I_Serializable::GetMetadataElementName_GatherReferencedObjectsFunction()    
                );
                const auto& GatherReferencedObjectsFunction = AnyCast<
                    I_Serializable::F_GatherReferencedObjectsFunction
                >(MetadataElement);
                GatherReferencedObjectsFunction((void*)&Value, Data);
                return true;
            }
        };
        template<typename __F_Object, typename __F_Allocator, typename __F_Config>
        struct TH_GatherReferencedSerializableObjects<ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>>
        {
            static B8 Invoke(F_Data_GatherReferencedSerializableObjects& Data, ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>& Value)
            {
                auto S = GetSThis(Value.GetObjectRawP());
                if (Data.OutObjects->find(S) == Data.OutObjects->end())
                {
                    Data.OutObjects->insert(S);
                }
                return true;
            }
        };
        template<typename __F_Object, typename __F_Allocator, typename __F_Config>
        struct TH_GatherReferencedSerializableObjects<ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>>
        {
            static B8 Invoke(F_Data_GatherReferencedSerializableObjects& Data, ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>& Value)
            {
                if (Data.OutObjects->find(Value) == Data.OutObjects->end())
                {
                    Data.OutObjects->insert(Value);
                }
                return true;
            }
        };
        
        template<typename __F_Type, typename = void>
        struct TH_GatherReferencedSerializableObjectPaths
        {
            static B8 Invoke(F_Data_GatherReferencedSerializableObjectPaths& Data, __F_Type& Value)
            {
                return true;
            }
        };
        template<typename __F_Type>
        struct TH_GatherReferencedSerializableObjectPaths<__F_Type, std::enable_if_t<IsReflectionBaseOf<I_Serializable, std::remove_const_t<__F_Type>>()>>
        {
            static B8 Invoke(F_Data_GatherReferencedSerializableObjectPaths& Data, __F_Type& Value)
            {
                auto ReflectionType = TF_ReflectionTypeHandle<std::remove_const_t<__F_Type>>(F_ReflectionContext::GetGlobal());
                const F_ReflectionMetadata& Metadata = ReflectionType->GetMetadata();
                const auto& MetadataElement = Metadata.Get(
                    I_Serializable::GetMetadataElementName_GatherReferencedObjectPathsFunction()    
                );
                const auto& GatherReferencedObjectPathsFunction = AnyCast<
                    I_Serializable::F_GatherReferencedObjectPathsFunction
                >(MetadataElement);
                GatherReferencedObjectPathsFunction((void*)&Value, Data);
                return true;
            }
        };
        template<typename __F_Object, typename __F_Allocator, typename __F_Config>
        struct TH_GatherReferencedSerializableObjectPaths<ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>>
        {
            static B8 Invoke(F_Data_GatherReferencedSerializableObjectPaths& Data, ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>& Value)
            {
                if (Value)
                {
                    const auto& Path = Value->GetPath();
                    if (Data.OutObjectPaths->find(Path) == Data.OutObjectPaths->end())
                    {
                        Data.OutObjectPaths->insert(Path);
                    }
                }
                return true;
            }
        };
        template<typename __F_Object, typename __F_Allocator, typename __F_Config>
        struct TH_GatherReferencedSerializableObjectPaths<ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>>
        {
            static B8 Invoke(F_Data_GatherReferencedSerializableObjectPaths& Data, ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>& Value)
            {
                if (Value)
                {
                    const auto& Path = Value->GetPath();
                    if (Data.OutObjectPaths->find(Path) == Data.OutObjectPaths->end())
                    {
                        Data.OutObjectPaths->insert(Path);
                    }
                }
                return true;
            }
        };
    }
}

#define ABYTEK_REFLECT_PROPERTY_ENABLE_SERIALIZABLE(MemberName) \
            ABYTEK_REFLECT_PROPERTY_ENABLE_JSON_SERIALIZABLE_AUTO(MemberName) \
            ABYTEK_REFLECT_PROPERTY_ENABLE_BINARY_SERIALIZABLE_AUTO(MemberName)

#define ABYTEK_REFLECT_PROPERTY_DISABLE_SERIALIZABLE(MemberName) \
            ABYTEK_REFLECT_PROPERTY_DISABLE_JSON_SERIALIZABLE_AUTO(MemberName) \
            ABYTEK_REFLECT_PROPERTY_DISABLE_BINARY_SERIALIZABLE_AUTO(MemberName)

#define ABYTEK_REFLECT_PROPERTY_ENABLE_SERIALIZABLE_DEV(MemberName) \
            ABYTEK_REFLECT_PROPERTY_ENABLE_JSON_SERIALIZABLE_DEV(MemberName)

#define ABYTEK_REFLECT_PROPERTY_DISABLE_SERIALIZABLE_DEV(MemberName) \
            ABYTEK_REFLECT_PROPERTY_DISABLE_JSON_SERIALIZABLE_DEV(MemberName)

#define ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(MemberName, ...) \
            ABYTEK_REFLECT_PROPERTY_ADVANCED(MemberName, __VA_ARGS__) \
            ABYTEK_REFLECT_PROPERTY_ENABLE_SERIALIZABLE(MemberName)

#define ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(MemberName) \
            ABYTEK_REFLECT_PROPERTY(MemberName) \
            ABYTEK_REFLECT_PROPERTY_ENABLE_SERIALIZABLE(MemberName)

#define ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED_DEV(MemberName, ...) \
            ABYTEK_REFLECT_PROPERTY_ADVANCED(MemberName, __VA_ARGS__) \
            ABYTEK_REFLECT_PROPERTY_ENABLE_SERIALIZABLE_DEV(MemberName)

#define ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_DEV(MemberName) \
            ABYTEK_REFLECT_PROPERTY(MemberName) \
            ABYTEK_REFLECT_PROPERTY_ENABLE_SERIALIZABLE_DEV(MemberName)