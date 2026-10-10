#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/SimplePrimitive/SRPRenderPrimitiveSet.hpp"
#include "Abytek/Renderer/SimplePrimitive/SRPCommon.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"
#include "Abytek/RHISubmissionItemContainer.hpp"
#include "Abytek/Renderer/GPUData/GPUDataCommon.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveProcessor.hpp"
#include "Abytek/Renderer/RenderPrimitive/Archetypes/Data_Simple.hpp"
#include "Abytek/Renderer/SRPBasicDrawers/StaticMesh.hpp"


namespace Abytek
{
    class A_RenderView;
    
    namespace SRP::SimplePrimitive::VisibilityBufferPass
    {
        struct F_Binding : F_GlobalRenderBinding
        {
            ABYTEK_GLOBAL_RENDER_BINDING(F_Binding, ABYTEK_NAME("Abytek::SRP::SimplePrimitive::VisibilityBufferPass::F_Binding"));
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeUniformData<F_PrimitiveVisibilityConfig>(
                        ABYTEK_NAME("PrimitiveVisibilityConfig")
                    ) 
                );
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    SRP::ECMS::F_InstancedMeshletBuffer::AddToBindGroup(
                        Config,
                        ABYTEK_NAME("InstancedMeshlets"),
                        true
                    )
                );
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    SRP::VisibilityBuffer::AddToBindGroup_Opaque(
                        Config, 
                        ABYTEK_NAME("OpaqueVisibilityBuffer"),
                        true
                    )
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        
        struct F_Pipeline : F_GlobalRenderPipeline
        {
            ABYTEK_GLOBAL_RENDER_PIPELINE(F_Pipeline, ABYTEK_NAME("Abytek::SRP::SimplePrimitive::VisibilityBufferPass::F_Pipeline"));
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Type = E_RHIPipelineStateType::GRAPHICS;
                Config.MeshShader = ABYTEK_GLOBAL_SHADER("MainMS", "Abytek/Renderer/SimplePrimitive/SRPVisibilityBufferPassMS", E_RHIShaderFrequency::MESH);
                Config.PixelShader = ABYTEK_GLOBAL_SHADER("MainPS", "Abytek/Renderer/SimplePrimitive/SRPVisibilityBufferPassPS", E_RHIShaderFrequency::PIXEL);
                Config.AddShaderDefinition(ABYTEK_NAME("ABYTEK_ECMS_MAX_VERTICES_PER_MESHLET"), ToText(ECMS_MAX_VERTICES_PER_MESHLET));
                Config.AddShaderDefinition(ABYTEK_NAME("ABYTEK_ECMS_MAX_INDICES_PER_MESHLET"), ToText(ECMS_MAX_INDICES_PER_MESHLET));
                Config.AddShaderDefinition(ABYTEK_NAME("ABYTEK_ECMS_MAX_PRIMITIVES_PER_MESHLET"), ToText(ECMS_MAX_PRIMITIVES_PER_MESHLET));
                Config.AddShaderDefinition(
                    ABYTEK_NAME("NUM_THREADS"), 
                    ToText(
                        Max<U32>(ECMS_MAX_VERTICES_PER_MESHLET, ECMS_MAX_INDICES_PER_MESHLET / 3)    
                    )
                );
                
                Config.Rasterizer.FillMode = E_RHIFillMode::SOLID;
                Config.Rasterizer.CullMode = E_RHICullMode::NONE;
                Config.DepthStencil.EnableDepthTest = false;
                
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
                    RenderPrimitive::F_Data_Simple::AddInstanceSetHeaderBindGroupToPipelineState(Config)
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
