#include "Abytek/SerializableObject.hpp"
#include "Abytek/SerializableObjectData.hpp"
#include "Abytek/SerializablePackage.hpp"
#include "Abytek/SerializableEnvironment.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_SerializableObject)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_SerializableObject"));    
    }
    
    A_SerializableObject::A_SerializableObject(const F_SerializableObjectInitParams& InitParams) :
        _Type(InitParams.Type),
        _Environment(InitParams.Environment),
        _Package(InitParams.Package),
        _Name(InitParams.Name),
        _PackageName(InitParams.PackageName),
        _SerializableFlags(InitParams.Flags),
        _Path(F_SerializableEnvironment::MakeObjectPath(InitParams.Name, InitParams.PackageName))
    {
        ABYTEK_BASE_SERIALIZABLE_ASSERT(!_PackageName || (_Name && _PackageName)) << "Serializable object name is required when using a valid package name";
        _DefaultObjectReleaser = GetObjectReleaser<F_StandardObjectManagement>(this);
        if (_Package)
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(_Environment == _Package->GetEnvironment()) << "Package mismatch";
            _Package->_RegisterObject(ABYTEK_WTHIS());
        }
        if (_Name)
        {
            _Environment->_RegisterObject(ABYTEK_WTHIS());
#ifdef ABYTEK_DEBUG_INFO
            A_Object::SetDebugName(_Name);
#endif
        }
        if (!HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            _Environment->_ObjectsToLoad.Push(ABYTEK_STHIS());
        }
    }
    A_SerializableObject::~A_SerializableObject()
    {
        if (_Name)
        {
            _Environment->_UnregisterObject(ABYTEK_WTHIS());
        }
        if (_Package)
        {
            _Package->_UnregisterObject(ABYTEK_WTHIS());
        }
    }

    void A_SerializableObject::CleanUp()
    {
    }

    void A_SerializableObject::OnLoad()
    {
    }
    void A_SerializableObject::OnUnload()
    {
    }

    void A_SerializableObject::CallLoad()
    {
        ABYTEK_BASE_SERIALIZABLE_ASSERT(!IsLoaded());
        _IsLoaded = true;

        if (_Package)
        {
            F_SerializableObjectHeader ObjectHeader;
            if (_Package->SearchLastObjectHeader(_Path, ObjectHeader))
            {
                TF_Vector<U8> ObjectPayload;
                _Package->LoadObjectPayload(ObjectHeader, ObjectPayload);
                
                F_Archive Archive = F_Archive::From(ObjectPayload);
                F_SerializableObjectDeserializeParams ObjectDeserializeParams;
                ObjectDeserializeParams.MainView = F_ArchiveReadOnlyView::From(Archive);
                ObjectDeserializeParams.MainView.HasDevelopmentBuild = _Environment->HasDevelopmentBuild();
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    Deserialize(ObjectDeserializeParams)
                );
            }
        }
        
        OnLoad();
    }
    void A_SerializableObject::CallUnload()
    {
        ABYTEK_BASE_SERIALIZABLE_ASSERT(IsLoaded());
        OnUnload();
        _IsLoaded = false;
    }
    
    void A_SerializableObject::OnPrepareForSaving(const TW_Valid<F_SerializableEnvironment>& Environment)
    {
    }
    void A_SerializableObject::OnCleanUpAfterSaving(const TW_Valid<F_SerializableEnvironment>& Environment)
    {
    }

    F_FeedbackStatus A_SerializableObject::BinarySerialize(F_SerializableObjectBinarySerializeParams& Params)
    {
        Params.MainView.IsTopLevel = true;
        const auto& Metadata = _Type->GetMetadata();
        if (!Metadata.HasElement(GetMetadataElementName_BinarySerializeFunction()))
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Corrupt type metadata, not found metadata element")    
            );
        }
        const auto& MetadataElement = Metadata.Get(GetMetadataElementName_BinarySerializeFunction());
        const auto& Function = AnyCast<F_BinarySerializeFunction>(MetadataElement);
        ABYTEK_FEEDBACK_STATUS_CHECK(
            Function(
                ABYTEK_WTHIS(),
                Params.MainView,
                Params.Environment
            )  
        );
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_SerializableObject::BinaryDeserialize(F_SerializableObjectBinaryDeserializeParams& Params)
    {
        Params.MainView.IsTopLevel = true;
        const auto& Metadata = _Type->GetMetadata();
        if (!Metadata.HasElement(GetMetadataElementName_BinaryDeserializeFunction()))
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Corrupt type metadata, not found metadata element")    
            );
        }
        const auto& MetadataElement = Metadata.Get(GetMetadataElementName_BinaryDeserializeFunction());
        const auto& Function = AnyCast<F_BinaryDeserializeFunction>(MetadataElement);
        ABYTEK_FEEDBACK_STATUS_CHECK(
            Function(
                ABYTEK_WTHIS(),
                Params.MainView,
                GetEnvironment()
            )  
        );
        return F_FeedbackStatus::MakeSucceeded();
    }

    F_FeedbackStatus A_SerializableObject::JSONSerialize(boost::json::value& JSON, const TW_Valid<F_SerializableEnvironment>& Environment, F_JSONContext& JSONContext)
    {
        JSONContext.IsTopLevel = true;
        const auto& Metadata = _Type->GetMetadata();
        if (!Metadata.HasElement(GetMetadataElementName_JSONSerializeFunction()))
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Corrupt type metadata, not found metadata element")    
            );
        }
        const auto& MetadataElement = Metadata.Get(GetMetadataElementName_JSONSerializeFunction());
        const auto& Function = AnyCast<F_JSONSerializeFunction>(MetadataElement);
        ABYTEK_FEEDBACK_STATUS_CHECK(
            Function(
                ABYTEK_WTHIS(),
                JSON,
                Environment,
                JSONContext
            )  
        );
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_SerializableObject::JSONDeserialize(const boost::json::value& JSON, const TW_Valid<F_SerializableEnvironment>& Environment, F_JSONContext& JSONContext)
    {
        JSONContext.IsTopLevel = true;
        const auto& Metadata = _Type->GetMetadata();
        if (!Metadata.HasElement(GetMetadataElementName_JSONDeserializeFunction()))
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Corrupt type metadata, not found metadata element")    
            );
        }
        const auto& MetadataElement = Metadata.Get(GetMetadataElementName_JSONDeserializeFunction());
        const auto& Function = AnyCast<F_JSONDeserializeFunction>(MetadataElement);
        ABYTEK_FEEDBACK_STATUS_CHECK(
            Function(
                ABYTEK_WTHIS(),
                JSON,
                Environment,
                JSONContext
            )  
        );
        return F_FeedbackStatus::MakeSucceeded();
    }

    F_FeedbackStatus A_SerializableObject::Serialize(F_SerializableObjectSerializeParams& Params)
    {
        SetGeneralMetadataElement_Environment(Params.MainView.Metadata, Params.Environment);
        
        F_JSONOptions JSONOptions;
        JSONOptions.ReflectionContext = F_ReflectionContext::GetGlobal();
        if (auto CDO = Params.Environment->FindCDO(_Type))
        {
            JSONOptions.CDO = CDO.GetObjectRawP();
            JSONOptions.SkipNonDirtyProperties = true;
        }
        SetGeneralMetadataElement_Environment(JSONOptions.Metadata, Params.Environment);
        F_JSONContext JSONContext = F_JSONContext::Make(JSONOptions);
        
        boost::json::value JSON = boost::json::object();
        ABYTEK_FEEDBACK_STATUS_CHECK(
            JSONSerialize(JSON, Params.Environment, JSONContext)
        );
        F_Text JSONText;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            H_JSON::BoostJSONValueToText(JSON, JSONText)  
        );
        ABYTEK_FEEDBACK_STATUS_CHECK(
            Params.MainView << JSONText
        );
        
        ABYTEK_FEEDBACK_STATUS_CHECK(
            BinarySerialize(Params)  
        );
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_SerializableObject::Deserialize(F_SerializableObjectDeserializeParams& Params)
    {
        SetGeneralMetadataElement_Environment(Params.MainView.Metadata, GetEnvironment());
        
        F_JSONOptions JSONOptions;
        JSONOptions.ReflectionContext = F_ReflectionContext::GetGlobal();
        SetGeneralMetadataElement_Environment(JSONOptions.Metadata, GetEnvironment());
        F_JSONContext JSONContext = F_JSONContext::Make(JSONOptions);
        
        F_Text JSONText;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            Params.MainView >> JSONText
        );
        boost::json::value JSON;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            H_JSON::TextToBoostJSONValue(JSON, JSONText)  
        );
        ABYTEK_FEEDBACK_STATUS_CHECK(
            JSONDeserialize(JSON, GetEnvironment(), JSONContext)
        );
        
        ABYTEK_FEEDBACK_STATUS_CHECK(
            BinaryDeserialize(Params)  
        );
        return F_FeedbackStatus::MakeSucceeded();
    }

    B8 A_SerializableObject::MarkPackageDirty()
    {
        if (!_Package)
        {
            return true;
        }
        return _Package->MarkDirty();
    }

    TS<A_SerializableObject> A_SerializableObject::CreateDefaultSerializableSubobject(
        const F_Name& SubobjectName,
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    ) const
    {
        F_Name ActualSubobjectName;
        if (_Name && SubobjectName)
        {
            ActualSubobjectName = *_Name + ABYTEK_TEXT(".") + *SubobjectName;
        }
        else
        {
            ActualSubobjectName = _Environment->GenerateAnonymousObjectName();
        }
        return _Environment->CreateObjectWithoutLoading(
            ActualSubobjectName,
            _PackageName,
            Type
        );
    }
    TS<A_SerializableObject> A_SerializableObject::CreateSerializableSubobject(
        const F_Name& SubobjectName,
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    ) const
    {
        F_Name ActualSubobjectName;
        if (_Name && SubobjectName)
        {
            ActualSubobjectName = *_Name + ABYTEK_TEXT(".") + *SubobjectName;
        }
        else
        {
            ActualSubobjectName = _Environment->GenerateAnonymousObjectName();
        }
        return _Environment->CreateObject(
            ActualSubobjectName,
            _PackageName,
            Type
        );
    }
}
