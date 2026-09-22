#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/Cookable.hpp"
#include "Abytek/Renderable.hpp"
#include "Abytek/Assets/SimpleMeshData.hpp"

   
namespace Abytek
{
    class F_StaticMeshRenderProxy;
    
    enum class E_StaticMeshDataType : U8
    {
        NONE,
        
        SIMPLE,
        
        DEFAULT = SIMPLE
    };
    
    struct F_StaticMeshDataBulkHeader
    {
        U64 PayloadOffsetInBytes = 0;
        U64 PayloadSizeInBytes = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_StaticMeshDataBulkHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PayloadOffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PayloadSizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_StaticMeshDataBulkHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PayloadOffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PayloadSizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    struct F_StaticMeshSetting
    {
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_StaticMeshSetting);
    };
    
    enum class E_StaticMeshFileImportFlag : U8
    {
        NONE = 0x0,
        DEFAULT = NONE
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_StaticMeshFileImportFlag);
    
    struct F_StaticMeshFileImportConfig
    {
        E_StaticMeshDataType DataType = E_StaticMeshDataType::DEFAULT;
        E_StaticMeshFileImportFlag Flags = E_StaticMeshFileImportFlag::DEFAULT;
    };
    
    class ABYTEK_ENGINE_NFC_API F_StaticMesh : public A_WorldContext, public A_Renderable, public I_Cookable
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(F_StaticMesh);
        
    protected:
        E_StaticMeshDataType _DataType = E_StaticMeshDataType::NONE;
        TF_Optional<F_StaticMeshDataBulkHeader> _LastSimpleDataBulkHeader;
        TF_Optional<TF_Vector<F_SimpleMeshData>> _NewSimpleDataList;
        
        TF_Optional<F_StaticMeshDataBulkHeader> _TempSerializationData_LastSimpleDataBulkHeader;
        
        F_StaticMeshSetting _Setting;
        
    public:
        ABYTEK_FORCE_INLINE auto GetDataType() const noexcept
        {
            return _DataType;
        }
        ABYTEK_FORCE_INLINE const auto& GetLastSimpleDataBulkHeader() const noexcept
        {
            return _LastSimpleDataBulkHeader;
        }
        ABYTEK_FORCE_INLINE const auto& GetNewSimpleDataList() const noexcept
        {
            return _NewSimpleDataList;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetSetting() const noexcept
        {
            return _Setting;
        }
        
    public:
        F_StaticMesh(const F_SerializableObjectInitParams& InitParam);
        ~F_StaticMesh() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    protected:
        F_FeedbackStatus BinarySerialize(F_SerializableObjectBinarySerializeParams& Params) override;
        F_FeedbackStatus BinaryDeserialize(F_SerializableObjectBinaryDeserializeParams& Params) override;
        
    protected:
        void OnCleanUpAfterSaving(const TW_Valid<F_SerializableEnvironment>& Environment) override;
        
    public:
        B8 IsRenderable() const override;
        
    protected:
        TS<A_RenderProxy> CreateRenderProxy() override;
        void OnCreateRenderState() override;
        void OnDestroyRenderState() override;
        
    public:
        void Import(const TF_Span<const U8>& Bytes, const F_StaticMeshFileImportConfig& Config = {}, const TF_Optional<F_StaticMeshSetting>& Setting = {});
        void Import(const F_Text& FilePath, const F_StaticMeshFileImportConfig& Config = {}, const TF_Optional<F_StaticMeshSetting>& Setting = {});
        void Import(const TF_Vector<F_SimpleMeshData>& SimpleDataList, const TF_Optional<F_StaticMeshSetting>& Setting = {});
        
    public:
        void UpdateSetting(const F_StaticMeshSetting& Setting);
        
    public:
        B8 LoadSimpleDataList(TF_Vector<F_SimpleMeshData>& OutData);
    };
}
