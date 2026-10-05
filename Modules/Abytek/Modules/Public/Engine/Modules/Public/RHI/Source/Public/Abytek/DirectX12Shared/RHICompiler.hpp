#pragma once

#include "Abytek/RHICompiler.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"
#include "Abytek/DirectX12Shared/RHIBindGroupTemplate.hpp"
#include "Abytek/DirectX12Shared/RHIPipelineStateTemplate.hpp"
#include "Abytek/DirectX12Shared/RHIRootSignatureTemplate.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12SharedRHICompiler : public A_RHICompiler
    {
    private:

    public:
        
    public:
        F_DirectX12SharedRHICompiler();
        ~F_DirectX12SharedRHICompiler() override;

    public:
        F_String D3DParseProfile(
            const F_RHIFeatureSupports& FeatureSupports,
            E_RHIShaderFrequency ShaderFrequency
        );

    public:
        F_RHICommonCompilationStatus D3DCompileBindGroupTemplate(
            const F_RHIBindGroupTemplateCompileParams& CompileParams,
            TS<A_RHIBindGroupTemplate>& OutCompiledObject
        );
        F_RHICommonCompilationStatus D3DCompilePipelineStateTemplate(
            const F_RHIPipelineStateTemplateCompileParams& CompileParams,
            TS<A_RHIPipelineStateTemplate>& OutCompiledObject
        );
        F_RHICommonCompilationStatus D3DCompileRootSignatureTemplate(
            const F_DirectX12SharedRHIRootSignatureTemplateCompileParams& CompileParams,
            TS<F_DirectX12SharedRHIRootSignatureTemplate>& OutCompiledObject
        );

    public:
        F_RHICommonCompilationStatus ValidateBindGroupTemplate(
            const F_RHIBindGroupTemplateCompileParams& CompileParams
        ) override;
        F_RHICommonCompilationStatus ValidatePipelineStateTemplate(
            const F_RHIPipelineStateTemplateCompileParams& CompileParams
        ) override;
        F_RHICommonCompilationStatus ValidateRootSignatureTemplate(
            const F_DirectX12SharedRHIRootSignatureTemplateCompileParams& CompileParams
        );

    public:
        F_RHICommonCompilationStatus CompileBindGroupTemplate(
            const F_RHIBindGroupTemplateCompileParams& CompileParams,
            TS<A_RHIBindGroupTemplate>& OutCompiledObject
        ) override;
        F_RHICommonCompilationStatus CompilePipelineStateTemplate(
            const F_RHIPipelineStateTemplateCompileParams& CompileParams,
            TS<A_RHIPipelineStateTemplate>& OutCompiledObject
        ) override;
        F_RHICommonCompilationStatus CompileRootSignatureTemplate(
            const F_DirectX12SharedRHIRootSignatureTemplateCompileParams& CompileParams,
            TS<F_DirectX12SharedRHIRootSignatureTemplate>& OutCompiledObject
        );
    };
}
#endif