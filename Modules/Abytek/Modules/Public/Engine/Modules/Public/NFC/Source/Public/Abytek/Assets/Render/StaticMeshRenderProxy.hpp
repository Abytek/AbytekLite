#pragma once

#include "Abytek/RenderBase/WorldContextRenderProxy.hpp"
#include "Abytek/Assets/StaticMesh.hpp"
#include "Abytek/Geometries/Render/RenderGeometryCommon.hpp"


namespace Abytek
{
    class A_RenderScene;

    struct F_StaticMeshResource_Simple
    {
        U32 Index = 0;
        F_RenderGeometryAllocation GeometryAllocation;
        F_RenderGeometryAllocationStructure_Simple GeometryAllocationStructure;
    };
    
    class ABYTEK_ENGINE_NFC_API F_StaticMeshRenderProxy : public A_WorldContextRenderProxy
    {
    public:
        friend class F_StaticMesh;
        
    private:
#ifdef ABYTEK_DEBUG_INFO
        F_Name _DebugName;
#endif
        E_StaticMeshDataType _DataType = E_StaticMeshDataType::NONE;
        TS_Unmanaged<TF_Vector<F_SimpleMeshData>> _TempSimpleDataList;
        F_StaticMeshSetting _Setting;
        
        TF_Vector<F_StaticMeshResource_Simple> _ResourceList_Simple;
        
    public:
#ifdef ABYTEK_DEBUG_INFO
        ABYTEK_FORCE_INLINE const auto& GetDebugName() const noexcept
        {
            return _DebugName;
        }
#endif
        ABYTEK_FORCE_INLINE auto GetDataType() const noexcept
        {
            return _DataType;
        }
        ABYTEK_FORCE_INLINE const auto& GetSetting() const noexcept
        {
            return _Setting;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetResourceList_Simple() const noexcept
        {
            return _ResourceList_Simple;
        }
        
    public:
        F_StaticMeshRenderProxy(const TW_Valid<F_StaticMesh>& Owner);
        ~F_StaticMeshRenderProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask() override;
        void OnDestroyRenderState_RenderTask() override;
    };
}
