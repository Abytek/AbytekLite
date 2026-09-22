#include "Abytek/RHISubmissionUtilities.hpp"
#include "Abytek/RHIClearUAVFloatPass.hpp"
#include "Abytek/RHIClearUAVUIntPass.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIDevice.hpp"
#include "Abytek/RHITransientUploadBufferManager.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntime.hpp"
#include "Abytek/RHIViewportPresentation.hpp"


namespace Abytek
{
    void H_RHISubmissionUtilities::CopyBuffer(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIResource>& DstBuffer, 
        U64 DstOffsetInBytes, 
        const TS_Valid<A_RHIResource>& SrcBuffer,
        U64 SrcOffsetInBytes,
        U64 SizeInBytes,
        const F_DebugName& DebugName
    )
    {
        F_RHICopyBufferPassBuildParams CopyBufferPassBuildParams;
        CopyBufferPassBuildParams.Context = DstBuffer->GetContext();
        CopyBufferPassBuildParams.DstBuffer = DstBuffer;
        CopyBufferPassBuildParams.DstOffsetInBytes = DstOffsetInBytes;
        CopyBufferPassBuildParams.SrcBuffer = SrcBuffer;
        CopyBufferPassBuildParams.SrcOffsetInBytes = SrcOffsetInBytes;
        CopyBufferPassBuildParams.SizeInBytes = SizeInBytes;
        auto CopyBufferPass = RACreateAndBuildShared<A_RHICopyBufferPass>(CopyBufferPassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        CopyBufferPass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(CopyBufferPass);
    }
    void H_RHISubmissionUtilities::CopyTexture(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIResource>& DstTexture, 
        const TS_Valid<A_RHIResource>& SrcTexture,
        F_RHITextureElementSet SrcElementSet, 
        F_RHITextureElementSet DstElementSet,
        const F_DebugName& DebugName
    )
    {
        F_RHICopyTexturePassBuildParams CopyTexturePassBuildParams;
        CopyTexturePassBuildParams.Context = DstTexture->GetContext();
        CopyTexturePassBuildParams.DestTexture = DstTexture;
        CopyTexturePassBuildParams.DstElementSet = DstElementSet;
        CopyTexturePassBuildParams.SrcTexture = SrcTexture;
        CopyTexturePassBuildParams.SrcElementSet = SrcElementSet;
        auto CopyTexturePass = RACreateAndBuildShared<A_RHICopyTexturePass>(CopyTexturePassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        CopyTexturePass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(CopyTexturePass);
    }

    void H_RHISubmissionUtilities::UploadBuffer( 
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RHIBufferDataView& BufferDataView,
        const TS_Valid<A_RHIResource>& Buffer, 
        U64 OffsetInBytes,
        const F_DebugName& DebugName
    )
    {
        F_RHIUploadBufferPassBuildParams UploadBufferPassBuildParams;
        UploadBufferPassBuildParams.Context = Buffer->GetContext();
        UploadBufferPassBuildParams.Buffer = Buffer;
        UploadBufferPassBuildParams.OffsetInBytes = OffsetInBytes;
        UploadBufferPassBuildParams.BufferDataView = BufferDataView;
        auto UploadBufferPass = RACreateAndBuildShared<A_RHIUploadBufferPass>(UploadBufferPassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        UploadBufferPass->SetDebugName(DebugName);
#endif
            
        SubmissionItemContainer->AddSubmissionItem(UploadBufferPass);
    }
    void H_RHISubmissionUtilities::ReadbackBuffer(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        F_RHIReadbackBufferCallback&& Callback, 
        const TS_Valid<A_RHIResource>& Buffer, 
        U64 SizeInBytes,
        U64 OffsetInBytes,
        const F_DebugName& DebugName
    )
    {
        F_RHIReadbackBufferPassBuildParams ReadbackBufferPassBuildParams;
        ReadbackBufferPassBuildParams.Context = Buffer->GetContext();
        ReadbackBufferPassBuildParams.Buffer = Buffer;
        ReadbackBufferPassBuildParams.SizeInBytes = SizeInBytes;
        ReadbackBufferPassBuildParams.OffsetInBytes = OffsetInBytes;
        ReadbackBufferPassBuildParams.Callback = ABYTEK_MOVE(Callback);
        auto ReadbackBufferPass = RACreateAndBuildShared<A_RHIReadbackBufferPass>(ReadbackBufferPassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        ReadbackBufferPass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(ReadbackBufferPass);
    }

    void H_RHISubmissionUtilities::UploadTexture( 
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RHITextureDataView& TextureDataView,
        const TS_Valid<A_RHIResource>& Texture, 
        const F_DebugName& DebugName
    )
    {
        F_RHIUploadTexturePassBuildParams UploadTexturePassBuildParams;
        UploadTexturePassBuildParams.Context = Texture->GetContext();
        UploadTexturePassBuildParams.Texture = Texture;
        UploadTexturePassBuildParams.TextureDataView = TextureDataView;
        auto UploadTexturePass = RACreateAndBuildShared<A_RHIUploadTexturePass>(UploadTexturePassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        UploadTexturePass->SetDebugName(DebugName);
#endif
            
        SubmissionItemContainer->AddSubmissionItem(UploadTexturePass);
    }
    void H_RHISubmissionUtilities::ReadbackTexture(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        F_RHIReadbackTextureCallback&& Callback, 
        const TS_Valid<A_RHIResource>& Texture, 
        const F_DebugName& DebugName
    )
    {
        F_RHIReadbackTexturePassBuildParams ReadbackTexturePassBuildParams;
        ReadbackTexturePassBuildParams.Context = Texture->GetContext();
        ReadbackTexturePassBuildParams.Texture = Texture;
        ReadbackTexturePassBuildParams.Callback = ABYTEK_MOVE(Callback);
        auto ReadbackTexturePass = RACreateAndBuildShared<A_RHIReadbackTexturePass>(ReadbackTexturePassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        ReadbackTexturePass->SetDebugName(DebugName);
#endif
            
        SubmissionItemContainer->AddSubmissionItem(ReadbackTexturePass);
    }

    void H_RHISubmissionUtilities::DispatchCompute(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIPipelineState>& PipelineState, 
        const F_RHIBindGroupSet& BindGroups,
        const F_Vector3_U32& NumThreadGroups, 
        E_RHIGPUWorkClass GPUWorkClass,
        const F_DebugName& DebugName
    )
    {
        F_RHIDispatchComputePassBuildParams BuildParams;
        BuildParams.Context = PipelineState->GetContext();
        BuildParams.PipelineState = PipelineState;
        BuildParams.BindGroups = BindGroups;
        BuildParams.NumThreadGroups = NumThreadGroups;
        BuildParams.GPUWorkClass = GPUWorkClass;
        auto Pass = RACreateAndBuildShared<A_RHIDispatchComputePass>(BuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Pass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(Pass);
    }
    void H_RHISubmissionUtilities::DispatchComputeIndirect(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIPipelineState>& PipelineState, 
        const F_RHIBindGroupSet& BindGroups,
        const F_RHIIndirectConfig& IndirectConfig, 
        E_RHIGPUWorkClass GPUWorkClass,
        const F_DebugName& DebugName
    )
    {
        F_RHIDispatchComputePassBuildParams BuildParams;
        BuildParams.Context = PipelineState->GetContext();
        BuildParams.PipelineState = PipelineState;
        BuildParams.BindGroups = BindGroups;
        BuildParams.Indirect = IndirectConfig;
        BuildParams.GPUWorkClass = GPUWorkClass;
        auto Pass = RACreateAndBuildShared<A_RHIDispatchComputePass>(BuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Pass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(Pass);
    }

    void H_RHISubmissionUtilities::DrawNonIndexed(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIPipelineState>& PipelineState, 
        const F_RHIBindGroupSet& BindGroups,
        const F_RHIViewportScissorConfig& ViewportScissor,
        const F_RHIDrawNonIndexedConfig& Config, 
        E_RHIGPUWorkClass GPUWorkClass, 
        const F_DebugName& DebugName
    )
    {
        F_RHIDrawPassBuildParams BuildParams;
        BuildParams.Context = PipelineState->GetContext();
        BuildParams.PipelineState = PipelineState;
        BuildParams.BindGroups = BindGroups;
        BuildParams.ViewportScissor = ViewportScissor;
        BuildParams.DrawType = E_RHIDrawType::NON_INDEXED;
        BuildParams.NonIndexed = Config;
        BuildParams.GPUWorkClass = GPUWorkClass;
        auto Pass = RACreateAndBuildShared<A_RHIDrawPass>(BuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Pass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(Pass);
    }
    void H_RHISubmissionUtilities::DrawNonIndexedIndirect(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIPipelineState>& PipelineState, 
        const F_RHIBindGroupSet& BindGroups,
        const F_RHIViewportScissorConfig& ViewportScissor,
        const F_RHIIndirectConfig& IndirectConfig, 
        E_RHIGPUWorkClass GPUWorkClass, 
        const F_DebugName& DebugName
    )
    {
        F_RHIDrawPassBuildParams BuildParams;
        BuildParams.Context = PipelineState->GetContext();
        BuildParams.PipelineState = PipelineState;
        BuildParams.BindGroups = BindGroups;
        BuildParams.ViewportScissor = ViewportScissor;
        BuildParams.DrawType = E_RHIDrawType::NON_INDEXED;
        BuildParams.Indirect = IndirectConfig;
        BuildParams.GPUWorkClass = GPUWorkClass;
        auto Pass = RACreateAndBuildShared<A_RHIDrawPass>(BuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Pass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(Pass);
    }

    void H_RHISubmissionUtilities::DrawIndexed(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIPipelineState>& PipelineState, 
        const F_RHIBindGroupSet& BindGroups,
        const F_RHIViewportScissorConfig& ViewportScissor,
        const F_RHIDrawIndexedConfig& Config, 
        E_RHIGPUWorkClass GPUWorkClass, 
        const F_DebugName& DebugName
    )
    {
        F_RHIDrawPassBuildParams BuildParams;
        BuildParams.Context = PipelineState->GetContext();
        BuildParams.PipelineState = PipelineState;
        BuildParams.BindGroups = BindGroups;
        BuildParams.ViewportScissor = ViewportScissor;
        BuildParams.DrawType = E_RHIDrawType::INDEXED;
        BuildParams.Indexed = Config;
        BuildParams.GPUWorkClass = GPUWorkClass;
        auto Pass = RACreateAndBuildShared<A_RHIDrawPass>(BuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Pass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(Pass);
    }
    void H_RHISubmissionUtilities::DrawIndexedIndirect(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIPipelineState>& PipelineState, 
        const F_RHIBindGroupSet& BindGroups,
        const F_RHIViewportScissorConfig& ViewportScissor,
        const F_RHIIndirectConfig& IndirectConfig, 
        E_RHIGPUWorkClass GPUWorkClass, 
        const F_DebugName& DebugName
    )
    {
        F_RHIDrawPassBuildParams BuildParams;
        BuildParams.Context = PipelineState->GetContext();
        BuildParams.PipelineState = PipelineState;
        BuildParams.BindGroups = BindGroups;
        BuildParams.ViewportScissor = ViewportScissor;
        BuildParams.DrawType = E_RHIDrawType::INDEXED;
        BuildParams.Indirect = IndirectConfig;
        BuildParams.GPUWorkClass = GPUWorkClass;
        auto Pass = RACreateAndBuildShared<A_RHIDrawPass>(BuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Pass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(Pass);
    }

    void H_RHISubmissionUtilities::DrawDispatchMesh(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIPipelineState>& PipelineState, 
        const F_RHIBindGroupSet& BindGroups,
        const F_RHIViewportScissorConfig& ViewportScissor,
        const F_RHIDrawDispatchMeshConfig& Config, 
        E_RHIGPUWorkClass GPUWorkClass, 
        const F_DebugName& DebugName
    )
    {
        F_RHIDrawPassBuildParams BuildParams;
        BuildParams.Context = PipelineState->GetContext();
        BuildParams.PipelineState = PipelineState;
        BuildParams.BindGroups = BindGroups;
        BuildParams.ViewportScissor = ViewportScissor;
        BuildParams.DrawType = E_RHIDrawType::DISPATCH_MESH;
        BuildParams.DispatchMesh = Config;
        BuildParams.GPUWorkClass = GPUWorkClass;
        auto Pass = RACreateAndBuildShared<A_RHIDrawPass>(BuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Pass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(Pass);
    }
    void H_RHISubmissionUtilities::DrawDispatchMeshIndirect(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIPipelineState>& PipelineState, 
        const F_RHIBindGroupSet& BindGroups,
        const F_RHIViewportScissorConfig& ViewportScissor,
        const F_RHIIndirectConfig& IndirectConfig, 
        E_RHIGPUWorkClass GPUWorkClass, 
        const F_DebugName& DebugName
    )
    {
        F_RHIDrawPassBuildParams BuildParams;
        BuildParams.Context = PipelineState->GetContext();
        BuildParams.PipelineState = PipelineState;
        BuildParams.BindGroups = BindGroups;
        BuildParams.ViewportScissor = ViewportScissor;
        BuildParams.DrawType = E_RHIDrawType::DISPATCH_MESH;
        BuildParams.Indirect = IndirectConfig;
        BuildParams.GPUWorkClass = GPUWorkClass;
        auto Pass = RACreateAndBuildShared<A_RHIDrawPass>(BuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Pass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(Pass);
    }

    void H_RHISubmissionUtilities::ClearRTV(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIResourceView>& RTV, 
        const F_Vector4_F32& ClearColor, 
        const F_DebugName& DebugName
    )
    {
        F_RHIClearRTVPassBuildParams ClearRTVPassBuildParams;
        ClearRTVPassBuildParams.Context = RTV->GetContext().Weak();
        ClearRTVPassBuildParams.RTV = RTV;
        ClearRTVPassBuildParams.Color = ClearColor;
        auto ClearRTVPass = RACreateAndBuildShared<A_RHIClearRTVPass>(ClearRTVPassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        ClearRTVPass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(ClearRTVPass);
    }
    void H_RHISubmissionUtilities::ClearDSV(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIResourceView>& DSV,
        E_RHIClearDSVFlag Flags, 
        F32 Depth, 
        U8 Stencil, 
        const F_DebugName& DebugName
    )
    {
        F_RHIClearDSVPassBuildParams ClearDSVPassBuildParams;
        ClearDSVPassBuildParams.Context = DSV->GetContext().Weak();
        ClearDSVPassBuildParams.DSV = DSV;
        ClearDSVPassBuildParams.Flags = Flags;
        ClearDSVPassBuildParams.Depth = Depth;
        ClearDSVPassBuildParams.Stencil = Stencil;
        auto ClearDSVPass = RACreateAndBuildShared<A_RHIClearDSVPass>(ClearDSVPassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        ClearDSVPass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(ClearDSVPass);
    }
    void H_RHISubmissionUtilities::ClearUAVFloat(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIResourceView>& UAV, 
        const F_Vector4_F32& Value, 
        const F_DebugName& DebugName
        )
    {
        F_RHIClearUAVFloatPassBuildParams ClearUAVFloatPassBuildParams;
        ClearUAVFloatPassBuildParams.Context = UAV->GetContext().Weak();
        ClearUAVFloatPassBuildParams.UAV = UAV;
        ClearUAVFloatPassBuildParams.Value = Value;
        auto ClearUAVFloatPass = RACreateAndBuildShared<A_RHIClearUAVFloatPass>(ClearUAVFloatPassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        ClearUAVFloatPass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(ClearUAVFloatPass);
    }
    void H_RHISubmissionUtilities::ClearUAVUInt(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIResourceView>& UAV, 
        const F_Vector4_U64& Value, 
        const F_DebugName& DebugName
    )
    {
        F_RHIClearUAVUIntPassBuildParams ClearUAVUIntPassBuildParams;
        ClearUAVUIntPassBuildParams.Context = UAV->GetContext().Weak();
        ClearUAVUIntPassBuildParams.UAV = UAV;
        ClearUAVUIntPassBuildParams.Value = Value;
        auto ClearUAVUIntPass = RACreateAndBuildShared<A_RHIClearUAVUIntPass>(ClearUAVUIntPassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        ClearUAVUIntPass->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(ClearUAVUIntPass);
    }

    void H_RHISubmissionUtilities::ClearViewport(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS_Valid<A_RHIViewport>& Viewport, 
        const F_Vector4_F32& ClearColor, 
        const F_DebugName& DebugName
    )
    {
        ClearRTV(
            SubmissionItemContainer,
            Viewport->GetCurrentBackRenderTargetView(),
            ClearColor,
            DebugName
        );
    }

    void H_RHISubmissionUtilities::PresentViewport(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const TS_Valid<A_RHIViewport>& Viewport,
        const F_DebugName& DebugName
    )
    {
        F_RHIViewportPresentationBuildParams ViewportPresentationBuildParams;
        ViewportPresentationBuildParams.Viewport = Viewport;
        auto ViewportPresentation = RACreateAndBuildShared<A_RHIViewportPresentation>(ViewportPresentationBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        ViewportPresentation->SetDebugName(DebugName);
#endif
        
        SubmissionItemContainer->AddSubmissionItem(ViewportPresentation);
    }
}
