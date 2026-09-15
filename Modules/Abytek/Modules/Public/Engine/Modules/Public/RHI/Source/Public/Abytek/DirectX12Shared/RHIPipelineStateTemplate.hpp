#pragma once

#include "Abytek/RHIPipelineStateTemplate.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"
#include "Abytek/DirectX12Shared/RHIRootSignatureTemplate.hpp"


namespace Abytek
{
    class F_DirectX12SharedRHIRootSignatureTemplate;
    
    struct F_DirectX12SharedRHIPipelineStateTemplateCompiledData
    {
        // For root signature creation
        TF_SmallVector<DirectX12SharedAPIWrapper::F_RootParameter, 4> RootParameters;
        TF_SmallVector<DirectX12SharedAPIWrapper::F_InputElementDesc, 4> InputElementDescs;
        B8 HasIndexBuffer = false;
        B8 AllowBindless = false;
        
        // RTV + DSV
        TF_SmallVector<E_RHIFormat, 8> RTVFormats;
        TF_Optional<E_RHIFormat> DSVFormat;

        // For compiled shader binaries,...
        TF_Optional<F_RHIPipelineStateTemplateCompiledVertexShader> VertexShader;
        TF_Optional<F_RHIPipelineStateTemplateCompiledPixelShader> PixelShader;
        TF_Optional<F_RHIPipelineStateTemplateCompiledMeshShader> MeshShader;
        TF_Optional<F_RHIPipelineStateTemplateCompiledAmplificationShader> AmplificationShader;
        TF_Optional<F_RHIPipelineStateTemplateCompiledComputeShader> ComputeShader;

        // For bindgroup-related mapping
        struct F_BindGroupMap
        {
            U32 RootParameterIndex = ~U32(0);
            U32 NumRootParameters = 0;

            B8 HasInputElementDescs = false;
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_BindGroupMap& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RootParameterIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.NumRootParameters);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.HasInputElementDescs);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_BindGroupMap& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RootParameterIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.NumRootParameters);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.HasInputElementDescs);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        TF_Vector<F_BindGroupMap> BindGroupMaps;

        F_RHITemplateHashCode RootSignatureTemplateHashCode = INVALID_RHI_TEMPLATE_HASH_CODE;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_DirectX12SharedRHIPipelineStateTemplateCompiledData& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RootParameters);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.InputElementDescs);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.HasIndexBuffer);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.AllowBindless);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RTVFormats);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DSVFormat);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.VertexShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PixelShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MeshShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.AmplificationShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ComputeShader);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.BindGroupMaps);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RootSignatureTemplateHashCode);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_DirectX12SharedRHIPipelineStateTemplateCompiledData& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RootParameters);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.InputElementDescs);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.HasIndexBuffer);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.AllowBindless);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RTVFormats);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DSVFormat);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.VertexShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PixelShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.MeshShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.AmplificationShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ComputeShader);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.BindGroupMaps);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RootSignatureTemplateHashCode);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };

    class ABYTEK_ENGINE_RHI_API F_DirectX12SharedRHIPipelineStateTemplate : public A_RHIPipelineStateTemplate
    {
    private:
        F_DirectX12SharedRHIPipelineStateTemplateCompiledData _CompiledData;
        TS<F_DirectX12SharedRHIRootSignatureTemplate> _RootSignatureTemplate;

    public:
        ABYTEK_FORCE_INLINE const auto& GetCompiledData() const noexcept
        {
            return _CompiledData;
        }
        ABYTEK_FORCE_INLINE auto& InjectCompiledData() noexcept
        {
            return _CompiledData;
        }
        ABYTEK_FORCE_INLINE const auto& GetRootSignatureTemplate() const noexcept
        {
            return _RootSignatureTemplate;
        }
        
    public:
        F_DirectX12SharedRHIPipelineStateTemplate(
            const F_RHIPipelineStateTemplateBuildParams& BuildParams,
            F_DirectX12SharedRHIPipelineStateTemplateCompiledData&& CompiledData    
        );
        ABYTEK_FORCE_INLINE F_DirectX12SharedRHIPipelineStateTemplate(
            const F_RHIPipelineStateTemplateBuildParams& BuildParams,
            const F_DirectX12SharedRHIPipelineStateTemplateCompiledData& CompiledData
        ) :
            F_DirectX12SharedRHIPipelineStateTemplate(
                BuildParams,
                F_DirectX12SharedRHIPipelineStateTemplateCompiledData(CompiledData)
            )
        {
        }
        ~F_DirectX12SharedRHIPipelineStateTemplate() override;
    };
}
