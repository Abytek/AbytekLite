#include "Abytek/DirectX12/RHIPipelineStateProxy.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntimeProxy.hpp"
#include "Abytek/DirectX12/RHIPipelineStateTemplateRuntimeProxy.hpp"
#include "Abytek/DirectX12/RHIContextProxy.hpp"
#include "Abytek/DirectX12/RHIRootSignatureTemplateRuntimeProxy.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/DirectX12/RHIFormat.hpp"
#include "Abytek/DirectX12Shared/RHIPipelineStateTemplate.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    /*void F_DirectX12RHIPipelineStateProxy::LateBuildPipelineState_DirectX12()
    {
    }
    void F_DirectX12RHIPipelineStateProxy::Release()
    {
        _D3D12PipelineState = nullptr;
        A_RHIPipelineStateProxy::Release();
    }

    void F_DirectX12RHIPipelineStateProxy::InitD3D12PipelineState()
    {
        switch (
            GetTemplateRuntimeProxy()
            ->GetTemplate()
            .FastCast<A_RHIPipelineStateTemplate>()
            ->GetType()
        )
        {
        case E_RHIPipelineStateType::GRAPHICS:
            _InitGraphics();
            break;
        case E_RHIPipelineStateType::COMPUTE:
            _InitCompute();
            break;
        case E_RHIPipelineStateType::WORK_GRAPH:
            _InitWorkGraph();
            break;
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown pipeline state type";
            break;
        }
    }

    void F_DirectX12RHIPipelineStateProxy::_InitGraphics()
    {
        auto D3D12Device = GetContextProxy()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        
        auto CastedTemplateRuntimeProxy = GetTemplateRuntimeProxy().FastCast<F_DirectX12RHIPipelineStateTemplateRuntimeProxy>();
        auto CastedRootSignatureTemplateRuntimeProxy = CastedTemplateRuntimeProxy->GetRootSignatureTemplateRuntimeProxy().FastCast<F_DirectX12RHIRootSignatureTemplateRuntimeProxy>();
        auto CastedTemplate = CastedTemplateRuntimeProxy->GetTemplate().FastCast<F_DirectX12SharedRHIPipelineStateTemplate>();
        const auto& CompiledData = CastedTemplate->GetCompiledData();
        
        const auto& RTVFormats = CompiledData.RTVFormats;
        const auto& DSVFormat = CompiledData.DSVFormat;
        
        const auto& Rasterizer = CastedTemplate->GetRasterizer();
        const auto& DepthStencil = CastedTemplate->GetDepthStencil();
        const auto& Blend = CastedTemplate->GetBlend();
        
        TF_SmallVector<F_String, 8> SemanticNames;
        TF_SmallVector<D3D12_INPUT_ELEMENT_DESC, 8> D3D12InputElementDescs;
        D3D12_INPUT_LAYOUT_DESC D3D12InputLayoutDesc = {};
        
        {
            U32 NumInputElementDescs = CompiledData.InputElementDescs.size();
            SemanticNames.reserve(NumInputElementDescs);
            D3D12InputElementDescs.reserve(NumInputElementDescs);
            for (U32 InputElementIndex = 0; InputElementIndex < NumInputElementDescs; ++InputElementIndex)
            {
                const auto& InputElementDesc = CompiledData.InputElementDescs[InputElementIndex];
                SemanticNames.push_back(ToString(*InputElementDesc.SemanticName));
            
                D3D12_INPUT_ELEMENT_DESC D3D12InputElementDesc;
                D3D12InputElementDesc.SemanticName = SemanticNames.back().c_str();
                D3D12InputElementDesc.SemanticIndex = InputElementDesc.SemanticIndex;
                D3D12InputElementDesc.Format = RHIFormatToD3DFormat(InputElementDesc.Format);
                D3D12InputElementDesc.InputSlot = InputElementDesc.InputSlot;
                D3D12InputElementDesc.InputSlotClass = DirectX12SharedAPIWrapper::Conversions::RHIVertexInputElementClassToD3D12InputClassification(
                    InputElementDesc.Class
                );
                D3D12InputElementDesc.AlignedByteOffset = InputElementDesc.AlignedByteOffset;
                D3D12InputElementDesc.InstanceDataStepRate = (
                    (D3D12InputElementDesc.InputSlotClass == D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA)
                    ? 1
                    : 0
                );
                D3D12InputElementDescs.push_back(D3D12InputElementDesc);
            }
            D3D12InputLayoutDesc.NumElements = D3D12InputElementDescs.size();
            D3D12InputLayoutDesc.pInputElementDescs = D3D12InputElementDescs.data();
        }
        
        D3D12_PRIMITIVE_TOPOLOGY_TYPE D3D12PrimitiveTopologyType = DirectX12SharedAPIWrapper::Conversions::RHIPrimitiveTopologyToD3D12PrimitiveTopologyType(
            CastedTemplate->GetPrimitiveTopology()  
        );
        
        U32 NumRTVFormats = RTVFormats.size();
        TF_SmallVector<DXGI_FORMAT, 8> DXGIRTVFormats;
        for (auto Format : RTVFormats)
        {
            DXGIRTVFormats.push_back(RHIFormatToD3DFormat(Format));
        }
        
        TF_Optional<DXGI_FORMAT> DXGIDSVFormat;
        if (DSVFormat)
        {
            DXGIDSVFormat = RHIFormatToD3DFormat(*DSVFormat);
        }
        
        UINT D3D12SampleMask = UINT_MAX;
        DXGI_SAMPLE_DESC DXGISampleDesc = {};
        DXGISampleDesc.Count = 1;
        DXGISampleDesc.Quality = 0;
        
        D3D12_RASTERIZER_DESC D3D12RasterizerDesc = {};
        D3D12RasterizerDesc.DepthClipEnable = true;
        D3D12RasterizerDesc.CullMode = DirectX12SharedAPIWrapper::Conversions::RHICullModeToD3D12CullMode(
            Rasterizer.CullMode
        );
        D3D12RasterizerDesc.FillMode = DirectX12SharedAPIWrapper::Conversions::RHIFillModeToD3D12FillMode(
            Rasterizer.FillMode
        );
        D3D12RasterizerDesc.FrontCounterClockwise = Rasterizer.FrontCounterClockwise;
        D3D12RasterizerDesc.DepthBias = Rasterizer.DepthBias;
        D3D12RasterizerDesc.ConservativeRaster = DirectX12SharedAPIWrapper::Conversions::RHIConservativeRasterModeToD3D12ConservativeRasterizationMode(
            Rasterizer.ConservativeRasterMode    
        );
        
        D3D12_DEPTH_STENCIL_DESC D3D12DepthStencilDesc = {};
        D3D12DepthStencilDesc.DepthEnable = DepthStencil.EnableDepthTest;
        D3D12DepthStencilDesc.DepthFunc = DirectX12SharedAPIWrapper::Conversions::RHICompareOperationToD3D12ComparisonFunc(
            DepthStencil.DepthCompareOperation    
        );
        D3D12DepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK(DepthStencil.DepthBufferWrite);
        
        U32 NumBlendRTs = static_cast<U32>(Blend.RenderTargets.size());
        
        D3D12_BLEND_DESC D3D12BlendDesc = {};
        D3D12BlendDesc.AlphaToCoverageEnable = false;
        D3D12BlendDesc.IndependentBlendEnable = Blend.RenderTargets.size() > 1;
        
        for (U32 Idx = 0; Idx < NumBlendRTs; ++Idx)
        {
            const auto& BlendRT = Blend.RenderTargets[Idx];
            
            D3D12_RENDER_TARGET_BLEND_DESC D3D12RenderTargetBlendDesc = {};
            D3D12RenderTargetBlendDesc.BlendEnable = BlendRT.EnableBlend;
            D3D12RenderTargetBlendDesc.LogicOpEnable = false;
            D3D12RenderTargetBlendDesc.SrcBlend = DirectX12SharedAPIWrapper::Conversions::RHIBlendFactorToD3D12Blend(
                BlendRT.SrcBlendFactorColor
            );
            D3D12RenderTargetBlendDesc.DestBlend = DirectX12SharedAPIWrapper::Conversions::RHIBlendFactorToD3D12Blend(
                BlendRT.DestBlendFactorColor
            );
            D3D12RenderTargetBlendDesc.BlendOp = DirectX12SharedAPIWrapper::Conversions::RHIBlendOperationToD3D12BlendOp(
                BlendRT.BlendOperationColor
            );
            D3D12RenderTargetBlendDesc.SrcBlendAlpha = DirectX12SharedAPIWrapper::Conversions::RHIBlendFactorToD3D12Blend(
                BlendRT.SrcBlendFactorAlpha
            );
            D3D12RenderTargetBlendDesc.DestBlendAlpha = DirectX12SharedAPIWrapper::Conversions::RHIBlendFactorToD3D12Blend(
                BlendRT.DestBlendFactorAlpha
            );
            D3D12RenderTargetBlendDesc.BlendOpAlpha = DirectX12SharedAPIWrapper::Conversions::RHIBlendOperationToD3D12BlendOp(
                BlendRT.BlendOperationAlpha
            );
            D3D12RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
             
            D3D12BlendDesc.RenderTarget[Idx] = D3D12RenderTargetBlendDesc;
        }
        for (U32 Idx = NumBlendRTs; Idx < GetStaticArraySize(D3D12BlendDesc.RenderTarget); ++Idx)
        {
            D3D12_RENDER_TARGET_BLEND_DESC D3D12RenderTargetBlendDesc = {};
            D3D12RenderTargetBlendDesc.BlendEnable = false;
            D3D12RenderTargetBlendDesc.LogicOpEnable = false;
            D3D12RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            
            D3D12BlendDesc.RenderTarget[Idx] = D3D12RenderTargetBlendDesc;
        }
        
        if (CompiledData.MeshShader || CompiledData.AmplificationShader)
        {
            D3DX12_MESH_SHADER_PIPELINE_STATE_DESC D3D12XPipelineStateDesc = {};
            D3D12XPipelineStateDesc.pRootSignature = CastedRootSignatureTemplateRuntimeProxy->GetD3D12RootSignature().Get();
            D3D12XPipelineStateDesc.RasterizerState = D3D12RasterizerDesc;
            D3D12XPipelineStateDesc.DepthStencilState = D3D12DepthStencilDesc;
            memcpy(
                &D3D12XPipelineStateDesc.BlendState,
                &D3D12BlendDesc,
                sizeof(D3D12BlendDesc)
            );
            D3D12XPipelineStateDesc.PrimitiveTopologyType = D3D12PrimitiveTopologyType;
            D3D12XPipelineStateDesc.SampleMask = D3D12SampleMask;
            D3D12XPipelineStateDesc.SampleDesc = DXGISampleDesc;
            D3D12XPipelineStateDesc.NumRenderTargets = NumRTVFormats;
            {
                CopyStaticArray(
                    D3D12XPipelineStateDesc.RTVFormats,
                    DXGIRTVFormats.data()
                );
                for (U32 Idx = NumRTVFormats; Idx < GetStaticArraySize(D3D12XPipelineStateDesc.RTVFormats); ++Idx)
                {
                    D3D12XPipelineStateDesc.RTVFormats[Idx] = DXGI_FORMAT_UNKNOWN;
                }
            }
            D3D12XPipelineStateDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
            if (DXGIDSVFormat)
            {
                D3D12XPipelineStateDesc.DSVFormat = *DXGIDSVFormat;
            }
            
            if (CompiledData.MeshShader)
            {
                for (auto& Target : CompiledData.MeshShader->Targets)
                {
                    if (Target.Type != E_RHIShaderTargetType::DXIL)
                    {
                        continue;
                    }
                    D3D12XPipelineStateDesc.MS.pShaderBytecode = Target.DXIL.Binary.data();
                    D3D12XPipelineStateDesc.MS.BytecodeLength = Target.DXIL.Binary.size();
                }
            }
            if (CompiledData.AmplificationShader)
            {
                for (auto& Target : CompiledData.AmplificationShader->Targets)
                {
                    if (Target.Type != E_RHIShaderTargetType::DXIL)
                    {
                        continue;
                    }
                    D3D12XPipelineStateDesc.AS.pShaderBytecode = Target.DXIL.Binary.data();
                    D3D12XPipelineStateDesc.AS.BytecodeLength = Target.DXIL.Binary.size();
                }
            }
            if (CompiledData.PixelShader)
            {
                for (auto& Target : CompiledData.PixelShader->Targets)
                {
                    if (Target.Type != E_RHIShaderTargetType::DXIL)
                    {
                        continue;
                    }
                    D3D12XPipelineStateDesc.PS.pShaderBytecode = Target.DXIL.Binary.data();
                    D3D12XPipelineStateDesc.PS.BytecodeLength = Target.DXIL.Binary.size();
                }
            }

            auto D3D12PipelineStateStream = CD3DX12_PIPELINE_MESH_STATE_STREAM(D3D12XPipelineStateDesc);

            D3D12_PIPELINE_STATE_STREAM_DESC D3D12PipelineStateStreamDesc;
            D3D12PipelineStateStreamDesc.pPipelineStateSubobjectStream = &D3D12PipelineStateStream;
            D3D12PipelineStateStreamDesc.SizeInBytes                   = sizeof(D3D12PipelineStateStream);

            Microsoft::WRL::ComPtr<ID3D12Device2> D3D12Device2 = nullptr;
            D3D12Device->QueryInterface(
                IID_PPV_ARGS(&D3D12Device2)
            );
            ABYTEK_ENGINE_RHI_ASSERT(D3D12Device2) << "Mesh shader is not supported";
            
            HRESULT HR = D3D12Device2->CreatePipelineState(
                &D3D12PipelineStateStreamDesc,
                IID_PPV_ARGS(&_D3D12PipelineState)
            );
            ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot create d3d12 pipeline state";
        }
        else
        {
            D3D12_GRAPHICS_PIPELINE_STATE_DESC D3D12PipelineStateDesc = {};
            D3D12PipelineStateDesc.pRootSignature = CastedRootSignatureTemplateRuntimeProxy->GetD3D12RootSignature().Get();
            D3D12PipelineStateDesc.InputLayout = D3D12InputLayoutDesc;
            D3D12PipelineStateDesc.RasterizerState = D3D12RasterizerDesc;
            D3D12PipelineStateDesc.DepthStencilState = D3D12DepthStencilDesc;
            memcpy(
                &D3D12PipelineStateDesc.BlendState,
                &D3D12BlendDesc,
                sizeof(D3D12BlendDesc)
            );
            D3D12PipelineStateDesc.PrimitiveTopologyType = D3D12PrimitiveTopologyType;
            D3D12PipelineStateDesc.SampleMask = D3D12SampleMask;
            D3D12PipelineStateDesc.SampleDesc = DXGISampleDesc;
            D3D12PipelineStateDesc.NumRenderTargets = NumRTVFormats;
            {
                CopyStaticArray(
                    D3D12PipelineStateDesc.RTVFormats,
                    DXGIRTVFormats.data()
                );
                for (U32 Idx = NumRTVFormats; Idx < GetStaticArraySize(D3D12PipelineStateDesc.RTVFormats); ++Idx)
                {
                    D3D12PipelineStateDesc.RTVFormats[Idx] = DXGI_FORMAT_UNKNOWN;
                }
            }
            D3D12PipelineStateDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
            if (DXGIDSVFormat)
            {
                D3D12PipelineStateDesc.DSVFormat = *DXGIDSVFormat;
            }
        
            if (CompiledData.VertexShader)
            {
                for (auto& Target : CompiledData.VertexShader->Targets)
                {
                    if (Target.Type != E_RHIShaderTargetType::DXIL)
                    {
                        continue;
                    }
                    D3D12PipelineStateDesc.VS.pShaderBytecode = Target.DXIL.Binary.data();
                    D3D12PipelineStateDesc.VS.BytecodeLength = Target.DXIL.Binary.size();
                }
            }
            if (CompiledData.PixelShader)
            {
                for (auto& Target : CompiledData.PixelShader->Targets)
                {
                    if (Target.Type != E_RHIShaderTargetType::DXIL)
                    {
                        continue;
                    }
                    D3D12PipelineStateDesc.PS.pShaderBytecode = Target.DXIL.Binary.data();
                    D3D12PipelineStateDesc.PS.BytecodeLength = Target.DXIL.Binary.size();
                }
            }
            
            HRESULT HR = D3D12Device->CreateGraphicsPipelineState(
                &D3D12PipelineStateDesc,
                IID_PPV_ARGS(&_D3D12PipelineState)
            );
            ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot create d3d12 pipeline state";
        }
    }
    void F_DirectX12RHIPipelineStateProxy::_InitCompute()
    {
        auto D3D12Device = GetContextProxy()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        
        auto CastedTemplateRuntimeProxy = GetTemplateRuntimeProxy().FastCast<F_DirectX12RHIPipelineStateTemplateRuntimeProxy>();
        auto CastedRootSignatureTemplateRuntimeProxy = CastedTemplateRuntimeProxy->GetRootSignatureTemplateRuntimeProxy().FastCast<F_DirectX12RHIRootSignatureTemplateRuntimeProxy>();
        auto CastedTemplate = CastedTemplateRuntimeProxy->GetTemplate().FastCast<F_DirectX12SharedRHIPipelineStateTemplate>();
        const auto& CompiledData = CastedTemplate->GetCompiledData();
        
        D3D12_COMPUTE_PIPELINE_STATE_DESC D3D12ComputePipelineStateDesc = {};
        D3D12ComputePipelineStateDesc.pRootSignature = CastedRootSignatureTemplateRuntimeProxy->GetD3D12RootSignature().Get();
        
        ABYTEK_ENGINE_RHI_ASSERT(CompiledData.ComputeShader) << "Requires compute shader";
        
        for (auto& Target : CompiledData.ComputeShader->Targets)
        {
            if (Target.Type != E_RHIShaderTargetType::DXIL)
            {
                continue;
            }
            D3D12ComputePipelineStateDesc.CS.pShaderBytecode = Target.DXIL.Binary.data();
            D3D12ComputePipelineStateDesc.CS.BytecodeLength = Target.DXIL.Binary.size();
        }
            
        HRESULT HR = D3D12Device->CreateComputePipelineState(
            &D3D12ComputePipelineStateDesc,
            IID_PPV_ARGS(&_D3D12PipelineState)
        );
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot create d3d12 pipeline state";
    }
    void F_DirectX12RHIPipelineStateProxy::_InitWorkGraph()
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "Work graph was not supported";
    }*/
}
#endif