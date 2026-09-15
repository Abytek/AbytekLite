#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIUploadBufferPass.hpp"
#include "Abytek/RHIReadbackBufferPass.hpp"
#include "Abytek/RHICopyBufferPass.hpp"
#include "Abytek/RHIUploadTexturePass.hpp"
#include "Abytek/RHIReadbackTexturePass.hpp"
#include "Abytek/RHICopyTexturePass.hpp"
#include "Abytek/RHIDispatchComputePass.hpp"
#include "Abytek/RHIDrawPass.hpp"
#include "Abytek/RHIViewport.hpp"
#include "Abytek/RHIClearRTVPass.hpp"
#include "Abytek/RHIClearDSVPass.hpp"
#include "Abytek/RHIPipelineState.hpp"
#include "Abytek/RHIResourceView.hpp"
#include "Abytek/RHISubmissionList.hpp"


namespace Abytek
{
    struct ABYTEK_ENGINE_RHI_API H_RHIPassUtilities
    {
        static void CopyBuffer(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIResource>& DstBuffer,
            U64 DstOffsetInBytes,
            const TS_Valid<A_RHIResource>& SrcBuffer,
            U64 SrcOffsetInBytes,
            U64 SizeInBytes,
            const F_DebugName& DebugName = {}
        );
        static void CopyTexture(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIResource>& DstTexture,
            const TS_Valid<A_RHIResource>& SrcTexture,
            F_RHITextureElementSet DstElementSet = {
                {
                    F_RHITextureElement {}
                }
            },
            F_RHITextureElementSet SrcElementSet = {
                {
                    F_RHITextureElement {}
                }
            },
            const F_DebugName& DebugName = {}
        );
        
        static void UploadBuffer(
            const F_RHIBufferDataView& BufferDataView,
            const TS_Valid<A_RHIResource>& Buffer,
            U64 OffsetInBytes = 0,
            const F_DebugName& DebugName = {}
        );
        static void ReadbackBuffer(
            F_RHIReadbackBufferCallback&& Callback,
            const TS_Valid<A_RHIResource>& Buffer,
            U64 SizeInBytes = 0,
            U64 OffsetInBytes = 0,
            const F_DebugName& DebugName = {}
        );
        static void UploadBuffer(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const F_RHIBufferDataView& BufferDataView,
            const TS_Valid<A_RHIResource>& Buffer,
            U64 OffsetInBytes = 0,
            const F_DebugName& DebugName = {}
        );
        static void ReadbackBuffer(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            F_RHIReadbackBufferCallback&& Callback,
            const TS_Valid<A_RHIResource>& Buffer,
            U64 SizeInBytes = 0,
            U64 OffsetInBytes = 0,
            const F_DebugName& DebugName = {}
        );
        static void UploadBuffer(
            I_RHISubmissionItemContainer& CPUSubmissionItemContainer,
            I_RHISubmissionItemContainer& GPUSubmissionItemContainer,
            const F_RHIBufferDataView& BufferDataView,
            const TS_Valid<A_RHIResource>& Buffer,
            U64 OffsetInBytes = 0,
            const F_DebugName& DebugName = {}
        );
        static void ReadbackBuffer(
            I_RHISubmissionItemContainer& CPUSubmissionItemContainer,
            I_RHISubmissionItemContainer& GPUSubmissionItemContainer,
            F_RHIReadbackBufferCallback&& Callback,
            const TS_Valid<A_RHIResource>& Buffer,
            U64 SizeInBytes = 0,
            U64 OffsetInBytes = 0,
            const F_DebugName& DebugName = {}
        );
        
        static void UploadTexture(
            const F_RHITextureDataView& TextureDataView,
            const TS_Valid<A_RHIResource>& Texture,
            const F_DebugName& DebugName = {}
        );
        static void ReadbackTexture(
            F_RHIReadbackTextureCallback&& Callback,
            const TS_Valid<A_RHIResource>& Texture,
            const F_DebugName& DebugName = {}
        );
        static void UploadTexture(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const F_RHITextureDataView& TextureDataView,
            const TS_Valid<A_RHIResource>& Texture,
            const F_DebugName& DebugName = {}
        );
        static void ReadbackTexture(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            F_RHIReadbackTextureCallback&& Callback,
            const TS_Valid<A_RHIResource>& Texture,
            const F_DebugName& DebugName = {}
        );
        static void UploadTexture(
            I_RHISubmissionItemContainer& CPUSubmissionItemContainer,
            I_RHISubmissionItemContainer& GPUSubmissionItemContainer,
            const F_RHITextureDataView& TextureDataView,
            const TS_Valid<A_RHIResource>& Texture,
            const F_DebugName& DebugName = {}
        );
        static void ReadbackTexture(
            I_RHISubmissionItemContainer& CPUSubmissionItemContainer,
            I_RHISubmissionItemContainer& GPUSubmissionItemContainer,
            F_RHIReadbackTextureCallback&& Callback,
            const TS_Valid<A_RHIResource>& Texture,
            const F_DebugName& DebugName = {}
        );
        
