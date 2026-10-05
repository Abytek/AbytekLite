#include "Abytek/Assets/Render/StaticMeshRenderProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryStorage.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryPage.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/Renderer/RenderScene.hpp"


namespace Abytek
{
    F_StaticMeshRenderProxy::F_StaticMeshRenderProxy(const TW_Valid<F_StaticMesh>& Owner) :
        A_WorldRenderResourceChild(Owner)
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
                const auto& SimpleData = *_TempSimpleData;
                F_StaticMeshResource_Simple Resource;
                if (
                    GeometryStorage->AddMeshData_Simple(
                        SubmissionItemContainer, 
                        SimpleData, 
                        Resource.GeometryAllocation,
                        Resource.GeometryAllocationStructure
                    )
                )
                {
                    _Resource_Simple = Resource;
                }
                H_Frame::EnqueueCommand<E_FrameParamType::DISPLAY, E_FrameParamType::RENDER>(
                    [TempSimpleData = ABYTEK_MOVE(_TempSimpleData)]
                    {}    
                );
            }
            break;
        case E_StaticMeshDataType::ECMS:
            {
                const auto& ECMSData = *_TempECMSData;
                F_StaticMeshResource_ECMS Resource;
                if (
                    GeometryStorage->AddMeshData_ECMS(
                        SubmissionItemContainer, 
                        ECMSData, 
                        Resource.GeometryAllocation,
                        Resource.GeometryAllocationStructure
                    )
                )
                {
                    _Resource_ECMS = Resource;
                }
                H_Frame::EnqueueCommand<E_FrameParamType::DISPLAY, E_FrameParamType::RENDER>(
                    [TempECMSData = ABYTEK_MOVE(_TempECMSData)]
                    {}    
                );
            }
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown mesh data type: " << static_cast<U32>(_DataType);
        }
        
        // Resource LOD
        {
            F_StaticMeshResource_LOD Resource;
            if (
                GeometryStorage->AddMeshData_LOD(
                    SubmissionItemContainer, 
                    _LevelRenderProxies, 
                    _DataType,
                    Resource.GeometryAllocation,
                    Resource.GeometryAllocationStructure
                )
            )
            {
                _Resource_LOD = Resource;
            }
        }
    }
    void F_StaticMeshRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto Scene = GetWorldRenderResource()->GetScene();
        auto GeometryStorage = Scene->GetGeometryStorage();
        
        if (_Resource_LOD)
        {
            GeometryStorage->RemoveMeshData_LOD(SubmissionItemContainer, _Resource_LOD->GeometryAllocation);
            _Resource_LOD = {};
        }
        
        switch (_DataType)
        {
        case E_StaticMeshDataType::SIMPLE:
            if (_Resource_Simple)
            {
                GeometryStorage->RemoveMeshData_Simple(SubmissionItemContainer, _Resource_Simple->GeometryAllocation);
                _Resource_Simple = {};
            }
            break;
        case E_StaticMeshDataType::ECMS:
            if (_Resource_ECMS)
            {
                GeometryStorage->RemoveMeshData_ECMS(SubmissionItemContainer, _Resource_ECMS->GeometryAllocation);
                _Resource_ECMS = {};
            }
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown mesh data type: " << static_cast<U32>(_DataType);
        }
        _LevelRenderProxies = {};
        _DataType = E_StaticMeshDataType::NONE;
    }
}
