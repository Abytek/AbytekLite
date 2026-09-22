#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"
#include "Abytek/Serializable.hpp"
#include "Abytek/SerializableObjectData.hpp"
#include "Abytek/SerializableEnvironment.hpp"
#include "Abytek/JSON.hpp"


namespace Abytek
{
    class A_SerializableObject;
    class F_SerializableEnvironment;
    class F_SerializablePackage;
    
    enum class E_SerializableObjectFlag : U8
    {
        NONE = 0x0,
        CDO = 0x1,
        DEFAULT = NONE
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_SerializableObjectFlag);
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_SerializableObjectFlag)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::E_SerializableObjectFlag"));
        ABYTEK_REFLECT_ENUM_VALUE(NONE);
        ABYTEK_REFLECT_ENUM_VALUE(CDO);
        ABYTEK_REFLECT_ENUM_VALUE(DEFAULT);
    }
    
    struct F_SerializableObjectInitParams
    {
        TF_ReflectionTypeHandle<A_SerializableObject> Type;
        TW<F_SerializableEnvironment> Environment;
        TS<F_SerializablePackage> Package;
        F_Name Name;
        F_Name PackageName;
        E_SerializableObjectFlag Flags = E_SerializableObjectFlag::DEFAULT;
    };
    
    namespace Internal::SerializableObject
    {
        template<typename __F_Object>
        void ObjectReleaser(void* ObjectRawP);
    }
    
    struct F_SerializableObjectBinarySerializeParams
    {
        F_ArchiveReadWriteView MainView;
        F_ArchiveReadWriteView BulkView;
        TW<F_SerializableEnvironment> Environment;
    };
    struct F_SerializableObjectBinaryDeserializeParams
    {
        F_ArchiveReadOnlyView MainView;
    };
    struct F_SerializableObjectSerializeParams : F_SerializableObjectBinarySerializeParams
    {
    };
    struct F_SerializableObjectDeserializeParams : F_SerializableObjectBinaryDeserializeParams
    {
    };
    struct F_SerializableObjectOnCleanUpAfterSavingParams : F_SerializableObjectSerializeParams
    {
    };
    
    class ABYTEK_BASE_SERIALIZABLE_API A_SerializableObject :
        public A_Object, 
        public I_Serializable
    { 
    public:
        friend class F_SerializablePackage;
        friend class F_SerializableEnvironment;
        template<typename __F_Object>
        friend void Internal::SerializableObject::ObjectReleaser(void* ObjectRawP);
        
    public:
        using F_Creator = TF_SharedObjectCreator<
            A_SerializableObject, 
            const F_SerializableObjectInitParams&
        >;
        using F_GenerateHeaderFunction = TF_Function<F_SerializableObjectHeader(
            const TW_Valid<A_SerializableObject>& Object
        )>;
        using F_JSONSerializeFunction = TF_Function<F_FeedbackStatus(
            const TW_Valid<A_SerializableObject>& Object, 
            boost::json::value& JSON, 
            const TW_Valid<F_SerializableEnvironment>& Environment,
            F_JSONContext& JSONContext
        )>;
        using F_JSONDeserializeFunction = TF_Function<F_FeedbackStatus(
            const TW_Valid<A_SerializableObject>& Object, 
            const boost::json::value& JSON, 
            const TW_Valid<F_SerializableEnvironment>& Environment,
            F_JSONContext& JSONContext
        )>;
        using F_BinarySerializeFunction = TF_Function<F_FeedbackStatus(
            const TW_Valid<A_SerializableObject>& Object, 
            F_ArchiveReadWriteView& View, 
            const TW_Valid<F_SerializableEnvironment>& Environment
        )>;
        using F_BinaryDeserializeFunction = TF_Function<F_FeedbackStatus(
            const TW_Valid<A_SerializableObject>& Object, 
            F_ArchiveReadOnlyView& View, 
            const TW_Valid<F_SerializableEnvironment>& Environment
        )>;
        
    public:
        static F_Name GetMetadataElementName_Creator()
        {
            return ABYTEK_NAME("Abytek::A_SerializableObject::Creator");
        }
        static F_Name GetMetadataElementName_GenerateHeaderFunction()
        {
            return ABYTEK_NAME("Abytek::A_SerializableObject::GenerateHeaderFunction");
        }
        static F_Name GetMetadataElementName_JSONSerializeFunction()
        {
            return ABYTEK_NAME("Abytek::A_SerializableObject::JSONSerializeFunction");
        }
        static F_Name GetMetadataElementName_JSONDeserializeFunction()
        {
            return ABYTEK_NAME("Abytek::A_SerializableObject::JSONDeserializeFunction");
        }
        static F_Name GetMetadataElementName_BinarySerializeFunction()
        {
            return ABYTEK_NAME("Abytek::A_SerializableObject::BinarySerializeFunction");
        }
        static F_Name GetMetadataElementName_BinaryDeserializeFunction()
        {
            return ABYTEK_NAME("Abytek::A_SerializableObject::BinaryDeserializeFunction");
        }
        
    public:
        static F_Name GetGeneralMetadataElementName_Environment()
        {
            return ABYTEK_NAME("Abytek::A_SerializableObject::Environment");
        }
        
    public:
        static void SetGeneralMetadataElement_Environment(TF_Map<F_Name, F_Any>& Metadata, const TW_Valid<F_SerializableEnvironment>& Value)
        {
            Metadata[GetGeneralMetadataElementName_Environment()] = TW<F_SerializableEnvironment>(Value);
        }
        static F_FeedbackStatus GetGeneralMetadataElement_Environment(const TF_Map<F_Name, F_Any>& Metadata, TW<F_SerializableEnvironment>& OutValue)
        {
            auto It = Metadata.find(GetGeneralMetadataElementName_Environment());
            if (It == Metadata.end())
            {
                return F_FeedbackStatus::MakeFailed(
                    ABYTEK_TEXT("Not found JSON option metadata element: ") 
                    + *GetGeneralMetadataElementName_Environment()
                );
            }
            if (auto Environment = AnyCast<TW<F_SerializableEnvironment>>(It->second))
            {
                OutValue = Environment;
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Invalid environment") 
            );
        }
        
    public:
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
            ABYTEK_REFLECT_GEN_INTERFACE()
            {
                auto& Metadata = ReflectionType->GetMetadata();
                if (F_Creator::Check<__F_SelfOrInherited>())
                {
                    Metadata.Add(
                        GetMetadataElementName_Creator(),
                        F_Creator::Make<__F_SelfOrInherited>(
                            [](const TS<A_SerializableObject>& Object)
                            {
                                Object.SetObjectReleaser(
                                    &Internal::SerializableObject::ObjectReleaser<__F_SelfOrInherited>
                                );
                            }
                        )
                    );
                }
                else
                {
                    ReflectionType->AdditionalInfo.IsAbstract = true;   
                }
                Metadata.Add(
                    GetMetadataElementName_GenerateHeaderFunction(),
                    F_GenerateHeaderFunction(
                        [](const TW_Valid<A_SerializableObject>& Object) 
                        -> F_SerializableObjectHeader
                        {
                            F_SerializableObjectHeader Result;
                            Result.Type = Object->GetType();
                            Result.Path = Object->GetPath();
                            TF_Set<F_Name> ReferencePaths;
                            GatherReferencedSerializableObjectPaths<__F_SelfOrInherited>(
                                ReferencePaths,
                                *Object.FastCast<__F_SelfOrInherited>()
                            );
                            for (const auto& Path : ReferencePaths)
                            {
                                Result.ReferencePaths.push_back(Path);
                            }
                            return ABYTEK_MOVE(Result);
                        }
                    )
                );
                Metadata.Add(
                    GetMetadataElementName_JSONSerializeFunction(),
                    F_JSONSerializeFunction(
                        [](
                            const TW_Valid<A_SerializableObject>& Object, 
                            boost::json::value& JSON, 
                            const TW_Valid<F_SerializableEnvironment>& Environment,
                            F_JSONContext& JSONContext
                        ) -> F_FeedbackStatus
                        {
                            TW<__F_SelfOrInherited> CastedObject;
                            if (!Object.TryDynamicCast<__F_SelfOrInherited>(CastedObject))
                            {
                                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid object type"));
                            }
                            ABYTEK_FEEDBACK_STATUS_CHECK(
                                H_JSON::ValueToBoostJSONValue(
                                    *CastedObject,
                                    JSON,
                                    JSONContext
                                )
                            );
                            return F_FeedbackStatus::MakeSucceeded();
                        }
                    )
                );
                Metadata.Add(
                    GetMetadataElementName_JSONDeserializeFunction(),
                    F_JSONDeserializeFunction(
                        [](
                            const TW_Valid<A_SerializableObject>& Object, 
                            const boost::json::value& JSON, 
                            const TW_Valid<F_SerializableEnvironment>& Environment,
                            F_JSONContext& JSONContext
                        ) -> F_FeedbackStatus
                        {
                            TW<__F_SelfOrInherited> CastedObject;
                            if (!Object.TryDynamicCast<__F_SelfOrInherited>(CastedObject))
                            {
                                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid object type"));
                            }
                            ABYTEK_FEEDBACK_STATUS_CHECK(
                                H_JSON::BoostJSONValueToValue(
                                    *CastedObject,
                                    JSON,
                                    JSONContext
                                )
                            );
                            return F_FeedbackStatus::MakeSucceeded();
                        }
                    )
                );
                Metadata.Add(
                    GetMetadataElementName_BinarySerializeFunction(),
                    F_BinarySerializeFunction(
                        [](
                            const TW_Valid<A_SerializableObject>& Object, 
                            F_ArchiveReadWriteView& View, 
                            const TW_Valid<F_SerializableEnvironment>& Environment
                        ) -> F_FeedbackStatus
                        {
                            TW<__F_SelfOrInherited> CastedObject;
                            if (!Object.TryDynamicCast<__F_SelfOrInherited>(CastedObject))
                            {
                                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid object type"));
                            }
                            ABYTEK_FEEDBACK_STATUS_CHECK(
                                View << *CastedObject
                            );
                            return F_FeedbackStatus::MakeSucceeded();
                        }
                    )
                );
                Metadata.Add(
                    GetMetadataElementName_BinaryDeserializeFunction(),
                    F_BinaryDeserializeFunction(
                        [](
                            const TW_Valid<A_SerializableObject>& Object, 
                            F_ArchiveReadOnlyView& View, 
                            const TW_Valid<F_SerializableEnvironment>& Environment
                        ) -> F_FeedbackStatus
                        {
                            TW<__F_SelfOrInherited> CastedObject;
                            if (!Object.TryDynamicCast<__F_SelfOrInherited>(CastedObject))
                            {
                                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid object type"));
                            }
                            ABYTEK_FEEDBACK_STATUS_CHECK(
                                View >> *CastedObject
                            );
                            return F_FeedbackStatus::MakeSucceeded();
                        }
                    )
                );
            }
        ABYTEK_END_REFLECTOR(A_SerializableObject);
        
    private:
        TF_ReflectionTypeHandle<A_SerializableObject> _Type;
        TW<F_SerializableEnvironment> _Environment;
        TS<F_SerializablePackage> _Package;
        F_Name _Name;
        F_Name _PackageName;
        E_SerializableObjectFlag _SerializableFlags = E_SerializableObjectFlag::NONE;
        F_Name _Path;
        B8 _IsLoaded = false;
        B8 _IsLoadedFromPackage = false;
        F_ObjectReleaser _DefaultObjectReleaser;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetType() const noexcept
        {
            return _Type;
        }
        ABYTEK_FORCE_INLINE const auto& GetEnvironment() const noexcept
        {
            return _Environment;
        }
        ABYTEK_FORCE_INLINE const auto& GetPackage() const noexcept
        {
            return _Package;
        }
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            return _Name;
        }
        ABYTEK_FORCE_INLINE const auto& GetPath() const noexcept
        {
            return _Path;
        }
        ABYTEK_FORCE_INLINE const auto& GetPackageName() const noexcept
        {
            return _PackageName;
        }
        ABYTEK_FORCE_INLINE auto GetSerializableFlags() const noexcept
        {
            return _SerializableFlags;
        }
        ABYTEK_FORCE_INLINE B8 HasSerializableFlags(E_SerializableObjectFlag X) const noexcept
        {
            return FlagHas(_SerializableFlags, X);
        }
        ABYTEK_FORCE_INLINE B8 HasSerializableFlagsAny(E_SerializableObjectFlag X) const noexcept
        {
            return FlagHasAny(_SerializableFlags, X);
        }
        ABYTEK_FORCE_INLINE const auto& IsLoaded() const noexcept
        {
            return _IsLoaded;
        }
        ABYTEK_FORCE_INLINE const auto& IsLoadedFromPackage() const noexcept
        {
            return _IsLoadedFromPackage;
        }
        ABYTEK_FORCE_INLINE auto GetDefaultObjectReleaser() const noexcept
        {
            return _DefaultObjectReleaser;
        }
        
    protected:
        A_SerializableObject(const F_SerializableObjectInitParams& InitParams);
        
    public:
        ~A_SerializableObject() override;
        
    protected:
        virtual void CleanUp();
        
    protected:
        virtual void OnLoad();
        virtual void OnUnload();
        
    public:
        void CallLoad();
        void CallUnload();
        
    public:
        virtual B8 CanSerialize() const
        {
            return static_cast<B8>(_Name) && static_cast<B8>(_PackageName);
        }
        
    protected:
        virtual void OnPrepareForSaving(const TW_Valid<F_SerializableEnvironment>& Environment);
        virtual void OnCleanUpAfterSaving(const TW_Valid<F_SerializableEnvironment>& Environment);
        
    protected:
        virtual F_FeedbackStatus BinarySerialize(F_SerializableObjectBinarySerializeParams& Params);
        virtual F_FeedbackStatus BinaryDeserialize(F_SerializableObjectBinaryDeserializeParams& Params);
        
    protected:
        virtual F_FeedbackStatus JSONSerialize(boost::json::value& JSON, const TW_Valid<F_SerializableEnvironment>& Environment, F_JSONContext& JSONContext);
        virtual F_FeedbackStatus JSONDeserialize(const boost::json::value& JSON, const TW_Valid<F_SerializableEnvironment>& Environment, F_JSONContext& JSONContext);
        
    public:
        F_FeedbackStatus Serialize(F_SerializableObjectSerializeParams& Params);
        F_FeedbackStatus Deserialize(F_SerializableObjectDeserializeParams& Params);
        
    public:
        B8 MarkPackageDirty();
        
    public:
        TS<A_SerializableObject> CreateDefaultSerializableSubobject(
            const F_Name& SubobjectName, 
            const TF_ReflectionTypeHandle<A_SerializableObject>& Type
        ) const;
        template<typename __F_Object>
        TS<__F_Object> CreateDefaultSerializableSubobject(
            const F_Name& SubobjectName, 
            const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())
        ) const
        {
            return CreateDefaultSerializableSubobject(
                SubobjectName,
                Type.template Cast<A_SerializableObject>()
            ).FastCast<__F_Object>();
        }
        
    public:
        TS<A_SerializableObject> CreateSerializableSubobject(
            const F_Name& SubobjectName, 
            const TF_ReflectionTypeHandle<A_SerializableObject>& Type
        ) const;
        template<typename __F_Object>
        TS<__F_Object> CreateSerializableSubobject(
            const F_Name& SubobjectName, 
            const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())
        ) const
        {
            return CreateSerializableSubobject(
                SubobjectName,
                Type.template Cast<A_SerializableObject>()
            ).FastCast<__F_Object>();
        }
    };
    
    namespace Internal::SerializableObject
    {
        template<typename __F_Object>
        void ObjectReleaser(void* ObjectRawP)
        {
            __F_Object* CastedObjectRawP = (__F_Object*)ObjectRawP;
            ABYTEK_BASE_SERIALIZABLE_ASSERT(
                dynamic_cast<A_SerializableObject*>(CastedObjectRawP) == ((A_SerializableObject*)CastedObjectRawP)
            );
            if (!CastedObjectRawP->HasSerializableFlags(E_SerializableObjectFlag::CDO))
            {
                if (CastedObjectRawP->IsLoaded())
                {
                    CastedObjectRawP->CallUnload();
                }
                ((A_SerializableObject*)CastedObjectRawP)->CleanUp();
            }
            CastedObjectRawP->GetDefaultObjectReleaser()(ObjectRawP);
        }
    }
    
    template<typename __F_Object, typename __F_Config>
    struct TH_JSONConvert<
        ObjectSmartPointerTemplates::TW<__F_Object, __F_Config>,
        std::enable_if_t<std::is_base_of_v<A_SerializableObject, __F_Object>>
    >
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const ObjectSmartPointerTemplates::TW<__F_Object, __F_Config>& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            F_Name Name;
            if (Value)
            {
                if (Value->CanSerialize())
                {
                    Name = Value->GetName();
                }
            }
            ABYTEK_FEEDBACK_STATUS_CHECK(
                H_JSON::ValueToBoostJSONValue(Name, JSONValue, JSONContext)    
            );
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(ObjectSmartPointerTemplates::TW<__F_Object, __F_Config>& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            F_Name Name;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                H_JSON::BoostJSONValueToValue(Name, JSONValue, JSONContext)    
            );
            if (!Name)
            {
                Value = {};
                return F_FeedbackStatus::MakeSucceeded();
            }
            
            TW<F_SerializableEnvironment> Environment;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                A_SerializableObject::GetGeneralMetadataElement_Environment(
                    JSONContext.Options->Metadata,
                    Environment    
                )    
            );
            
            auto Object = Environment->FindObject(Name);
            if (!Object)
            {
                return F_FeedbackStatus::MakeFailed(
                      ABYTEK_TEXT("Not found object: ")
                      + *Name
                );
            }
            
            Value = Object.FastCast<__F_Object>();
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    template<typename __F_Object, typename __F_Allocator, typename __F_Config>
    struct TH_JSONConvert<
        ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>,
        std::enable_if_t<std::is_base_of_v<A_SerializableObject, __F_Object>>
    >
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            F_Name Name;
            if (Value)
            {
                if (Value->CanSerialize())
                {
                    Name = Value->GetName();
                }
            }
            ABYTEK_FEEDBACK_STATUS_CHECK(
                H_JSON::ValueToBoostJSONValue(Name, JSONValue, JSONContext)    
            );
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            F_Name Name;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                H_JSON::BoostJSONValueToValue(Name, JSONValue, JSONContext)    
            );
            if (!Name)
            {
                Value = {};
                return F_FeedbackStatus::MakeSucceeded();
            }
            
            TW<F_SerializableEnvironment> Environment;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                A_SerializableObject::GetGeneralMetadataElement_Environment(
                    JSONContext.Options->Metadata,
                    Environment    
                )    
            );
            
            auto Object = Environment->FindObject(Name);
            if (!Object)
            {
                return F_FeedbackStatus::MakeFailed(
                      ABYTEK_TEXT("Not found object: ")
                      + *Name
                );
            }
            if (!Object->IsLoaded())
            {
                Object->CallLoad();
            }
            
            Value = GetUThis(Object.GetObjectRawP()).FastCast<__F_Object>();
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    template<typename __F_Object, typename __F_Allocator, typename __F_Config>
    struct TH_JSONConvert<
        ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>,
        std::enable_if_t<std::is_base_of_v<A_SerializableObject, __F_Object>>
    >
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            F_Name Name;
            if (Value)
            {
                if (Value->CanSerialize())
                {
                    Name = Value->GetName();
                }
            }
            ABYTEK_FEEDBACK_STATUS_CHECK(
                H_JSON::ValueToBoostJSONValue(Name, JSONValue, JSONContext)    
            );
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            F_Name Name;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                H_JSON::BoostJSONValueToValue(Name, JSONValue, JSONContext)    
            );
            if (!Name)
            {
                Value = {};
                return F_FeedbackStatus::MakeSucceeded();
            }
            
            TW<F_SerializableEnvironment> Environment;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                A_SerializableObject::GetGeneralMetadataElement_Environment(
                    JSONContext.Options->Metadata,
                    Environment    
                )    
            );
            
            auto Object = Environment->FindObject(Name);
            if (!Object)
            {
                return F_FeedbackStatus::MakeFailed(
                      ABYTEK_TEXT("Not found object: ")
                      + *Name
                );
            }
            if (!Object->IsLoaded())
            {
                Object->CallLoad();
            }
            
            Value = ShareObject(Object).FastCast<__F_Object>();
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    template<typename __F_Object, typename __F_Config ABYTEK_REQUIRES(std::is_base_of_v<A_SerializableObject, __F_Object>)>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const ObjectSmartPointerTemplates::TW<__F_Object, __F_Config>& Value) noexcept
    {
        F_Name Name;
        if (Value)
        {
            if (Value->CanSerialize())
            {
                Name = Value->GetName();
            }
        }
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Name);
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Object, typename __F_Config ABYTEK_REQUIRES(std::is_base_of_v<A_SerializableObject, __F_Object>)>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, ObjectSmartPointerTemplates::TW<__F_Object, __F_Config>& Value) noexcept
    {
        F_Name Name;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            View >> Name
        );
        if (!Name)
        {
            Value = {};
            return F_FeedbackStatus::MakeSucceeded();
        }
            
        TW<F_SerializableEnvironment> Environment;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_SerializableObject::GetGeneralMetadataElement_Environment(
                View.Metadata,
                Environment    
            )    
        );
            
        auto Object = Environment->FindObject(Name);
        if (!Object)
        {
            return F_FeedbackStatus::MakeFailed(
                  ABYTEK_TEXT("Not found object: ")
                  + *Name
            );
        }
            
        Value = Object.FastCast<__F_Object>();
        return F_FeedbackStatus::MakeSucceeded();
    }
    
    template<typename __F_Object, typename __F_Allocator, typename __F_Config ABYTEK_REQUIRES(std::is_base_of_v<A_SerializableObject, __F_Object>)>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>& Value) noexcept
    {
        F_Name Name;
        if (Value)
        {
            if (Value->CanSerialize())
            {
                Name = Value->GetName();
            }
        }
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Name);
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Object, typename __F_Allocator, typename __F_Config ABYTEK_REQUIRES(std::is_base_of_v<A_SerializableObject, __F_Object>)>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, ObjectSmartPointerTemplates::TU<__F_Object, __F_Allocator, __F_Config>& Value) noexcept
    {
        F_Name Name;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            View >> Name
        );
        if (!Name)
        {
            Value = {};
            return F_FeedbackStatus::MakeSucceeded();
        }
            
        TW<F_SerializableEnvironment> Environment;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_SerializableObject::GetGeneralMetadataElement_Environment(
                View.Metadata,
                Environment    
            )    
        );
            
        auto Object = Environment->FindObject(Name);
        if (!Object)
        {
            return F_FeedbackStatus::MakeFailed(
                  ABYTEK_TEXT("Not found object: ")
                  + *Name
            );
        }
            
        Value = GetUThis(Object.GetObjectRawP()).FastCast<__F_Object>();
        return F_FeedbackStatus::MakeSucceeded();
    }
    
    template<typename __F_Object, typename __F_Allocator, typename __F_Config ABYTEK_REQUIRES(std::is_base_of_v<A_SerializableObject, __F_Object>)>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>& Value) noexcept
    {
        F_Name Name;
        if (Value)
        {
            if (Value->CanSerialize())
            {
                Name = Value->GetName();
            }
        }
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Name);
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Object, typename __F_Allocator, typename __F_Config ABYTEK_REQUIRES(std::is_base_of_v<A_SerializableObject, __F_Object>)>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, ObjectSmartPointerTemplates::TS<__F_Object, __F_Allocator, __F_Config>& Value) noexcept
    {
        F_Name Name;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            View >> Name
        );
        if (!Name)
        {
            Value = {};
            return F_FeedbackStatus::MakeSucceeded();
        }
            
        TW<F_SerializableEnvironment> Environment;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_SerializableObject::GetGeneralMetadataElement_Environment(
                View.Metadata,
                Environment    
            )    
        );
            
        auto Object = Environment->FindObject(Name);
        if (!Object)
        {
            return F_FeedbackStatus::MakeFailed(
                  ABYTEK_TEXT("Not found object: ")
                  + *Name
            );
        }
            
        Value = ShareObject(Object).FastCast<__F_Object>();
        return F_FeedbackStatus::MakeSucceeded();
    }
}