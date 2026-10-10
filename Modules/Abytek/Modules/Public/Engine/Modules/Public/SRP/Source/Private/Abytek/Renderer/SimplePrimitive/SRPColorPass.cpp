#include "Abytek/Renderer/SimplePrimitive/SRPColorPass.hpp"
#include "Abytek/RenderCoreExtensions.hpp"
#include "Abytek/Renderer/SimplePrimitive/SRPRenderPrimitiveProcessor.hpp"
#include "Abytek/Renderer/SimplePrimitive/SRPRenderPrimitiveSet.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"
#include "Abytek/Renderer/GPUData/GPUDataInstanceSet.hpp"
#include "Abytek/Renderer/SRPRenderView.hpp"
#include "Abytek/Renderer/SRPRenderViewFamily.hpp"
#include "Abytek/Renderer/SRPRenderScene.hpp"
#include "Abytek/ActorComponents/Render/StaticMeshComponentRenderProxy.hpp"


namespace Abytek
{
    namespace SRP::SimplePrimitive::ColorPass 
    {
        void Invoke(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,   
            const TW_Valid<A_RenderView>& View
        )
        {
            ABYTEK_RHI_CAPTURE_EVENT_SCOPE(
                SubmissionItemContainer,
                ABYTEK_TEXT("Abytek::SRP::SimplePrimitive::ColorPass(")
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
            
            auto Resolution = View->GetResolution();
            
            F_RHITextureBuildParams TempDepthTextureBuildParams;
            TempDepthTextureBuildParams.Context = H_RHI::GetMainContext().Weak();
            TempDepthTextureBuildParams.Format = E_RHIFormat::R32_FLOAT;
            TempDepthTextureBuildParams.TextureAspect.Width = Resolution.X;
            TempDepthTextureBuildParams.TextureAspect.Height = Resolution.Y;
            TempDepthTextureBuildParams.AccessCapabilities = (
                F_RHIResourceAccess::MakeSRVCapabilities()    
                | F_RHIResourceAccess::MakeUAVCapabilities()    
            );
            TempDepthTextureBuildParams.AdditionalFlags |= E_RHIResourceAdditionalFlag::TRANSIENT;
            auto TempDepthTexture = RACreateAndBuildShared<A_RHIResource>(TempDepthTextureBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            TempDepthTexture->SetDebugName(
                ABYTEK_TEXT("Abytek::SRP::SimplePrimitive::ColorPass::TempDepthTexture(")
                + *View->GetDebugName()
                + ABYTEK_TEXT(")")    
            );
#endif
            
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
            PrimitiveBindGroup->BindResourceView(
                ABYTEK_NAME("ColorTexture"),
                View->GetRTV()->GetResource()
            );
            PrimitiveBindGroup->BindResourceView(
                ABYTEK_NAME("DepthTexture"),
                TempDepthTexture
            );
            {
                F_PrimitiveVisibilityConfig VisibilityConfig;
                VisibilityConfig.PrimitiveProcessorId = Processor->GetId();
                PrimitiveBindGroup->BindUniformData(ABYTEK_NAME("PrimitiveVisibilityConfig"), VisibilityConfig);
            }
            InstancedMeshletBuffer.Bind(PrimitiveBindGroup, ABYTEK_NAME("InstancedMeshlets"));
            CastedView->GetOpaqueVisibilityBuffer().Bind(
                PrimitiveBindGroup,
                ABYTEK_NAME("OpaqueVisibilityBuffer")
            );
            PrimitiveBindGroup->Commit();
            
            F_RHIBindGroupSet BindGroups = { 
                ViewUniformBindGroup,
                PrimitiveBindGroup,
                GeometryGlobalSRVBindGroup,
                RenderPrimitive::F_Component_Transform::GetBindGroup<RenderPrimitive::F_Data_Simple>(ProcessorGPUData),
                RenderPrimitive::F_Component_InverseTransposeTransform::GetBindGroup<RenderPrimitive::F_Data_Simple>(ProcessorGPUData),
                RenderPrimitive::F_Component_GeometryAddress_ECMS::GetBindGroup<RenderPrimitive::F_Data_Simple>(ProcessorGPUData)
            };

            H_RHISubmissionUtilities::DispatchCompute(
                SubmissionItemContainer,
                Pipeline.AcquirePipelineState(),
                BindGroups,
                F_Vector3_U32(
                    RoundUpDivide(Resolution.X, NUM_THREADS.X), 
                    RoundUpDivide(Resolution.Y, NUM_THREADS.Y), 
                    1
                )
            );
            
            H_RenderCoreExtensions::MergeDepthTexture(
                SubmissionItemContainer,
                WorldRenderResource->GetRenderRegistryRuntime(),
                View->GetDSV()->GetResource(),
                TempDepthTexture
            );
        }
    }
}
