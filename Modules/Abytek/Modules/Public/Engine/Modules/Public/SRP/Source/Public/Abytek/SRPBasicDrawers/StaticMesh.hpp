#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"
#include "Abytek/Assets/Render/StaticMeshRenderProxy.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryStorage.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/ECMS/SRPInstancedMeshletBuffer_ECMS.hpp"
#include "Abytek/SRPVisibilityBuffer.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveProcessor.hpp"


namespace Abytek
{
    class A_RenderView;
    class F_StaticMeshRenderProxy;

    namespace SRPBasicDrawers
    {
        ABYTEK_DEFINE_PERMUTATION(
            F_StaticMeshPermuation_DataType, 
            TF_Permutation_Set<
                E_StaticMeshDataType::SIMPLE,
                E_StaticMeshDataType::ECMS
            >
        )
        
        enum class E_StaticMeshOutputMode : U8
        {
            RTV_DSV,
            VISIBILITY_BUFFER,
            DEFAULT = RTV_DSV
        };
        ABYTEK_DEFINE_PERMUTATION(
            F_StaticMeshPermuation_OutputMode, 
            TF_Permutation_Set<
                E_StaticMeshOutputMode::RTV_DSV,
                E_StaticMeshOutputMode::VISIBILITY_BUFFER
            >
        );
        
        struct ABYTEK_ALIGN(16) F_StaticMeshVisibilityConfig
        {
            F_GPUDataInstanceAddress PrimitiveAddress;
            F_RenderPrimitiveProcessorId PrimitiveProcessorId = INVALID_RENDER_PRIMITIVE_PROCESSOR_ID;
        };
        struct ABYTEK_ALIGN(16) F_StaticMeshUniformData
        {
            F_Matrix4x4_F32 TransformMatrix;
            F_Matrix4x4_F32 InverseTransposeTransformMatrix;
            F_Vector4_F32 Color = { 0.0f, 1.0f, 1.0f, 1.0f };
        };
        struct ABYTEK_ENGINE_SRP_API F_StaticMeshBinding : F_GlobalRenderBinding
        {
            ABYTEK_OVERRIDE_PERMUTATION_DOMAIN(F_StaticMeshPermuation_DataType, F_StaticMeshPermuation_OutputMode)
            
