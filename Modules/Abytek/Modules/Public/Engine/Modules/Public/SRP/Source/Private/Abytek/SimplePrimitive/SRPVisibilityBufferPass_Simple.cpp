#include "Abytek/SimplePrimitive/SRPVisibilityBufferPass_Simple.hpp"
#include "Abytek/SimplePrimitive/SRPRenderPrimitiveProcessor_Simple.hpp"
#include "Abytek/SimplePrimitive/SRPRenderPrimitiveSet_Simple.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"
#include "Abytek/Renderer/GPUData/GPUDataInstanceSet.hpp"
#include "Abytek/SRPRenderView.hpp"
#include "Abytek/SRPRenderViewFamily.hpp"
#include "Abytek/SRPRenderScene.hpp"
#include "Abytek/ActorComponents/Render/StaticMeshComponentRenderProxy.hpp"


namespace Abytek
{
    namespace SRP::SimplePrimitive::VisibilityBufferPass
    {
        void Invoke(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,   
            const TW_Valid<A_RenderView>& View
        )
        {
            ABYTEK_RHI_CAPTURE_EVENT_SCOPE(
                SubmissionItemContainer,
                ABYTEK_TEXT("Abytek::SRP::SimplePrimitive::VisibilityBufferPass(")
                + *View->GetDebugName()
                + ABYTEK_TEXT(")")
            );
            
            auto CastedView = View.FastCast<F_SRPRenderView>();
            auto WorldRenderResource = View->GetWorldRenderResource();
            auto Scene = WorldRenderResource->GetScene();
            auto GeometryStorage = Scene->GetGeometryStorage();
            auto Processor = Scene->GetPrimitiveProcessor_Simple();
            auto ProcessorGPUData = Processor->GetGPUData();
            
            auto ViewUniformBindGroup = View->GetUniformBindGroup();
            auto GeometryGlobalSRVBindGroup = GeometryStorage->GetGlobalSRVBindGroup();
            
            for (const auto& PrimitiveSet : Processor->GetPrimitiveSets())
            {
                auto CastedPrimitiveSet = PrimitiveSet.FastCast<F_SRPRenderPrimitiveSet_Simple>();
                
                const auto& StaticMeshComponentRenderProxy = CastedPrimitiveSet->GetStaticMeshComponentRenderProxy();
                const auto& Resource = StaticMeshComponentRenderProxy->GetStaticMeshRenderProxy()->GetResource_ECMS();
        
                F_Binding::F_DynamicPermutationVector BindingPermutationVector;
                auto Binding = F_Binding::Instantiate(
                    View->GetRenderRegistryRuntime(),
                    BindingPermutationVector
                );
        
                F_Pipeline::F_DynamicPermutationVector PipelinePermutationVector;
                auto Pipeline = F_Pipeline::Instantiate(
                    View->GetRenderRegistryRuntime(),
                    PipelinePermutationVector
                );
        
                const auto& InstancedMeshletBuffer = CastedView->GetInstancedMeshletBuffer_ECMS();
                
                auto PrimitiveBindGroup = Binding.CreateBindGroup();
                {
                    F_PrimitiveVisibilityConfig VisibilityConfig;
                    VisibilityConfig.PrimitiveAddress = F_GPUDataInstanceAddress::From(PrimitiveSet->GetGPUDataInstanceSet()->GetAllocation());
                    VisibilityConfig.PrimitiveProcessorId = Processor->GetId();
                    PrimitiveBindGroup->BindUniformData(ABYTEK_NAME("PrimitiveVisibilityConfig"), VisibilityConfig);
                }
                InstancedMeshletBuffer.Bind(PrimitiveBindGroup, ABYTEK_NAME("InstancedMeshlets"), true);
                CastedView->GetOpaqueVisibilityBuffer().Bind(
                    PrimitiveBindGroup,
                    ABYTEK_NAME("OpaqueVisibilityBuffer"),
                    true
                );
                PrimitiveBindGroup->Commit();
                
                F_RHIBindGroupSet BindGroups = { 
                    ViewUniformBindGroup,
                    PrimitiveBindGroup,
                    GeometryGlobalSRVBindGroup,
                    ProcessorGPUData->GetInstanceSetHeaderBindGroup(),
                    RenderPrimitive::F_Component_Transform::GetBindGroup<RenderPrimitive::F_Data_Simple>(ProcessorGPUData),
                    RenderPrimitive::F_Component_InverseTransposeTransform::GetBindGroup<RenderPrimitive::F_Data_Simple>(ProcessorGPUData),
                    RenderPrimitive::F_Component_GeometryAddress_ECMS::GetBindGroup<RenderPrimitive::F_Data_Simple>(ProcessorGPUData)
                };
    
                H_RHISubmissionUtilities::DrawDispatchMesh(
                    SubmissionItemContainer,
                    Pipeline.AcquirePipelineState(),
                    BindGroups,
                    View->GetDefaultViewportScissorConfig(),
                    F_RHIDrawDispatchMeshConfig::Make(
                        F_Vector3_U32(
                            Resource->GeometryAllocationStructure.NumMeshlets, 
                            1, 
                            1
                        )
                    )
                );
            }
        }
    }
}
