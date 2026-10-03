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
        ABYTEK_RHI_CAPTURE_EVENT_SCOPE(
            SubmissionItemContainer,
            ABYTEK_TEXT("Abytek::F_StaticMeshRenderProxy::CreateRenderState(")
            + *GetDebugName()
            + ABYTEK_TEXT(")")
        );
        
        auto RHIContext = H_RHI::GetMainContext();
        
        auto Scene = GetWorldRenderResource()->GetScene();
        auto GeometryStorage = Scene->GetGeometryStorage();
        
        switch (_DataType)
        {
        case E_StaticMeshDataType::SIMPLE:
            {
                const auto& SimpleDataList = *_TempSimpleDataList;
                U32 NumSimpleData = static_cast<U32>(SimpleDataList.size());
                for (U32 Idx = 0; Idx < NumSimpleData; ++Idx)
                {
                    const auto& SimpleData = SimpleDataList[Idx];
                    F_StaticMeshResource_Simple Resource;
                    Resource.Index = Idx;
                    if (
                        GeometryStorage->AddMeshData_Simple(
                            SubmissionItemContainer, 
                            SimpleData, 
                            Resource.GeometryAllocation, 
                            Resource.GeometryAllocationStructure
                        )
                    )
                    {
                        _ResourceList_Simple.push_back(Resource);
                    }
                }
            
                H_Frame::EnqueueCommand<E_FrameParamType::DISPLAY, E_FrameParamType::RENDER>(
                    [TempSimpleDataList = ABYTEK_MOVE(_TempSimpleDataList)]
                    {}    
                );
            }
            break;
        case E_StaticMeshDataType::ECMS:
            {
                const auto& ECMSDataList = *_TempECMSDataList;
                U32 NumECMSData = static_cast<U32>(ECMSDataList.size());
                for (U32 Idx = 0; Idx < NumECMSData; ++Idx)
                {
                    const auto& ECMSData = ECMSDataList[Idx];
                    F_StaticMeshResource_ECMS Resource;
                    Resource.Index = Idx;
                    if (
                        GeometryStorage->AddMeshData_ECMS(
                            SubmissionItemContainer, 
                            ECMSData, 
                            Resource.GeometryAllocation, 
                            Resource.GeometryAllocationStructure
                        )
                    )
                    {
                        _ResourceList_ECMS.push_back(Resource);
                    }
                }
            
                H_Frame::EnqueueCommand<E_FrameParamType::DISPLAY, E_FrameParamType::RENDER>(
                    [TempECMSDataList = ABYTEK_MOVE(_TempECMSDataList)]
                    {}    
                );
            }
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown mesh data type: " << static_cast<U32>(_DataType);
        }
    }
    void F_StaticMeshRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        switch (_DataType)
        {
        case E_StaticMeshDataType::SIMPLE:
            {
                auto Scene = GetWorldRenderResource()->GetScene();
                auto GeometryStorage = Scene->GetGeometryStorage();
            
                for (const auto& Resource : _ResourceList_Simple)
                {
                    GeometryStorage->RemoveMeshData_Simple(SubmissionItemContainer, Resource.GeometryAllocation);
                }
                _ResourceList_Simple = {};
            }
            break;
        case E_StaticMeshDataType::ECMS:
            {
                auto Scene = GetWorldRenderResource()->GetScene();
                auto GeometryStorage = Scene->GetGeometryStorage();
            
                for (const auto& Resource : _ResourceList_ECMS)
                {
                    GeometryStorage->RemoveMeshData_ECMS(SubmissionItemContainer, Resource.GeometryAllocation);
                }
                _ResourceList_ECMS = {};
            }
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown mesh data type: " << static_cast<U32>(_DataType);
        }
        _DataType = E_StaticMeshDataType::NONE;
    }
}
