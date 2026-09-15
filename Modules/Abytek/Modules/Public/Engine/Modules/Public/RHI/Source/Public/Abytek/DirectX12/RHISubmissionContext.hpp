#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIBindGroupProxy.hpp"
#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIDrawPass.hpp"
#include "Abytek/RHIPrimitiveTopology.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    struct F_DirectX12RHISubmissionContext
    { 
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> D3D12CommandList = nullptr;
        
        TF_SmallVector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>, 2> D3D12DescriptorHeaps;
        
        Microsoft::WRL::ComPtr<ID3D12RootSignature> D3D12RootSignature_Graphics = nullptr;
        Microsoft::WRL::ComPtr<ID3D12RootSignature> D3D12RootSignature_Compute = nullptr;
        F_RHIBindGroupProxySet_Weak BindGroupProxySet_Graphics;
        F_RHIBindGroupProxySet_Weak BindGroupProxySet_Compute;
        
        TF_SmallVector<F_RHIViewportConfig, 1> ViewportConfigs;
        TF_SmallVector<F_Rect_F32, 1> Scissors;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> D3D12PipelineState = nullptr;
        E_RHIPrimitiveTopology PrimitiveTopology = E_RHIPrimitiveTopology::NONE;
        
        ABYTEK_FORCE_INLINE auto& GetD3D12RootSignature(E_RHIPassClass PassClass)
        {
            switch (PassClass)
            {
            case E_RHIPassClass::GRAPHICS:
                return D3D12RootSignature_Graphics;
            case E_RHIPassClass::COMPUTE:
                return D3D12RootSignature_Compute;
            default:
                ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown pass class";
                break;
            }
            return D3D12RootSignature_Graphics;
        }
        ABYTEK_FORCE_INLINE auto& GetBindGroupProxySet(E_RHIPassClass PassClass)
        {
            switch (PassClass)
            {
            case E_RHIPassClass::GRAPHICS:
                return BindGroupProxySet_Graphics;
            case E_RHIPassClass::COMPUTE:
                return BindGroupProxySet_Compute;
            default:
                ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown pass class";
                break;
            }
            return BindGroupProxySet_Graphics;
        }
        
        void Update_D3D12DescriptorHeaps(
            const TF_SmallVector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>, 2>& NewD3D12DescriptorHeaps
        )
        {
            U32 NumDescriptorHeaps_Last = D3D12DescriptorHeaps.size();
            U32 NumDescriptorHeaps = NewD3D12DescriptorHeaps.size();
        
            B8 IsDirty = false;
            if (NumDescriptorHeaps == NumDescriptorHeaps_Last)
            {
                for (U32 Idx = 0; Idx < NumDescriptorHeaps; ++Idx)
                {
                    if (NewD3D12DescriptorHeaps[Idx] != D3D12DescriptorHeaps[Idx])
                    {
                        IsDirty = true;
                        break;
                    }
                }
            }
            else
            {
                IsDirty = true;
            }
        
            if (!IsDirty)
            {
                return;
            }
            
            TF_SmallVector<ID3D12DescriptorHeap*, 2> D3D12DescriptorHeaps_RawPtr;
            D3D12DescriptorHeaps_RawPtr.reserve(NumDescriptorHeaps);
            for (const auto& D3D12DescriptorHeap : NewD3D12DescriptorHeaps)
            {
                D3D12DescriptorHeaps_RawPtr.push_back(D3D12DescriptorHeap.Get());
            }
            
            D3D12CommandList->SetDescriptorHeaps(
                D3D12DescriptorHeaps_RawPtr.size(), 
                D3D12DescriptorHeaps_RawPtr.data()
            );
            
            D3D12DescriptorHeaps = NewD3D12DescriptorHeaps;
            
            BindGroupProxySet_Graphics = {};
            BindGroupProxySet_Compute = {};
        }
        void Update_D3D12RootSignature(E_RHIPassClass PassClass, const Microsoft::WRL::ComPtr<ID3D12RootSignature>& NewD3D12RootSignature)
        {
            auto& D3D12RootSignature = GetD3D12RootSignature(PassClass);
            if (D3D12RootSignature == NewD3D12RootSignature)
            {
                return;   
            }

            switch (PassClass)
            {
            case E_RHIPassClass::GRAPHICS:
                D3D12CommandList->SetGraphicsRootSignature(NewD3D12RootSignature.Get());
                break;
            case E_RHIPassClass::COMPUTE:
                D3D12CommandList->SetComputeRootSignature(NewD3D12RootSignature.Get());
                break;
            default:
                ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid pass class";
                break;
            }
                
            D3D12RootSignature = NewD3D12RootSignature;
            GetBindGroupProxySet(PassClass) = {};
        }
        void Update_ViewportConfigs(const TF_SmallVector<F_RHIViewportConfig, 1>& InViewportConfigs)
        {
            if (ViewportConfigs == InViewportConfigs)
            {
                return;
            }
            ABYTEK_ENGINE_RHI_ASSERT(InViewportConfigs.size() > 0) << "Invalid num viewport configs";
            TF_SmallVector<D3D12_VIEWPORT, 1> D3D12Viewports;
            for (const auto& ViewportConfig : InViewportConfigs)
            {
                D3D12_VIEWPORT D3D12Viewport;
                D3D12Viewport.TopLeftX = ViewportConfig.Min.X;
                D3D12Viewport.TopLeftY = ViewportConfig.Min.Y;
                D3D12Viewport.Width = ViewportConfig.Max.X - ViewportConfig.Min.X;
                D3D12Viewport.Height = ViewportConfig.Max.Y - ViewportConfig.Min.Y;
                D3D12Viewport.MinDepth = ViewportConfig.Min.Z;
                D3D12Viewport.MaxDepth = ViewportConfig.Max.Z;
                D3D12Viewports.push_back(D3D12Viewport);
            }
            D3D12CommandList->RSSetViewports(
                static_cast<U32>(D3D12Viewports.size()),
                D3D12Viewports.data()
            );
            ViewportConfigs = InViewportConfigs;
        }
        void Update_Scissors(const TF_SmallVector<F_Rect_F32, 1>& InScissors)
        {
            if (Scissors == InScissors)
            {
                return;
            }
            ABYTEK_ENGINE_RHI_ASSERT(InScissors.size() > 0) << "Invalid num scissors";
            TF_SmallVector<D3D12_RECT, 1> D3D12ScissorRects;
            for (const auto& Scissor : InScissors)
            {
                D3D12_RECT D3D12ScissorRect;
                D3D12ScissorRect.left = Scissor.Min.X;
                D3D12ScissorRect.top = Scissor.Min.Y;
                D3D12ScissorRect.right = Scissor.Max.X;
                D3D12ScissorRect.bottom = Scissor.Max.Y;
                D3D12ScissorRects.push_back(D3D12ScissorRect);
            }
            D3D12CommandList->RSSetScissorRects(
                static_cast<U32>(D3D12ScissorRects.size()),
                D3D12ScissorRects.data()
            );
            Scissors = InScissors;
        }
        void Update_D3D12PipelineState(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& InD3D12PipelineState)
        {
            if (D3D12PipelineState == InD3D12PipelineState)
            {
                return;
            }
            D3D12CommandList->SetPipelineState(InD3D12PipelineState.Get());
            D3D12PipelineState = InD3D12PipelineState;
        }
        void Update_PrimitiveTopology(E_RHIPrimitiveTopology InPrimitiveTopology)
        {
            if (PrimitiveTopology == InPrimitiveTopology)
            {
                return;
            }
            ABYTEK_ENGINE_RHI_ASSERT(InPrimitiveTopology != E_RHIPrimitiveTopology::NONE) << "Invalid primitive topology";
            D3D12CommandList->IASetPrimitiveTopology(
                DirectX12SharedAPIWrapper::Conversions::RHIPrimitiveTopologyToD3DPrimitiveTopology(InPrimitiveTopology)    
            );
            PrimitiveTopology = InPrimitiveTopology;
        }
    };
}
#endif