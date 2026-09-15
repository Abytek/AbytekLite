#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"
#include "Abytek/RenderBase/RenderView.hpp"


namespace Abytek
{
    class A_RenderView;

    namespace SRPBasicDrawers
    {
        struct ABYTEK_ALIGN(16) F_CubeUniformData
        {
            F_Matrix4x4_F32 TransformMatrix;
            F_Vector4_F32 Color = { 0.0f, 1.0f, 1.0f, 1.0f };
        };
        struct ABYTEK_ENGINE_SRP_API F_CubeBinding : TF_GlobalRenderBinding<F_CubeBinding>
        {
            ABYTEK_DECLARE_GLOBAL_RENDER_BINDING(F_CubeBinding);
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeRTV(
                        ABYTEK_NAME("RTV"),
                        E_RHIFormat::R8G8B8A8_UNORM
                    ) 
                );
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeUniformData<F_CubeUniformData>(
                        ABYTEK_NAME("CubeUniformData")
                    ) 
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct ABYTEK_ENGINE_SRP_API F_CubePipeline : TF_GlobalRenderPipeline<F_CubePipeline>
        {
            ABYTEK_DEFINE_PERMUTATION(
                F_FillMode, 
                TF_Permutation_Set<
                    E_RHIFillMode::SOLID,
                    E_RHIFillMode::WIREFRAME
                >
            );
            ABYTEK_OVERRIDE_PERMUTATION_DOMAIN(F_FillMode);
            
            ABYTEK_DECLARE_GLOBAL_RENDER_PIPELINE(F_CubePipeline);
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Type = E_RHIPipelineStateType::GRAPHICS;
                Config.Rasterizer.FillMode = Config.PermutationVector.Get<F_FillMode>();
                Config.Rasterizer.CullMode = E_RHICullMode::NONE;
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        F_RenderViewUniformDataBinding::GetTemplateHashCode()
                    )
                );
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        F_CubeBinding::GetTemplateHashCode()
                    )
                );
                Config.VertexShader = ABYTEK_GLOBAL_SHADER("MainVS", "SRP/BasicDrawers/CubeVS", E_RHIShaderFrequency::VERTEX);
                Config.PixelShader = ABYTEK_GLOBAL_SHADER("MainPS", "SRP/BasicDrawers/CubePS", E_RHIShaderFrequency::PIXEL);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
    }
    
    struct ABYTEK_ENGINE_SRP_API H_SRPCubeDrawer
    {
        static void Render(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TW_Valid<A_RenderView>& View, 
            const F_Matrix4x4_F32& CubeTransformMatrix, 
            const F_Vector4_F32& CubeColor = { 0.0f, 1.0f, 1.0f, 1.0f }, 
            E_RHIFillMode FillMode = E_RHIFillMode::DEFAULT
        );
    };
}
