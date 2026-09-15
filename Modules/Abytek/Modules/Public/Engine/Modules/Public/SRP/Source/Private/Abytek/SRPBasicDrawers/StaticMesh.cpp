#include "Abytek/SRPBasicDrawers/StaticMesh.hpp"
#include "Abytek/Assets/Render/StaticMeshRenderProxy.hpp"
#include "Abytek/RenderBase/RenderScene.hpp"
#include "Abytek/RenderBase/RenderViewFamily.hpp"


namespace Abytek
{
    namespace SRPBasicDrawers
    {
        ABYTEK_DEFINE_GLOBAL_RENDER_PIPELINE(F_StaticMeshBinding)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::SRPBasicDrawers::F_StaticMeshBinding"));
        }
        ABYTEK_DEFINE_GLOBAL_RENDER_PIPELINE(F_StaticMeshPipeline)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::SRPBasicDrawers::F_StaticMeshPipeline"));
        }
    }

    void H_SRPStaticMeshDrawer::Render(
        I_RHISubmissionItemContainer& SubmissionItemContainer,
        const TW_Valid<A_RenderView>& View, 
        const TW_Valid<F_StaticMeshRenderProxy> StaticMeshRenderProxy,
        const F_Matrix4x4_F32& StaticMeshTransformMatrix, 
        const F_Vector4_F32& StaticMeshColor, 
        E_RHIFillMode FillMode
    )
    {
        auto ViewUniformBindGroup = View->GetUniformBindGroup();
        auto GeometryGlobalSRVBindGroup = View->GetFamily()->GetScene()->GetGeometryManager()->GetGlobalSRVBindGroup();
        
        auto DataType = StaticMeshRenderProxy->GetDataType();
        
        SRPBasicDrawers::F_StaticMeshBinding::F_DynamicPermutationVector BindingPermutationVector;
        BindingPermutationVector.Get<SRPBasicDrawers::F_StaticMeshPermuation_DataType>() = DataType;
        auto Binding = SRPBasicDrawers::F_StaticMeshBinding::Instantiate(
            View->GetRenderRegistryRuntime(),
            BindingPermutationVector
        );
        
        SRPBasicDrawers::F_StaticMeshPipeline::F_DynamicPermutationVector PipelinePermutationVector;
        PipelinePermutationVector.Get<SRPBasicDrawers::F_StaticMeshPipeline::F_FillMode>() = FillMode;
        PipelinePermutationVector.Get<SRPBasicDrawers::F_StaticMeshPermuation_DataType>() = DataType;
        auto Pipeline = SRPBasicDrawers::F_StaticMeshPipeline::Instantiate(
            View->GetRenderRegistryRuntime(),
            PipelinePermutationVector
        );
        
        SRPBasicDrawers::F_StaticMeshUniformData StaticMeshUniformData;
        StaticMeshUniformData.TransformMatrix = StaticMeshTransformMatrix;
        StaticMeshUniformData.InverseTransposeTransformMatrix = Inverse(
            Transpose(StaticMeshTransformMatrix)    
        );
        StaticMeshUniformData.Color = StaticMeshColor;
        
        if (DataType == E_StaticMeshDataType::SIMPLE)
        {
            const auto& ResourceList = StaticMeshRenderProxy->GetResourceList_Simple();
            for (const auto& Resource : ResourceList)
            {
                auto StaticMeshBindGroup = Binding.CreateBindGroup();
                StaticMeshBindGroup->BindRTV(ABYTEK_NAME("RTV"), View->GetRTV());
                StaticMeshBindGroup->BindDSV(ABYTEK_NAME("DSV"), View->GetDSV());
                StaticMeshBindGroup->BindUniformData(ABYTEK_NAME("StaticMeshUniformData"), StaticMeshUniformData);
                {
                    auto GeometryUniformData = F_StaticMeshGeometryUniformData_Simple::Make(
                        Resource.Index,
                        Resource.GeometryAllocation,
                        Resource.GeometryAllocationStructure
                    );
                    StaticMeshBindGroup->BindUniformData(ABYTEK_NAME("StaticMeshGeometryUniformData_Simple"), GeometryUniformData);
                }
                StaticMeshBindGroup->Commit();
        
                H_RHIPassUtilities::DrawNonIndexed(
                    SubmissionItemContainer,
                    Pipeline.AcquirePipelineState(),
                    { 
                        ViewUniformBindGroup,
                        StaticMeshBindGroup,
                        GeometryGlobalSRVBindGroup
                    },
                    View->GetDefaultViewportScissorConfig(),
                    F_RHIDrawNonIndexedConfig::Make(
                        Resource.GeometryAllocationStructure.NumIndices
                    )
                );
            }
        }
        else
        {
            ABYTEK_LOG_FATAL() << "Invalid static mesh data type: " << static_cast<U32>(DataType);
        }
    }
}
