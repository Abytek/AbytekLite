#include "Abytek/SRPBasicDrawers/StaticMesh.hpp"

#include "Abytek/SimplePrimitive/SRPRenderPrimitiveProcessor_Simple.hpp"
#include "Abytek/SRPRenderView.hpp"
#include "Abytek/Assets/Render/StaticMeshRenderProxy.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/Renderer/RenderViewFamily.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"


namespace Abytek
{
    void H_SRPStaticMeshDrawer::RenderAdvanced(
        const F_AdvancedParams& AdvancedParams,
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TW_Valid<A_RenderView>& View, 
        const TW_Valid<F_StaticMeshRenderProxy> StaticMeshRenderProxy,
        const F_Matrix4x4_F32& StaticMeshTransformMatrix, 
        const F_Vector4_F32& StaticMeshColor, 
        E_RHIFillMode FillMode
    )
    {
        auto CastedView = View.FastCast<F_SRPRenderView>();
        
        auto Scene = View->GetWorldRenderResource()->GetScene();
        
        auto ViewUniformBindGroup = View->GetUniformBindGroup();
        auto GeometryGlobalSRVBindGroup = View->GetFamily()->GetScene()->GetGeometryStorage()->GetGlobalSRVBindGroup();
        
        auto DataType = StaticMeshRenderProxy->GetDataType();
        
        SRPBasicDrawers::F_StaticMeshBinding::F_DynamicPermutationVector BindingPermutationVector;
        BindingPermutationVector.Get<SRPBasicDrawers::F_StaticMeshPermuation_DataType>() = DataType;
        BindingPermutationVector.Get<SRPBasicDrawers::F_StaticMeshPermuation_OutputMode>() = AdvancedParams.OutputMode;
        auto Binding = SRPBasicDrawers::F_StaticMeshBinding::Instantiate(
            View->GetRenderRegistryRuntime(),
            BindingPermutationVector
        );
        
        SRPBasicDrawers::F_StaticMeshPipeline::F_DynamicPermutationVector PipelinePermutationVector;
        PipelinePermutationVector.Get<SRPBasicDrawers::F_StaticMeshPipeline::F_FillMode>() = FillMode;
        PipelinePermutationVector.Get<SRPBasicDrawers::F_StaticMeshPermuation_DataType>() = DataType;
        PipelinePermutationVector.Get<SRPBasicDrawers::F_StaticMeshPermuation_OutputMode>() = AdvancedParams.OutputMode;
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
        
        auto ApplyOutputModeToBindGroup = [&](const TS<A_RHIBindGroup>& BindGroup)
        {
            switch (AdvancedParams.OutputMode)
            {
            case SRPBasicDrawers::E_StaticMeshOutputMode::RTV_DSV:
                {
                    BindGroup->BindRTV(ABYTEK_NAME("RTV"), View->GetRTV());
                    BindGroup->BindDSV(ABYTEK_NAME("DSV"), View->GetDSV());
                }
                break;
            case SRPBasicDrawers::E_StaticMeshOutputMode::VISIBILITY_BUFFER:
                {
                    CastedView->GetOpaqueVisibilityBuffer().Bind(
                        BindGroup,
                        ABYTEK_NAME("OpaqueVisibilityBuffer"),
                        true
                    );
                        
                    SRPBasicDrawers::F_StaticMeshVisibilityConfig VisibilityConfig;
                    VisibilityConfig.PrimitiveProcessorId = AdvancedParams.PrimitiveProcessorId;
                    BindGroup->BindUniformData(ABYTEK_NAME("StaticMeshVisibilityConfig"), VisibilityConfig);
                }
                break;
            default:
                ABYTEK_LOG_FATAL() << "Unknown output mode: " << static_cast<U32>(AdvancedParams.OutputMode);
            }
        };
        
        switch (DataType)
        {
        case E_StaticMeshDataType::SIMPLE:
            {
                const auto& ResourceList = StaticMeshRenderProxy->GetResourceList_Simple();
                for (const auto& Resource : ResourceList)
                {
                    auto StaticMeshBindGroup = Binding.CreateBindGroup();
                    StaticMeshBindGroup->BindUniformData(ABYTEK_NAME("StaticMeshUniformData"), StaticMeshUniformData);
                    {
                        auto GeometryUniformData = F_StaticMeshGeometryUniformData_Simple::Make(
                            Resource.Index,
                            Resource.GeometryAllocation,
                            Resource.GeometryAllocationStructure
                        );
                        StaticMeshBindGroup->BindUniformData(ABYTEK_NAME("StaticMeshGeometryUniformData_Simple"), GeometryUniformData);
                    }
                    ApplyOutputModeToBindGroup(StaticMeshBindGroup);
                    StaticMeshBindGroup->Commit();
        
                    H_RHISubmissionUtilities::DrawNonIndexed(
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
            break;
        case E_StaticMeshDataType::ECMS:
            {
                const auto& InstancedMeshletBuffer = CastedView->GetInstancedMeshletBuffer_ECMS();
                
                const auto& ResourceList = StaticMeshRenderProxy->GetResourceList_ECMS();
                for (const auto& Resource : ResourceList)
                {
                    auto StaticMeshBindGroup = Binding.CreateBindGroup();
                    StaticMeshBindGroup->BindUniformData(ABYTEK_NAME("StaticMeshUniformData"), StaticMeshUniformData);
                    {
                        auto GeometryUniformData = F_StaticMeshGeometryUniformData_ECMS::Make(
                            Resource.Index,
                            Resource.GeometryAllocation,
                            Resource.GeometryAllocationStructure
                        );
                        StaticMeshBindGroup->BindUniformData(ABYTEK_NAME("StaticMeshGeometryUniformData_ECMS"), GeometryUniformData);
                    }
                    InstancedMeshletBuffer.Bind(StaticMeshBindGroup, ABYTEK_NAME("InstancedMeshlets"), true);
                    ApplyOutputModeToBindGroup(StaticMeshBindGroup);
                    StaticMeshBindGroup->Commit();
        
                    H_RHISubmissionUtilities::DrawDispatchMesh(
                        SubmissionItemContainer,
                        Pipeline.AcquirePipelineState(),
                        { 
                            ViewUniformBindGroup,
                            StaticMeshBindGroup,
                            GeometryGlobalSRVBindGroup
                        },
                        View->GetDefaultViewportScissorConfig(),
                        F_RHIDrawDispatchMeshConfig::Make(
                            F_Vector3_U32(
                                Resource.GeometryAllocationStructure.NumMeshlets, 
                                1, 
                                1
                            )
                        )
                    );
                }
            }
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown mesh data type: " << static_cast<U32>(DataType);
        }
    }
    void H_SRPStaticMeshDrawer::Render(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TW_Valid<A_RenderView>& View, 
        const TW_Valid<F_StaticMeshRenderProxy> StaticMeshRenderProxy,
        const F_Matrix4x4_F32& StaticMeshTransformMatrix, 
        const F_Vector4_F32& StaticMeshColor, 
        E_RHIFillMode FillMode
    )
    {
        F_AdvancedParams AdvancedParams;
        RenderAdvanced(
            AdvancedParams,
            SubmissionItemContainer,
            View,
            StaticMeshRenderProxy,
            StaticMeshTransformMatrix,
            StaticMeshColor,
            FillMode
        );
    }
}
