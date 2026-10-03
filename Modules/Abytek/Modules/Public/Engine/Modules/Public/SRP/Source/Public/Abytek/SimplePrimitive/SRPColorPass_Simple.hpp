#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/SimplePrimitive/SRPRenderPrimitiveSet_Simple.hpp"
#include "Abytek/SimplePrimitive/SRPCommon_Simple.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"
#include "Abytek/RHISubmissionItemContainer.hpp"
#include "Abytek/Renderer/GPUData/GPUDataCommon.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveProcessor.hpp"
#include "Abytek/SRPBasicDrawers/StaticMesh.hpp"


namespace Abytek
{
    class A_RenderView;
    
    namespace SRP::SimplePrimitive::ColorPass
    {
        static inline F_Vector3_U32 NUM_THREADS = F_Vector3_U32(8, 8, 1);
        
        struct F_Binding : F_GlobalRenderBinding
        {
            ABYTEK_GLOBAL_RENDER_BINDING(F_Binding, ABYTEK_NAME("Abytek::SRP::SimplePrimitive::ColorPass::F_Binding"));
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeResourceView(
                        ABYTEK_NAME("ColorTexture"),
                        F_RHIResourceAccess::MakeUAV()
                    ) 
                );
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeResourceView(
                        ABYTEK_NAME("DepthTexture"),
                        F_RHIResourceAccess::MakeUAV()
                    ) 
                );
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeUniformData<F_PrimitiveVisibilityConfig>(
                        ABYTEK_NAME("PrimitiveVisibilityConfig")
                    ) 
                );
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    SRP::VisibilityBuffer::AddToBindGroup_Opaque(
                        Config, 
                        ABYTEK_NAME("OpaqueVisibilityBuffer")
                    )
                );
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    SRP::ECMS::F_InstancedMeshletBuffer::AddToBindGroup(
                        Config,
                        ABYTEK_NAME("InstancedMeshlets")
                    )
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        
        struct F_Pipeline : F_GlobalRenderPipeline
        {
            ABYTEK_GLOBAL_RENDER_PIPELINE(F_Pipeline, ABYTEK_NAME("Abytek::SRP::SimplePrimitive::ColorPass::F_Pipeline"));
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Type = E_RHIPipelineStateType::COMPUTE;
                Config.ComputeShader = ABYTEK_GLOBAL_SHADER("MainCS", "Abytek/SimplePrimitive/SRPColorPassCS_Simple", E_RHIShaderFrequency::COMPUTE);
                Config.AddShaderDefinition(
                    ABYTEK_NAME("NUM_THREADS_X"), 
                    ToText(NUM_THREADS.X)
                );
                Config.AddShaderDefinition(
                    ABYTEK_NAME("NUM_THREADS_Y"), 
                    ToText(NUM_THREADS.Y)
                );
                Config.AddShaderDefinition(
                    ABYTEK_NAME("NUM_THREADS_Z"), 
                    ToText(NUM_THREADS.Z)
                );
                
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        F_RenderViewUniformDataBinding::GetTemplateHashCode()
                    )
                );
                {
                    F_Binding::F_DynamicPermutationVector BindingPermutationVector;
                    Config.BindGroups.push_back(
                        F_RHIPipelineStateTemplateBindGroup::Make(
                            F_Binding::GetTemplateHashCode(BindingPermutationVector)
                        )
                    );
                }
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        RenderGeometry::F_GlobalSRVBinding::GetTemplateHashCode()
                    )
                );
                
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    RenderPrimitive::F_Component_Transform::AddBindGroupToPipelineState<RenderPrimitive::F_Data_Simple>(Config)
                );
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    RenderPrimitive::F_Component_InverseTransposeTransform::AddBindGroupToPipelineState<RenderPrimitive::F_Data_Simple>(Config)
                );
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    RenderPrimitive::F_Component_GeometryAddress_ECMS::AddBindGroupToPipelineState<RenderPrimitive::F_Data_Simple>(Config)
                );
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    RenderPrimitive::F_Component_GeometryAllocationStructure_ECMS::AddBindGroupToPipelineState<RenderPrimitive::F_Data_Simple>(Config)
                );
                
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    RenderGeometry::SetupCompileParams(
                        Config
                    )
                );
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    GPUData::SetupCompileParams(
                        Config
                    )
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        
        ABYTEK_ENGINE_SRP_API void Invoke(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,   
            const TW_Valid<A_RenderView>& View
        );
    }
}
