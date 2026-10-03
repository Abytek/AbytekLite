#pragma once

#include "Abytek/Renderer/WorldContextRenderProxy.hpp"
#include "Abytek/Assets/StaticMesh.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryCommon.hpp"


namespace Abytek
{
    class A_RenderScene;

    struct F_StaticMeshResource_Simple
    {
        F_RenderGeometryAllocation GeometryAllocation;
        F_RenderGeometryAllocationStructure_Simple GeometryAllocationStructure;
    };
    struct F_StaticMeshResource_ECMS
    {
        F_RenderGeometryAllocation GeometryAllocation;
        F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure;
    };
    
    class ABYTEK_ENGINE_NFC_API F_StaticMeshRenderProxy : public A_WorldContextRenderProxy
    {
    public:
        friend class F_StaticMesh;
        
    private:
        E_StaticMeshDataType _DataType = E_StaticMeshDataType::NONE;
        TS_Unmanaged<F_SimpleMeshData> _TempSimpleData;
        TS_Unmanaged<F_ECMSMeshData> _TempECMSData;
        F_StaticMeshSetting _Setting;
        
        TF_Optional<F_StaticMeshResource_Simple> _Resource_Simple;
        TF_Optional<F_StaticMeshResource_ECMS> _Resource_ECMS;
        
    public:
        ABYTEK_FORCE_INLINE auto GetDataType() const noexcept
        {
            return _DataType;
        }
        ABYTEK_FORCE_INLINE const auto& GetSetting() const noexcept
        {
            return _Setting;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetResource_Simple() const noexcept
        {
            return _Resource_Simple;
        }
        ABYTEK_FORCE_INLINE const auto& GetResource_ECMS() const noexcept
        {
            return _Resource_ECMS;
        }
        
    public:
        F_StaticMeshRenderProxy(const TW_Valid<F_StaticMesh>& Owner);
        ~F_StaticMeshRenderProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}
