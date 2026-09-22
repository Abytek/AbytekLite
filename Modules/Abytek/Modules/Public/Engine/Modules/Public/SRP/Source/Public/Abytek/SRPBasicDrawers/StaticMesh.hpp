#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"
#include "Abytek/Assets/Render/StaticMeshRenderProxy.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryStorage.hpp"
#include "Abytek/Renderer/RenderView.hpp"


namespace Abytek
{
    class A_RenderView;
    class F_StaticMeshRenderProxy;

    namespace SRPBasicDrawers
    {
        ABYTEK_DEFINE_PERMUTATION(
            F_StaticMeshPermuation_DataType, 
            TF_Permutation_Set<
                E_StaticMeshDataType::SIMPLE
            >
        )
        
        struct ABYTEK_ALIGN(16) F_StaticMeshUniformData
        {
            F_Matrix4x4_F32 TransformMatrix;
            F_Matrix4x4_F32 InverseTransposeTransformMatrix;
            F_Vector4_F32 Color = { 0.0f, 1.0f, 1.0f, 1.0f };
        };
        struct ABYTEK_ENGINE_SRP_API F_StaticMeshBinding : F_GlobalRenderBinding
        {
            ABYTEK_OVERRIDE_PERMUTATION_DOMAIN(F_StaticMeshPermuation_DataType)
            
            ABYTEK_GLOBAL_RENDER_BINDING(F_StaticMeshBinding, ABYTEK_NAME("Abytek::SRPBasicDrawers::F_StaticMeshBinding"));
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeRTV(
                        ABYTEK_NAME("RTV"),
                        E_RHIFormat::R8G8B8A8_UNORM
                    ) 
                );
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeDSV(
                        ABYTEK_NAME("DSV"),
                        E_RHIFormat::D32_FLOAT
                    ) 
                );
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeUniformData<F_StaticMeshUniformData>(
                        ABYTEK_NAME("StaticMeshUniformData")
                    ) 
                );
                if (Config.PermutationVector.Get<F_StaticMeshPermuation_DataType>() == E_StaticMeshDataType::SIMPLE)
                {
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeUniformData<F_StaticMeshGeometryUniformData_Simple>(
                            ABYTEK_NAME("StaticMeshGeometryUniformData_Simple")
                        ) 
                    );
                }
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct ABYTEK_ENGINE_SRP_API F_StaticMeshPipeline : F_GlobalRenderPipeline
        {
            ABYTEK_DEFINE_PERMUTATION(
                F_FillMode, 
                TF_Permutation_Set<
                    E_RHIFillMode::SOLID,
                    E_RHIFillMode::WIREFRAME
                >
            );
            ABYTEK_OVERRIDE_PERMUTATION_DOMAIN(F_FillMode, F_StaticMeshPermuation_DataType);
            
            ABYTEK_GLOBAL_RENDER_PIPELINE(F_StaticMeshPipeline, ABYTEK_NAME("Abytek::SRPBasicDrawers::F_StaticMeshPipeline"));
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Type = E_RHIPipelineStateType::GRAPHICS;
                Config.Rasterizer.FillMode = Config.PermutationVector.Get<F_FillMode>();
                Config.Rasterizer.CullMode = E_RHICullMode::NONE;
                Config.DepthStencil.EnableDepthTest = true;
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        F_RenderViewUniformDataBinding::GetTemplateHashCode()
                    )
                );
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        F_StaticMeshBinding::GetTemplateHashCode()
                    )
                );
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        RenderGeometry::F_GlobalSRVBinding::GetTemplateHashCode()
                    )
                );
                Config.VertexShader = ABYTEK_GLOBAL_SHADER("MainVS", "SRP/BasicDrawers/StaticMeshVS", E_RHIShaderFrequency::VERTEX);
                Config.PixelShader = ABYTEK_GLOBAL_SHADER("MainPS", "SRP/BasicDrawers/StaticMeshPS", E_RHIShaderFrequency::PIXEL);
                RenderGeometry::SetupCompileParams(
                    Config
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
    }
    
    struct ABYTEK_ENGINE_SRP_API H_SRPStaticMeshDrawer
    {
        static void Render(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
            const TW_Valid<A_RenderView>& View, 
            const TW_Valid<F_StaticMeshRenderProxy> StaticMeshRenderProxy,
            const F_Matrix4x4_F32& StaticMeshTransformMatrix, 
            const F_Vector4_F32& StaticMeshColor = { 0.0f, 1.0f, 1.0f, 1.0f }, 
            E_RHIFillMode FillMode = E_RHIFillMode::DEFAULT
        );
    };
}