        static void DispatchCompute(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIPipelineState>& PipelineState,
            const F_RHIBindGroupSet& BindGroups,
            const F_Vector3_U32& NumThreadGroups,
            E_RHIGPUWorkClass GPUWorkClass = E_RHIGPUWorkClass::DEFAULT,
            const F_DebugName& DebugName = {}
        );
        static void DispatchComputeIndirect(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIPipelineState>& PipelineState,
            const F_RHIBindGroupSet& BindGroups,
            const F_RHIIndirectConfig& IndirectConfig,
            E_RHIGPUWorkClass GPUWorkClass = E_RHIGPUWorkClass::DEFAULT,
            const F_DebugName& DebugName = {}
        );
        
        static void DrawNonIndexed(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIPipelineState>& PipelineState,
            const F_RHIBindGroupSet& BindGroups,
            const F_RHIViewportScissorConfig& ViewportScissor,
            const F_RHIDrawNonIndexedConfig& Config,
            E_RHIGPUWorkClass GPUWorkClass = E_RHIGPUWorkClass::DEFAULT,
            const F_DebugName& DebugName = {}
        );
        static void DrawNonIndexedIndirect(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIPipelineState>& PipelineState,
            const F_RHIBindGroupSet& BindGroups,
            const F_RHIViewportScissorConfig& ViewportScissor,
            const F_RHIIndirectConfig& IndirectConfig,
            E_RHIGPUWorkClass GPUWorkClass = E_RHIGPUWorkClass::DEFAULT,
            const F_DebugName& DebugName = {}
        );
        
        static void DrawIndexed(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIPipelineState>& PipelineState,
            const F_RHIBindGroupSet& BindGroups,
            const F_RHIViewportScissorConfig& ViewportScissor,
            const F_RHIDrawIndexedConfig& Config,
            E_RHIGPUWorkClass GPUWorkClass = E_RHIGPUWorkClass::DEFAULT,
            const F_DebugName& DebugName = {}
        );
        static void DrawIndexedIndirect(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIPipelineState>& PipelineState,
            const F_RHIBindGroupSet& BindGroups,
            const F_RHIViewportScissorConfig& ViewportScissor,
            const F_RHIIndirectConfig& IndirectConfig,
            E_RHIGPUWorkClass GPUWorkClass = E_RHIGPUWorkClass::DEFAULT,
            const F_DebugName& DebugName = {}
        );
        
        static void DrawDispatchMesh(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIPipelineState>& PipelineState,
            const F_RHIBindGroupSet& BindGroups,
            const F_RHIViewportScissorConfig& ViewportScissor,
            const F_RHIDrawDispatchMeshConfig& Config,
            E_RHIGPUWorkClass GPUWorkClass = E_RHIGPUWorkClass::DEFAULT,
            const F_DebugName& DebugName = {}
        );
        static void DrawDispatchMeshIndirect(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIPipelineState>& PipelineState,
            const F_RHIBindGroupSet& BindGroups,
            const F_RHIViewportScissorConfig& ViewportScissor,
            const F_RHIIndirectConfig& IndirectConfig,
            E_RHIGPUWorkClass GPUWorkClass = E_RHIGPUWorkClass::DEFAULT,
            const F_DebugName& DebugName = {}
        );
        
        static void ClearRTV(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIResourceView>& RTV, 
            const F_Vector4_F32& ClearColor,
            const F_DebugName& DebugName = {}
        );
        static void ClearDSV(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIResourceView>& DSV,
            E_RHIClearDSVFlag Flags = E_RHIClearDSVFlag::DEPTH,
            F32 Depth = 0.0f,
            U8 Stencil = 0,
            const F_DebugName& DebugName = {}
        );
        static void ClearUAVFloat(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIResourceView>& UAV, 
            const F_Vector4_F32& Value,
            const F_DebugName& DebugName = {}
        );
        static void ClearUAVUInt(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIResourceView>& UAV, 
            const F_Vector4_U64& Value,
            const F_DebugName& DebugName = {}
        );
        
        static void ClearViewport(
            I_RHISubmissionItemContainer& SubmissionItemContainer,
            const TS_Valid<A_RHIViewport>& Viewport, 
            const F_Vector4_F32& ClearColor,
            const F_DebugName& DebugName = {}
        );
    };
}