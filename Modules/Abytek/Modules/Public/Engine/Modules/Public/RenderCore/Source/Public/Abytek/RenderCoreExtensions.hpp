#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"


namespace Abytek
{
    class A_RHISubmissionItemContainer;
    
    namespace RenderCoreExtensions
    {
        namespace CopyTexture2D_R32_SRVToUAV
        {
            struct F_UniformData
            {
                F_Vector2_U32 Resolution = F_Vector2_U32::Zero();
            };
            
            struct F_Binding : F_GlobalRenderBinding
            {
                ABYTEK_GLOBAL_RENDER_BINDING(F_Binding, ABYTEK_NAME("Abytek::RenderCoreExtensions::CopyTexture2D_R32_SRVToUAV::F_Binding"));
                
                static F_FeedbackStatus Build(F_Config& Config)
                {
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeUniformData<F_UniformData>(
                            ABYTEK_NAME("UniformData")
                        )
                    );
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeResourceView(
                            ABYTEK_NAME("SRV"),
                            F_RHIResourceAccess::MakeSRV()
                        )
                    );
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeResourceView(
                            ABYTEK_NAME("UAV"),
                            F_RHIResourceAccess::MakeUAV()
                        )
                    );
                    return F_FeedbackStatus::MakeSucceeded();
                }
            };
            
            struct F_Pipeline : F_GlobalRenderPipeline
            {
                ABYTEK_GLOBAL_RENDER_PIPELINE(F_Pipeline, ABYTEK_NAME("Abytek::RenderCoreExtensions::CopyTexture2D_R32_SRVToUAV::F_Pipeline"));
                
                static F_FeedbackStatus Build(F_Config& Config)
                {
                    Config.Type = E_RHIPipelineStateType::COMPUTE;
                    Config.ComputeShader = ABYTEK_GLOBAL_SHADER("MainCS", "Abytek/RenderCoreExtensions/CopyTexture2D_R32Float_SRVToUAV_CS", E_RHIShaderFrequency::COMPUTE);
                    Config.BindGroups.push_back(
                        F_RHIPipelineStateTemplateBindGroup::Make(
                            F_Binding::GetTemplateHashCode()     
                        )
                    );
                    return F_FeedbackStatus::MakeSucceeded();
                }
            };
        }
        
        namespace CopyTexture2D_R32_SRVToDSV
        {
            struct F_UniformData
            {
                F_Vector2_U32 Resolution = F_Vector2_U32::Zero();
            };
            
            struct F_Binding : F_GlobalRenderBinding
            {
                ABYTEK_GLOBAL_RENDER_BINDING(F_Binding, ABYTEK_NAME("Abytek::RenderCoreExtensions::CopyTexture2D_R32_SRVToDSV::F_Binding"));
                
                static F_FeedbackStatus Build(F_Config& Config)
                {
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeUniformData<F_UniformData>(
                            ABYTEK_NAME("UniformData")
                        )
                    );
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeResourceView(
                            ABYTEK_NAME("SRV"),
                            F_RHIResourceAccess::MakeSRV()
                        )
                    );
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeDSV(
                            ABYTEK_NAME("DSV"),
                            E_RHIFormat::D32_FLOAT
                        )
                    );
                    return F_FeedbackStatus::MakeSucceeded();
                }
            };
            
            struct F_Pipeline : F_GlobalRenderPipeline
            {
                ABYTEK_GLOBAL_RENDER_PIPELINE(F_Pipeline, ABYTEK_NAME("Abytek::RenderCoreExtensions::CopyTexture2D_R32_SRVToDSV::F_Pipeline"));
                
                static F_FeedbackStatus Build(F_Config& Config)
                {
                    Config.Type = E_RHIPipelineStateType::GRAPHICS;
                    Config.VertexShader = ABYTEK_GLOBAL_SHADER("MainVS", "Abytek/RenderCoreExtensions/CopyTexture2D_R32Float_SRVToDSV_VS", E_RHIShaderFrequency::VERTEX);
                    Config.PixelShader = ABYTEK_GLOBAL_SHADER("MainPS", "Abytek/RenderCoreExtensions/CopyTexture2D_R32Float_SRVToDSV_PS", E_RHIShaderFrequency::PIXEL);
                    
                    Config.Rasterizer.FillMode = E_RHIFillMode::SOLID;
                    Config.Rasterizer.CullMode = E_RHICullMode::NONE;
                    Config.DepthStencil.EnableDepthTest = true;
                    Config.BindGroups.push_back(
                        F_RHIPipelineStateTemplateBindGroup::Make(
                            F_Binding::GetTemplateHashCode()     
                        )
                    );
                    return F_FeedbackStatus::MakeSucceeded();
                }
            };
        }
    }
    
    struct ABYTEK_ENGINE_RENDER_CORE_API H_RenderCoreExtensions
    {
        static void SafeCopyTexture2D(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime, 
            const TS<A_RHIResource>& DstTexture,
            const TS<A_RHIResource>& SrcTexture,
            const F_DebugName& DebugName = {}
        );
        static void MergeDepthTexture(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime, 
            const TS<A_RHIResource>& DstTexture,
            const TS<A_RHIResource>& SrcTexture,
            const F_DebugName& DebugName = {}
        );
    };
}