#include "Abytek/SRPBasicDrawers/Cube.hpp"


namespace Abytek
{
    void H_SRPCubeDrawer::Render(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TW_Valid<A_RenderView>& View, 
        const F_Matrix4x4_F32& CubeTransformMatrix, 
        const F_Vector4_F32& CubeColor, 
        E_RHIFillMode FillMode
    )
    {
        auto ViewUniformBindGroup = View->GetUniformBindGroup();
        
        SRPBasicDrawers::F_CubeUniformData CubeUniformData;
        CubeUniformData.TransformMatrix = CubeTransformMatrix;
        CubeUniformData.Color = CubeColor;
        auto CubeBindGroup = SRPBasicDrawers::F_CubeBinding::Instantiate(
            View->GetRenderRegistryRuntime()  
        ).CreateBindGroup();
        CubeBindGroup->BindRTV(ABYTEK_NAME("RTV"), View->GetRTV());
        CubeBindGroup->BindUniformData(ABYTEK_NAME("CubeUniformData"), CubeUniformData);
        CubeBindGroup->Commit();
        
        SRPBasicDrawers::F_CubePipeline::F_DynamicPermutationVector PermutationVector;
        PermutationVector.Get<SRPBasicDrawers::F_CubePipeline::F_FillMode>() = FillMode;
        
        H_RHISubmissionUtilities::DrawNonIndexed(
            SubmissionItemContainer,
            SRPBasicDrawers::F_CubePipeline::Instantiate(
                View->GetRenderRegistryRuntime(), 
                PermutationVector
            ).AcquirePipelineState(),
            { 
                ViewUniformBindGroup,
                CubeBindGroup
            },
            View->GetDefaultViewportScissorConfig(),
            F_RHIDrawNonIndexedConfig::Make(36)
        );
    }
}
