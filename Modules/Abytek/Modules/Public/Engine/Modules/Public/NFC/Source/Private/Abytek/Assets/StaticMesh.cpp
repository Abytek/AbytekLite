#include "Abytek/Assets/StaticMesh.hpp"
#include "Abytek/Assets/Render/StaticMeshRenderProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#ifdef ABYTEK_ENGINE_NFC_ENABLE_ASSIMP
#include "Abytek/Assimp.hpp"
#endif


namespace Abytek
{
    ABYTEK_REFLECT(F_StaticMeshSetting)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_StaticMeshSetting"));
    }
    
    ABYTEK_REFLECT(F_StaticMesh)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_StaticMesh"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_Setting);
    }

    F_StaticMesh::F_StaticMesh(const F_SerializableObjectInitParams& InitParam) :
        A_WorldContext(InitParam)
    {
    }
    F_StaticMesh::~F_StaticMesh()
    {
    }

    void F_StaticMesh::OnLoad()
    {
        SetupRenderable();
    }
    void F_StaticMesh::OnUnload()
    {
        CleanUpRenderable();
    }

    F_FeedbackStatus F_StaticMesh::BinarySerialize(F_SerializableObjectBinarySerializeParams& Params)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_SerializableObject::BinarySerialize(Params)
        );
        
        ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView << _DataType);
        switch (_DataType)
        {
        case E_StaticMeshDataType::SIMPLE:
            {
                F_SimpleMeshData Data;
                if (LoadSimpleData(Data))
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView << true);
            
                    F_StaticMeshDataBulkHeader DataBulkHeader;
                    Params.BulkView.Shift<F_ArchiveData>(0);
                    DataBulkHeader.PayloadOffsetInBytes = Params.BulkView.Offset;
                    ABYTEK_FEEDBACK_STATUS_CHECK(
                        Params.BulkView << Data  
                    );
                    DataBulkHeader.PayloadSizeInBytes = Params.BulkView.Offset - DataBulkHeader.PayloadOffsetInBytes;
            
                    ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView << DataBulkHeader);
                    _TempSerializationData_LastSimpleDataBulkHeader = DataBulkHeader;
                }
                else
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView << false);
                }
            }
            break;
        case E_StaticMeshDataType::ECMS:
            {
                F_ECMSMeshData Data;
                if (LoadECMSData(Data))
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView << true);
            
                    F_StaticMeshDataBulkHeader DataBulkHeader;
                    Params.BulkView.Shift<F_ArchiveData>(0);
                    DataBulkHeader.PayloadOffsetInBytes = Params.BulkView.Offset;
                    ABYTEK_FEEDBACK_STATUS_CHECK(
                        Params.BulkView << Data  
                    );
                    DataBulkHeader.PayloadSizeInBytes = Params.BulkView.Offset - DataBulkHeader.PayloadOffsetInBytes;
            
                    ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView << DataBulkHeader);
                    _TempSerializationData_LastECMSDataBulkHeader = DataBulkHeader;
                }
                else
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView << false);
                }
            }
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown mesh data type: " << static_cast<U32>(_DataType);
        };
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_StaticMesh::BinaryDeserialize(F_SerializableObjectBinaryDeserializeParams& Params)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_SerializableObject::BinaryDeserialize(Params)
        );
        
        ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView >> _DataType);
        switch (_DataType)
        {
        case E_StaticMeshDataType::SIMPLE:
            {
                B8 HasLastSimpleDataBulk = false;
                ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView >> HasLastSimpleDataBulk);
                if (HasLastSimpleDataBulk)
                {
                    F_StaticMeshDataBulkHeader DataBulkHeader;
                    ABYTEK_FEEDBACK_STATUS_CHECK(
                        Params.MainView >> DataBulkHeader
                    );
                    _LastSimpleDataBulkHeader = DataBulkHeader;
                }
                else
                {
                    _LastSimpleDataBulkHeader = {};
                }
            }
            break;
        case E_StaticMeshDataType::ECMS:
            {
                B8 HasLastECMSDataBulk = false;
                ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView >> HasLastECMSDataBulk);
                if (HasLastECMSDataBulk)
                {
                    F_StaticMeshDataBulkHeader DataBulkHeader;
                    ABYTEK_FEEDBACK_STATUS_CHECK(
                        Params.MainView >> DataBulkHeader
                    );
                    _LastECMSDataBulkHeader = DataBulkHeader;
                }
                else
                {
                    _LastECMSDataBulkHeader = {};
                }
            }
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown mesh data type: " << static_cast<U32>(_DataType);
        };
        return F_FeedbackStatus::MakeSucceeded();
    }

    void F_StaticMesh::OnCleanUpAfterSaving(const TW_Valid<F_SerializableEnvironment>& Environment)
    {
        if (Environment == GetEnvironment())
        {
            _LastSimpleDataBulkHeader = _TempSerializationData_LastSimpleDataBulkHeader;
            _NewSimpleData = {};
            _LastECMSDataBulkHeader = _TempSerializationData_LastECMSDataBulkHeader;
            _NewECMSData = {};
        }
        _TempSerializationData_LastSimpleDataBulkHeader = {};
        _TempSerializationData_LastECMSDataBulkHeader = {};
    }

    B8 F_StaticMesh::IsRenderable() const
    {
        if (!A_Renderable::IsRenderable())
        {
            return false;
        }
        return (
            (static_cast<B8>(_LastSimpleDataBulkHeader) && GetPackageName()) 
            || static_cast<B8>(_NewSimpleData)
            || (static_cast<B8>(_LastECMSDataBulkHeader) && GetPackageName()) 
            || static_cast<B8>(_NewECMSData)
        );
    }

    TS<A_RenderProxy> F_StaticMesh::CreateRenderProxy()
    {
        return TS<F_StaticMeshRenderProxy>()(ABYTEK_WTHIS());
    }
    void F_StaticMesh::OnCreateRenderState()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                RenderProxy = GetRenderProxy().FastCast<F_StaticMeshRenderProxy>(), 
                CachedDataType = _DataType
            ]() mutable
            {
                RenderProxy->_DataType = CachedDataType;
            }
        );
        switch (_DataType)
        {
        case E_StaticMeshDataType::SIMPLE:
            {
                F_SimpleMeshData SimpleData;
                B8 Status = LoadSimpleData(SimpleData);
                ABYTEK_ENGINE_NFC_ASSERT(Status) << "Failed to load simple data while being renderable";
                H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                    [
                        RenderProxy = GetRenderProxy().FastCast<F_StaticMeshRenderProxy>(), 
                        CachedSimpleData = ABYTEK_MOVE(SimpleData),
                        CachedSetting = ABYTEK_MOVE(_Setting)
                    ]() mutable
                    {
                        RenderProxy->_TempSimpleData = TS_Unmanaged<F_SimpleMeshData>()(
                            ABYTEK_MOVE(CachedSimpleData)    
                        );
                        RenderProxy->_Setting = ABYTEK_MOVE(CachedSetting);
                    }
                );
            }
            break;
        case E_StaticMeshDataType::ECMS:
            {
                F_ECMSMeshData ECMSData;
                B8 Status = LoadECMSData(ECMSData);
                ABYTEK_ENGINE_NFC_ASSERT(Status) << "Failed to load ECMS data while being renderable";
                H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                    [
                        RenderProxy = GetRenderProxy().FastCast<F_StaticMeshRenderProxy>(), 
                        CachedECMSData = ABYTEK_MOVE(ECMSData),
                        CachedSetting = ABYTEK_MOVE(_Setting)
                    ]() mutable
                    {
                        RenderProxy->_TempECMSData = TS_Unmanaged<F_ECMSMeshData>()(
                            ABYTEK_MOVE(CachedECMSData)    
                        );
                        RenderProxy->_Setting = ABYTEK_MOVE(CachedSetting);
                    }
                );
            }
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown mesh data type: " << static_cast<U32>(_DataType);
        };
    }
    void F_StaticMesh::OnDestroyRenderState()
    {
    }

    namespace Internal::StaticMesh::Simple
    {
        F_FeedbackStatus Decode(const TF_Span<const U8>& Bytes, const F_StaticMeshSourceDecodeConfig& Config, TF_Vector<F_SimpleMeshData>& OutSimpleDataList)
        {
#ifdef ABYTEK_ENGINE_NFC_ENABLE_ASSIMP
            {
                TF_Vector<F_AssimpSimpleMeshData> AssimpSimpleMeshDataList;
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    H_Assimp::Decode(Bytes, AssimpSimpleMeshDataList)  
                );
                TF_Vector<F_SimpleMeshData> SimpleDataList;
                for (const auto& AssimpSimpleMeshData : AssimpSimpleMeshDataList)
                {
                    F_SimpleMeshData SimpleMeshData;
                    SimpleMeshData.Indices = AssimpSimpleMeshData.Indices;
                    SimpleMeshData.Positions = AssimpSimpleMeshData.Positions;
                    SimpleMeshData.Normals = AssimpSimpleMeshData.Normals;
                    SimpleMeshData.TangentsAndSigns = AssimpSimpleMeshData.TangentsAndSigns;
                    SimpleMeshData.UVs = AssimpSimpleMeshData.UVs;
                    SimpleDataList.push_back(ABYTEK_MOVE(SimpleMeshData));
                }
                OutSimpleDataList = ABYTEK_MOVE(SimpleDataList);
                return F_FeedbackStatus::MakeSucceeded();
            }
#endif
            return F_FeedbackStatus::MakeSucceeded();
        }
    }

    F_FeedbackStatus F_StaticMesh::SourceDecode(const TF_Span<const U8>& Bytes, const F_StaticMeshSourceDecodeConfig& Config, TF_Vector<F_SimpleMeshData>& OutSimpleDataList)
    {
        return Internal::StaticMesh::Simple::Decode(Bytes, Config, OutSimpleDataList);
    }
    
    void F_StaticMesh::Import(const TF_Span<const U8>& Bytes, const F_StaticMeshSourceImportConfig& Config, const TF_Optional<F_StaticMeshSetting>& Setting)
    {
        switch (Config.DataType)
        {
        case E_StaticMeshDataType::SIMPLE:
            {
                TF_Vector<F_SimpleMeshData> SimpleDataList;
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    SourceDecode(Bytes, Config.Decode, SimpleDataList)  
                );
                Import(SimpleDataList[0], Setting);
            }
            break;
        case E_StaticMeshDataType::ECMS:
            {
                TF_Vector<F_SimpleMeshData> SimpleDataList;
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    SourceDecode(Bytes, Config.Decode, SimpleDataList)  
                );
                ABYTEK_LOG_INFO() << "Building ECMS static mesh data: " << GetPath();
                F_ECMSMeshData ECMSData = F_ECMSMeshData::From(SimpleDataList[0]);
                ABYTEK_LOG_INFO() << "Built ECMS static mesh data: " << GetPath();
                Import(ECMSData, Setting);
            }
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown mesh data type: " << static_cast<U32>(Config.DataType);
        }
    }
    void F_StaticMesh::Import(const F_Text& FilePath, const F_StaticMeshSourceImportConfig& Config, const TF_Optional<F_StaticMeshSetting>& Setting)
    {
        F_Text AbsoluteFilePath;
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            GetEnvironment()->ResolveAbsolutePath(FilePath, AbsoluteFilePath)
        );
        
        TF_Vector<U8> Bytes;
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            H_FSUtilities::ReadFileBinary(AbsoluteFilePath, Bytes)
        );
        Import(
            Bytes,
            Config,
            Setting
        );
    }
    void F_StaticMesh::Import(const F_SimpleMeshData& SimpleData, const TF_Optional<F_StaticMeshSetting>& Setting)
    {
        _DataType = E_StaticMeshDataType::SIMPLE;
        _NewSimpleData = SimpleData;
        if (Setting)
        {
            _Setting = *Setting;
        }
        MarkPackageDirty();
        RecreateRenderState();
    }
    void F_StaticMesh::Import(const F_ECMSMeshData& ECMSData, const TF_Optional<F_StaticMeshSetting>& Setting)
    {
        _DataType = E_StaticMeshDataType::ECMS;
        _NewECMSData = ECMSData;
        if (Setting)
        {
            _Setting = *Setting;
        }
        MarkPackageDirty();
        RecreateRenderState();
    }

    void F_StaticMesh::UpdateSetting(const F_StaticMeshSetting& Setting)
    {
        _Setting = Setting;
        MarkPackageDirty();
        RecreateRenderState();
    }

    B8 F_StaticMesh::LoadSimpleData(F_SimpleMeshData& OutData)
    {
        ABYTEK_ENGINE_NFC_ASSERT(_DataType == E_StaticMeshDataType::SIMPLE);
        if (_NewSimpleData)
        {
            OutData = *_NewSimpleData;
            return true;
        }
        if (!_LastSimpleDataBulkHeader)
        {
            return false;
        }
        auto Package = GetPackage();
        if (!Package)
        {
            return false;
        }
        
        TF_Vector<U8> Bytes;
        Package->LoadBulkPayload(
            _LastSimpleDataBulkHeader->PayloadOffsetInBytes,    
            _LastSimpleDataBulkHeader->PayloadSizeInBytes,
            Bytes
        );
        
        F_Archive Archive = F_Archive::From(Bytes);
        F_ArchiveReadOnlyView ArchiveView = F_ArchiveReadOnlyView::From(Archive);
        ArchiveView.HasDevelopmentBuild = GetEnvironment()->HasDevelopmentBuild();
        
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            ArchiveView >> OutData  
        );
        return true;
    }
    B8 F_StaticMesh::LoadECMSData(F_ECMSMeshData& OutData)
    {
        ABYTEK_ENGINE_NFC_ASSERT(_DataType == E_StaticMeshDataType::ECMS);
        if (_NewECMSData)
        {
            OutData = *_NewECMSData;
            return true;
        }
        if (!_LastECMSDataBulkHeader)
        {
            return false;
        }
        auto Package = GetPackage();
        if (!Package)
        {
            return false;
        }
        
        TF_Vector<U8> Bytes;
        Package->LoadBulkPayload(
            _LastECMSDataBulkHeader->PayloadOffsetInBytes,    
            _LastECMSDataBulkHeader->PayloadSizeInBytes,
            Bytes
        );
        
        F_Archive Archive = F_Archive::From(Bytes);
        F_ArchiveReadOnlyView ArchiveView = F_ArchiveReadOnlyView::From(Archive);
        ArchiveView.HasDevelopmentBuild = GetEnvironment()->HasDevelopmentBuild();
        
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            ArchiveView >> OutData  
        );
        return true;
    }
}