            ABYTEK_GLOBAL_RENDER_BINDING(F_StaticMeshBinding, ABYTEK_NAME("Abytek::SRPBasicDrawers::F_StaticMeshBinding"));
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                auto DataType = Config.PermutationVector.Get<F_StaticMeshPermuation_DataType>();
                auto OutputMode = Config.PermutationVector.Get<F_StaticMeshPermuation_OutputMode>();
                
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeUniformData<F_StaticMeshUniformData>(
                        ABYTEK_NAME("StaticMeshUniformData")
                    ) 
                );
                if (DataType == E_StaticMeshDataType::SIMPLE)
                {
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeUniformData<F_StaticMeshGeometryUniformData_Simple>(
                            ABYTEK_NAME("StaticMeshGeometryUniformData_Simple")
                        ) 
                    );
                }
                if (DataType == E_StaticMeshDataType::ECMS)
                {
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeUniformData<F_StaticMeshGeometryUniformData_ECMS>(
                            ABYTEK_NAME("StaticMeshGeometryUniformData_ECMS")
                        ) 
                    );
                }
                
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    SRP::ECMS::F_InstancedMeshletBuffer::AddToBindGroup(
                        Config,
                        ABYTEK_NAME("InstancedMeshlets"),
                        true
                    )
                );
                
                switch (OutputMode)
                {
                case E_StaticMeshOutputMode::RTV_DSV:
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
                    }
                    break;
                case E_StaticMeshOutputMode::VISIBILITY_BUFFER:
                    {
                        ABYTEK_FEEDBACK_STATUS_CHECK(
                            SRP::VisibilityBuffer::AddToBindGroup_Opaque(
                                Config, 
                                ABYTEK_NAME("OpaqueVisibilityBuffer"),
                                true
                            )
                        );
                        Config.Slots.push_back(
                            F_RHIBindGroupTemplateSlot::MakeUniformData<F_StaticMeshVisibilityConfig>(
                                ABYTEK_NAME("StaticMeshVisibilityConfig")
                            ) 
                        );
                    }
                    break;
                default:
                    ABYTEK_LOG_FATAL() << "Unknown output mode: " << static_cast<U32>(OutputMode);
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
            ABYTEK_OVERRIDE_PERMUTATION_DOMAIN(F_FillMode, F_StaticMeshPermuation_DataType, F_StaticMeshPermuation_OutputMode);
            
            ABYTEK_GLOBAL_RENDER_PIPELINE(F_StaticMeshPipeline, ABYTEK_NAME("Abytek::SRPBasicDrawers::F_StaticMeshPipeline"));
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                auto DataType = Config.PermutationVector.Get<F_StaticMeshPermuation_DataType>();
                auto OutputMode = Config.PermutationVector.Get<F_StaticMeshPermuation_OutputMode>();
                
                Config.Type = E_RHIPipelineStateType::GRAPHICS;
                Config.Rasterizer.FillMode = Config.PermutationVector.Get<F_FillMode>();
                Config.Rasterizer.CullMode = E_RHICullMode::NONE;
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        F_RenderViewUniformDataBinding::GetTemplateHashCode()
                    )
                );
                {
                    F_StaticMeshBinding::F_DynamicPermutationVector StaticMeshBindingPermutationVector;
                    StaticMeshBindingPermutationVector.Get<F_StaticMeshPermuation_DataType>() = DataType;
                    StaticMeshBindingPermutationVector.Get<F_StaticMeshPermuation_OutputMode>() = OutputMode;
                    Config.BindGroups.push_back(
                        F_RHIPipelineStateTemplateBindGroup::Make(
                            F_StaticMeshBinding::GetTemplateHashCode(StaticMeshBindingPermutationVector)
                        )
                    );
                }
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        RenderGeometry::F_GlobalSRVBinding::GetTemplateHashCode()
                    )
                );
                
                switch (DataType)
                {
                case E_StaticMeshDataType::SIMPLE:
                    {
                        Config.VertexShader = ABYTEK_GLOBAL_SHADER("MainVS", "Abytek/SRPBasicDrawers/StaticMeshVS_Simple", E_RHIShaderFrequency::VERTEX);
                        Config.PixelShader = ABYTEK_GLOBAL_SHADER("MainPS", "Abytek/SRPBasicDrawers/StaticMeshPS", E_RHIShaderFrequency::PIXEL);
                        Config.AddShaderDefinition(ABYTEK_NAME("ABYTEK_STATIC_MESH_DATA_TYPE_SIMPLE"));
                    }
                    break;
                case E_StaticMeshDataType::ECMS:
                    {
                        Config.MeshShader = ABYTEK_GLOBAL_SHADER("MainMS", "Abytek/SRPBasicDrawers/StaticMeshMS_ECMS", E_RHIShaderFrequency::MESH);
                        Config.PixelShader = ABYTEK_GLOBAL_SHADER("MainPS", "Abytek/SRPBasicDrawers/StaticMeshPS", E_RHIShaderFrequency::PIXEL);
                        Config.AddShaderDefinition(ABYTEK_NAME("ABYTEK_SRP_ECMS"));
                        Config.AddShaderDefinition(ABYTEK_NAME("ABYTEK_ECMS_MAX_VERTICES_PER_MESHLET"), ToText(ECMS_MAX_VERTICES_PER_MESHLET));
                        Config.AddShaderDefinition(ABYTEK_NAME("ABYTEK_ECMS_MAX_INDICES_PER_MESHLET"), ToText(ECMS_MAX_INDICES_PER_MESHLET));
                        Config.AddShaderDefinition(ABYTEK_NAME("ABYTEK_ECMS_MAX_PRIMITIVES_PER_MESHLET"), ToText(ECMS_MAX_PRIMITIVES_PER_MESHLET));
                        Config.AddShaderDefinition(
                            ABYTEK_NAME("NUM_THREADS"), 
                            ToText(
                                Max<U32>(ECMS_MAX_VERTICES_PER_MESHLET, ECMS_MAX_INDICES_PER_MESHLET / 3)    
                            )
                        );
                    }
                    break;
                default:
                    ABYTEK_LOG_FATAL() << "Unknown mesh data type: " << static_cast<U32>(DataType);
                }

                switch (OutputMode)
                {
                case E_StaticMeshOutputMode::RTV_DSV:
                    Config.DepthStencil.EnableDepthTest = true;
                    Config.AddShaderDefinition(ABYTEK_NAME("OUTPUT_RTV_DSV"));
                    break;
                case E_StaticMeshOutputMode::VISIBILITY_BUFFER:
                    Config.DepthStencil.EnableDepthTest = false;
                    Config.AddShaderDefinition(ABYTEK_NAME("OUTPUT_OPAQUE_VISIBILITY_BUFFER"));
                    break;
                default:
                    ABYTEK_LOG_FATAL() << "Unknown output mode: " << static_cast<U32>(OutputMode);
                }
                
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    RenderGeometry::SetupCompileParams(
                        Config
                    )
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
    }
    
    struct ABYTEK_ENGINE_SRP_API H_SRPStaticMeshDrawer
    {
        struct F_AdvancedParams
        {
            SRPBasicDrawers::E_StaticMeshOutputMode OutputMode = SRPBasicDrawers::E_StaticMeshOutputMode::DEFAULT;
            F_RenderPrimitiveProcessorId PrimitiveProcessorId = INVALID_RENDER_PRIMITIVE_PROCESSOR_ID;
        };
        static void RenderAdvanced(
            const F_AdvancedParams& AdvancedParams,
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
            const TW_Valid<A_RenderView>& View, 
            const TW_Valid<F_StaticMeshRenderProxy> StaticMeshRenderProxy,
            const F_Matrix4x4_F32& StaticMeshTransformMatrix, 
            const F_Vector4_F32& StaticMeshColor = { 0.0f, 1.0f, 1.0f, 1.0f }, 
            E_RHIFillMode FillMode = E_RHIFillMode::DEFAULT
        );
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
