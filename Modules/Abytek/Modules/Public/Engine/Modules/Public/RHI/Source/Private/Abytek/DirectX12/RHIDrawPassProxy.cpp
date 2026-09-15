#include "Abytek/DirectX12/RHIDrawPassProxy.hpp"
#include "Abytek/DirectX12/RHICommandSignature.hpp"
#include "Abytek/DirectX12/RHIContextProxy.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIDrawPassProxy::Build(const TW_Valid<A_RHIDrawPass>& Pass)
    {
        A_RHIDrawPassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
        A_DirectX12RHIHasGeneralPipelineProxy::Build(Pass);
    }   
    void F_DirectX12RHIDrawPassProxy::Release()
    {
        A_DirectX12RHIHasGeneralPipelineProxy::Release();
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIDrawPassProxy::Release();
    }

    void F_DirectX12RHIDrawPassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);

        auto DrawType = GetDrawType();
        const auto& NonIndexed = GetNonIndexed();
        const auto& Indexed = GetIndexed();
        const auto& DispatchMesh = GetDispatchMesh();
        const auto& IndirectProxy = GetIndirectProxy();
        switch (DrawType)
        {
        case E_RHIDrawType::NON_INDEXED:
            if (
                (
                    (NonIndexed.NumVerticesPerInstance == 0)    
                    || (NonIndexed.NumInstances == 0)  
                )
                && !IndirectProxy
            )
            {
                return;
            }
            break;
        case E_RHIDrawType::INDEXED:
            if (
                (
                    (Indexed.NumIndicesPerInstance == 0)    
                    || (Indexed.NumInstances == 0)    
                )
                && !IndirectProxy
            )
            {
                return;
            }
            break;
        case E_RHIDrawType::DISPATCH_MESH:
            if (
                (
                    (DispatchMesh.NumThreadGroups.X == 0)    
                    || (DispatchMesh.NumThreadGroups.Y == 0)    
                    || (DispatchMesh.NumThreadGroups.Z == 0)   
                )
                && !IndirectProxy
            )
            {
                return;
            }
            break;
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown draw type";
            break;
        }
        
        A_DirectX12RHIHasGeneralPipelineProxy::Execute(ExecuteParams);
        
        auto CastedContextProxy = GetContextProxy().FastCast<F_DirectX12RHIContextProxy>();
        
        const auto& ViewportScissor = GetViewportScissor();
        ExecuteParams.Update_ViewportConfigs(ViewportScissor.Viewports);
        ExecuteParams.Update_Scissors(ViewportScissor.Scissors);
        
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
        
        switch (DrawType)
        {
        case E_RHIDrawType::NON_INDEXED:
            if (IndirectProxy)
            {
                auto CommandSignature = CastedContextProxy->GetCommandSignature(E_RHIIndirectArgumentType::DRAW_NON_INDEXED);
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
                ExecuteParams.D3D12CommandList->DrawInstanced(
                    NonIndexed.NumVerticesPerInstance,
                    NonIndexed.NumInstances,
                    NonIndexed.VertexOffset,
                    NonIndexed.InstanceOffset
                );
            }
            break;
        case E_RHIDrawType::INDEXED:
            if (IndirectProxy)
            {
                auto CommandSignature = CastedContextProxy->GetCommandSignature(E_RHIIndirectArgumentType::DRAW_INDEXED);
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
                ExecuteParams.D3D12CommandList->DrawIndexedInstanced(
                    Indexed.NumIndicesPerInstance,
                    Indexed.NumInstances,
                    Indexed.IndexOffset,
                    Indexed.VertexOffset,
                    Indexed.InstanceOffset
                );
            }
            break;
        case E_RHIDrawType::DISPATCH_MESH:
            if (IndirectProxy)
            {
                auto CommandSignature = CastedContextProxy->GetCommandSignature(E_RHIIndirectArgumentType::DISPATCH_MESH);
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
                Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> D3D12CommandList6;
                HRESULT HR = ExecuteParams.D3D12CommandList.As(&D3D12CommandList6);
                ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR), "Mesh shader is not supported");
                D3D12CommandList6->DispatchMesh(
                    DispatchMesh.NumThreadGroups.X,  
                    DispatchMesh.NumThreadGroups.Y,  
                    DispatchMesh.NumThreadGroups.Z  
                );
            }
            break;
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown draw type";
            break;
        }
    }
}
#endif