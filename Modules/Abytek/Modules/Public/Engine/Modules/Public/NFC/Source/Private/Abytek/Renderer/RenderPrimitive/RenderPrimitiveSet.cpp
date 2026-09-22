#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveManager.hpp"
#include "Abytek/Renderer/GPUData/GPUDataInstanceSet.hpp"


namespace Abytek
{
    void F_RenderPrimitiveSet::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderPrimitiveSetBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Manager = BuildParams.Manager;
        _Num = BuildParams.Num;
        
        F_GPUDataInstanceSetBuildParams GPUDataInstanceSetBuildParams;
        GPUDataInstanceSetBuildParams.GPUData = _Manager->GetGPUData().Weak();
        GPUDataInstanceSetBuildParams.Num = _Num;
        _GPUDataInstanceSet = F_GPUDataInstanceSet::CreateAndInit(
            GetWorldRenderResource(),
            SubmissionItemContainer, 
            GPUDataInstanceSetBuildParams
        );
    }
    void F_RenderPrimitiveSet::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _GPUDataInstanceSet->Release(SubmissionItemContainer);
        _GPUDataInstanceSet = {};
        
        _Num = 0;
        _Manager = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void F_RenderPrimitiveSet::UploadComponents_Transform(const RenderPrimitive::F_Component_Transform* ValuePtr)
    {
        _GPUDataInstanceSet->UploadComponents(
            _Manager->GetComponentIndex_Transform(),
            ValuePtr
        );
    }
    void F_RenderPrimitiveSet::UploadComponents_InverseTransposeTransform(const RenderPrimitive::F_Component_InverseTransposeTransform* ValuePtr)
    {
        _GPUDataInstanceSet->UploadComponents(
            _Manager->GetComponentIndex_InverseTransposeTransform(),
            ValuePtr
        );
    }
    void F_RenderPrimitiveSet::UploadComponents_MeshHandle(const RenderPrimitive::F_Component_MeshHandle* ValuePtr)
    {
        _GPUDataInstanceSet->UploadComponents(
            _Manager->GetComponentIndex_MeshHandle(),
            ValuePtr
        );
    }
}
