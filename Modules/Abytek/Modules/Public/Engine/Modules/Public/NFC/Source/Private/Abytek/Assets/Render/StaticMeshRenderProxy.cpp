#include "Abytek/Assets/Render/StaticMeshRenderProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryStorage.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryPage.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/Renderer/RenderScene.hpp"


namespace Abytek
{
    F_StaticMeshRenderProxy::F_StaticMeshRenderProxy(const TW_Valid<F_StaticMesh>& Owner) :
        A_WorldContextRenderProxy(Owner)
    {
    }
    F_StaticMeshRenderProxy::~F_StaticMeshRenderProxy()
    {
    }

    void F_StaticMeshRenderProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto RHIContext = H_RHI::GetMainContext();
        
        if (_DataType == E_StaticMeshDataType::SIMPLE)
        {
            auto Scene = GetWorldRenderResource()->GetScene();
            auto GeometryStorage = Scene->GetGeometryStorage();
            
            const auto& SimpleDataList = *_TempSimpleDataList;
            U32 NumSimpleData = static_cast<U32>(SimpleDataList.size());
            for (U32 Idx = 0; Idx < NumSimpleData; ++Idx)
            {
                const auto& SimpleData = SimpleDataList[Idx];
                F_StaticMeshResource_Simple Resource;
                Resource.Index = Idx;
                if (GeometryStorage->AddMeshData_Simple(SubmissionItemContainer, SimpleData, Resource.GeometryAllocation, Resource.GeometryAllocationStructure))
                {
                    _ResourceList_Simple.push_back(Resource);
                }
            }
            
            H_Frame::EnqueueCommand<E_FrameParamType::DISPLAY, E_FrameParamType::RENDER>(
                [TempSimpleDataList = ABYTEK_MOVE(_TempSimpleDataList)]
                {}    
            );
        }
    }
    void F_StaticMeshRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        if (_DataType == E_StaticMeshDataType::SIMPLE)
        {
            auto Scene = GetWorldRenderResource()->GetScene();
            auto GeometryStorage = Scene->GetGeometryStorage();
            
            for (const auto& Resource : _ResourceList_Simple)
            {
                GeometryStorage->RemoveMeshData_Simple(SubmissionItemContainer, Resource.GeometryAllocation);
            }
            _ResourceList_Simple = {};
        }
        _DataType = E_StaticMeshDataType::NONE;
    }
}
