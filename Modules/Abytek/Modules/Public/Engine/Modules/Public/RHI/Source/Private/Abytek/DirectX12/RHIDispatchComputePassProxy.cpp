#include "Abytek/DirectX12/RHIDispatchComputePassProxy.hpp"
#include "Abytek/DirectX12/RHICommandSignature.hpp"
#include "Abytek/DirectX12/RHIContextProxy.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIDispatchComputePassProxy::Build(const TW_Valid<A_RHIDispatchComputePass>& Pass)
    {
        A_RHIDispatchComputePassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
        A_DirectX12RHIHasGeneralPipelineProxy::Build(Pass);
    }   
    void F_DirectX12RHIDispatchComputePassProxy::Release()
    {
        A_DirectX12RHIHasGeneralPipelineProxy::Release();
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIDispatchComputePassProxy::Release();
    }

    void F_DirectX12RHIDispatchComputePassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        
        const auto& IndirectProxy = GetIndirectProxy();
        const auto& NumThreadGroups = GetNumThreadGroups();
        if (
            (
                (NumThreadGroups.X == 0)
                || (NumThreadGroups.Y == 0)
                || (NumThreadGroups.Z == 0)
            )
            && !IndirectProxy
        )
        {
            return;
        }
        
        A_DirectX12RHIHasGeneralPipelineProxy::Execute(ExecuteParams);

        auto CastedContextProxy = GetContextProxy().FastCast<F_DirectX12RHIContextProxy>();

        Microsoft::WRL::ComPtr<ID3D12Resource> D3D12ArgumentBuffer;
        Microsoft::WRL::ComPtr<ID3D12Resource> D3D12CountBuffer;
        if (IndirectProxy)
        {
            ABYTEK_ENGINE_RHI_ASSERT(IndirectProxy->ArgumentBufferProxy) << "Invalid indirect argument buffer";
            D3D12ArgumentBuffer = IndirectProxy->ArgumentBufferProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
            if (IndirectProxy->CountBufferProxy)
            {
                D3D12CountBuffer = IndirectProxy->CountBufferProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
            }
        }
        
        if (IndirectProxy)
        {
            auto CommandSignature = CastedContextProxy->GetCommandSignature(E_RHIIndirectArgumentType::DISPATCH_COMPUTE);
            ExecuteParams.D3D12CommandList->ExecuteIndirect(
                CommandSignature->GetD3D12CommandSignature().Get(),
                D3D12CountBuffer ? (IndirectProxy->MaxCount) : 1,
                D3D12ArgumentBuffer.Get(),
                IndirectProxy->ArgumentBufferOffsetInBytes,
                D3D12CountBuffer.Get(),
                IndirectProxy->CountBufferOffsetInBytes
            );
        }
        else
        {
            ExecuteParams.D3D12CommandList->Dispatch(
                NumThreadGroups.X,    
                NumThreadGroups.Y,    
                NumThreadGroups.Z   
            );
        }
    }
}
#endif