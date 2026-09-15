#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"
#include "Abytek/Reflection.hpp"
#include "Abytek/JSONSerializable.hpp"
#include "Abytek/BinarySerializable.hpp"


namespace Abytek
{
    class A_SerializableObject;
    
    namespace Internal::Serializable
    {
        template<typename __F_Type, typename>
        struct TH_GatherReferencedSerializableObjects;
        template<typename __F_Type, typename>
        struct TH_GatherReferencedSerializableObjectPaths;
    }
    
    template<typename __F_Type>
    void GatherReferencedSerializableObjects(TF_Set<TS<A_SerializableObject>>& OutObjects, const __F_Type& Value)
    {
        DataTraverse<Internal::Serializable::TH_GatherReferencedSerializableObjects>(OutObjects, *(__F_Type*)&Value);
    }
    template<typename __F_Type>
    void GatherReferencedSerializableObjectPaths(TF_Set<F_Name>& OutObjectPaths, const __F_Type& Value)
    {
        DataTraverse<Internal::Serializable::TH_GatherReferencedSerializableObjectPaths>(OutObjectPaths, *(__F_Type*)&Value);
    }
    
    struct ABYTEK_BASE_SERIALIZABLE_API I_Serializable
    {
    public:
        using F_GatherReferencedObjectsFunction = TF_Function<void(
            void* DataPtr,
            TF_Set<TS<A_SerializableObject>>& OutObjects
        )>;
        using F_GatherReferencedObjectPathsFunction = TF_Function<void(
            void* DataPtr,
            TF_Set<F_Name>& OutObjectPaths
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
                            TF_Set<TS<A_SerializableObject>>& OutObjects
                        ) 
                        {
                            GatherReferencedSerializableObjects<__F_Member>(OutObjects, *(__F_Member*)DataPtr);
                        }
                    )
                );
                Metadata.Add(
                    GetMetadataElementName_GatherReferencedObjectPathsFunction(),
                    F_GatherReferencedObjectPathsFunction(
                        [](
                            void* DataPtr,
                            TF_Set<F_Name>& OutObjectPaths
                        ) 
                        {
                            GatherReferencedSerializableObjectPaths<__F_Member>(OutObjectPaths, *(__F_Member*)DataPtr);
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
                            TF_Set<TS<A_SerializableObject>>& OutObjects
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
                                PropertyGatherReferencedObjectsFunction(PropertyTarget, OutObjects);
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
                            TF_Set<F_Name>& OutObjectPaths
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
                                PropertyGatherReferencedObjectPathsFunction(PropertyTarget, OutObjectPaths);
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
            static B8 Invoke(TF_Set<TS<A_SerializableObject>>& OutObjects, __F_Type& Value)
            {
                return true;
            }
        };
        template<typename __F_Type>
        struct TH_GatherReferencedSerializableObjects<__F_Type, std::enable_if_t<IsReflectionBaseOf<I_Serializable, __F_Type>()>>
        {
            static B8 Invoke(TF_Set<TS<A_SerializableObject>>& OutObjects, __F_Type& Value)
            {
                auto ReflectionType = TF_ReflectionTypeHandle<__F_Type>(F_ReflectionContext::GetGlobal());
                const F_ReflectionMetadata& Metadata = ReflectionType->GetMetadata();
                const auto& MetadataElement = Metadata.Get(
                    I_Serializable::GetMetadataElementName_GatherReferencedObjectsFunction()    
                );
                const auto& GatherReferencedObjectsFunction = AnyCast<
                    I_Serializable::F_GatherReferencedObjectsFunction
                >(MetadataElement);
                GatherReferencedObjectsFunction((void*)&Value, OutObjects);
                return true;
            }
        };
        template<typename __F_Object, typename __F_Allocator, typename __F_Config>
        struct TH_GatherReferencedSerializableObjects<ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>>
        {
            static B8 Invoke(TF_Set<TS<A_SerializableObject>>& OutObjects, ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>& Value)
            {
                auto S = GetSThis(Value.GetObjectRawP());
                if (OutObjects.find(S) == OutObjects.end())
                {
                    OutObjects.insert(S);
                }
                return true;
            }
        };
        template<typename __F_Object, typename __F_Allocator, typename __F_Config>
        struct TH_GatherReferencedSerializableObjects<ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>>
        {
            static B8 Invoke(TF_Set<TS<A_SerializableObject>>& OutObjects, ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>& Value)
            {
                if (OutObjects.find(Value) == OutObjects.end())
                {
                    OutObjects.insert(Value);
                }
                return true;
            }
        };
        
        template<typename __F_Type, typename = void>
        struct TH_GatherReferencedSerializableObjectPaths
        {
            static B8 Invoke(TF_Set<F_Name>& OutObjectPaths, __F_Type& Value)
            {
                return true;
            }
        };
        template<typename __F_Type>
        struct TH_GatherReferencedSerializableObjectPaths<__F_Type, std::enable_if_t<IsReflectionBaseOf<I_Serializable, std::remove_const_t<__F_Type>>()>>
        {
            static B8 Invoke(TF_Set<F_Name>& OutObjectPaths, __F_Type& Value)
            {
                auto ReflectionType = TF_ReflectionTypeHandle<std::remove_const_t<__F_Type>>(F_ReflectionContext::GetGlobal());
                const F_ReflectionMetadata& Metadata = ReflectionType->GetMetadata();
                const auto& MetadataElement = Metadata.Get(
                    I_Serializable::GetMetadataElementName_GatherReferencedObjectPathsFunction()    
                );
                const auto& GatherReferencedObjectPathsFunction = AnyCast<
                    I_Serializable::F_GatherReferencedObjectPathsFunction
                >(MetadataElement);
                GatherReferencedObjectPathsFunction((void*)&Value, OutObjectPaths);
                return true;
            }
        };
        template<typename __F_Object, typename __F_Allocator, typename __F_Config>
        struct TH_GatherReferencedSerializableObjectPaths<ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>>
        {
            static B8 Invoke(TF_Set<F_Name>& OutObjectPaths, ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>& Value)
            {
                if (Value)
                {
                    const auto& Path = Value->GetPath();
                    if (OutObjectPaths.find(Path) == OutObjectPaths.end())
                    {
                        OutObjectPaths.insert(Path);
                    }
                }
                return true;
            }
        };
        template<typename __F_Object, typename __F_Allocator, typename __F_Config>
        struct TH_GatherReferencedSerializableObjectPaths<ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>>
        {
            static B8 Invoke(TF_Set<F_Name>& OutObjectPaths, ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>& Value)
            {
                if (Value)
                {
                    const auto& Path = Value->GetPath();
                    if (OutObjectPaths.find(Path) == OutObjectPaths.end())
                    {
                        OutObjectPaths.insert(Path);
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