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
        if (_DataType == E_StaticMeshDataType::SIMPLE)
        {
            TF_Vector<F_SimpleMeshData> Data;
            if (LoadSimpleDataList(Data))
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
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_StaticMesh::BinaryDeserialize(F_SerializableObjectBinaryDeserializeParams& Params)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_SerializableObject::BinaryDeserialize(Params)
        );
        
        ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView >> _DataType);
        if (_DataType == E_StaticMeshDataType::SIMPLE)
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
        return F_FeedbackStatus::MakeSucceeded();
    }

    void F_StaticMesh::OnCleanUpAfterSaving(const TW_Valid<F_SerializableEnvironment>& Environment)
    {
        if (Environment == GetEnvironment())
        {
            _LastSimpleDataBulkHeader = _TempSerializationData_LastSimpleDataBulkHeader;
            _NewSimpleDataList = {};
        }
        _TempSerializationData_LastSimpleDataBulkHeader = {};
    }

    B8 F_StaticMesh::IsRenderable() const
    {
        if (!A_Renderable::IsRenderable())
        {
            return false;
        }
        return (static_cast<B8>(_LastSimpleDataBulkHeader) && GetPackageName()) || static_cast<B8>(_NewSimpleDataList);
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
#ifdef ABYTEK_DEBUG_INFO
                , CachedName = GetName()
#endif
            ]() mutable
            {
                RenderProxy->_DataType = CachedDataType;
#ifdef ABYTEK_DEBUG_INFO
                RenderProxy->_DebugName = CachedName;
#endif
            }
        );
        if (_DataType == E_StaticMeshDataType::SIMPLE)
        {
            TF_Vector<F_SimpleMeshData> SimpleDataList;
            B8 Status = LoadSimpleDataList(SimpleDataList);
            ABYTEK_ENGINE_NFC_ASSERT(Status) << "Failed to load simple data while being renderable";
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_StaticMeshRenderProxy>(), 
                    CachedSimpleDataList = ABYTEK_MOVE(SimpleDataList),
                    CachedSetting = ABYTEK_MOVE(_Setting)
                ]() mutable
                {
                    RenderProxy->_TempSimpleDataList = TS_Unmanaged<TF_Vector<F_SimpleMeshData>>()(
                        ABYTEK_MOVE(CachedSimpleDataList)    
                    );
                    RenderProxy->_Setting = ABYTEK_MOVE(CachedSetting);
                }
            );
        }
    }
    void F_StaticMesh::OnDestroyRenderState()
    {
    }

    namespace Internal::StaticMesh::Simple
    {
        F_FeedbackStatus Decode(const TF_Span<const U8>& Bytes, TF_Vector<F_SimpleMeshData>& OutSimpleDataList)
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
    void F_StaticMesh::Import(const TF_Span<const U8>& Bytes, const F_StaticMeshFileImportConfig& Config, const TF_Optional<F_StaticMeshSetting>& Setting)
    {
        TF_Vector<F_SimpleMeshData> SimpleDataList;
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            Internal::StaticMesh::Simple::Decode(Bytes, SimpleDataList)  
        );
        Import(SimpleDataList, Setting);
    }
    void F_StaticMesh::Import(const F_Text& FilePath, const F_StaticMeshFileImportConfig& Config, const TF_Optional<F_StaticMeshSetting>& Setting)
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
    void F_StaticMesh::Import(const TF_Vector<F_SimpleMeshData>& SimpleDataList, const TF_Optional<F_StaticMeshSetting>& Setting)
    {
        _DataType = E_StaticMeshDataType::SIMPLE;
        _NewSimpleDataList = SimpleDataList;
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

    B8 F_StaticMesh::LoadSimpleDataList(TF_Vector<F_SimpleMeshData>& OutData)
    {
        ABYTEK_ENGINE_NFC_ASSERT(_DataType == E_StaticMeshDataType::SIMPLE);
        if (_NewSimpleDataList)
        {
            OutData = *_NewSimpleDataList;
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
}
