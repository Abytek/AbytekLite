#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveManager.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_Transform.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_InverseTransposeTransform.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_MeshHandle.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"
#include "Abytek/Renderer/GPUData/GPUDataInstanceSet.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"


namespace Abytek
{
    void F_RenderPrimitiveManager::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderPrimitiveManagerBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Scene = BuildParams.Scene;
        
        _GPUData = F_GPUData::Create(GetWorldRenderResource());
#ifdef ABYTEK_DEBUG_INFO
        _GPUData->SetDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".GPUData")
        );
#endif
        RenderPrimitive::F_Data::Init(
            SubmissionItemContainer,
            _GPUData,
            _Scene
        );
        
        _ComponentIndex_Transform = _GPUData->GetComponentTypeIndex<RenderPrimitive::F_Component_Transform>();
        _ComponentIndex_InverseTransposeTransform = _GPUData->GetComponentTypeIndex<RenderPrimitive::F_Component_InverseTransposeTransform>();
        _ComponentIndex_MeshHandle = _GPUData->GetComponentTypeIndex<RenderPrimitive::F_Component_MeshHandle>();
    }
    void F_RenderPrimitiveManager::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _GPUData->Release(SubmissionItemContainer);
        _GPUData = {};
        
        _Scene = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void F_RenderPrimitiveManager::BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _GPUData->BeginUpdate(SubmissionItemContainer);
    }
    void F_RenderPrimitiveManager::EndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _GPUData->EndUpdate(SubmissionItemContainer);
    }
    void F_RenderPrimitiveManager::BeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _GPUData->BeginPostUpdate(SubmissionItemContainer);
    }
    void F_RenderPrimitiveManager::EndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _GPUData->EndPostUpdate(SubmissionItemContainer);
    }
}
