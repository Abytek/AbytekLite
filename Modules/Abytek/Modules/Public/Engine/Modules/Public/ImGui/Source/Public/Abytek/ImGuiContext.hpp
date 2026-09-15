#pragma once

#include "Abytek/Engine.ImGui.prerequisites.hpp"
#include "Abytek/DirectX12/RHICommandListManager.hpp"
#include "Abytek/LLEvents/Windows/LLEvents.hpp"


namespace Abytek
{
    class F_ImGuiRenderData;
    
    struct F_ImGuiContextBuildParams
    {
        TW<A_Window> Window;
    };
    
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
    struct F_DirectX12ImGuiPackPopParams
    {
        F_Vector2_U32 Size = F_Vector2_U32::Zero();  
    };
    struct F_DirectX12ImGuiPack
    {
        F_Vector2_U32 Size = F_Vector2_U32::Zero();  
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> D3D12CommandAllocator = nullptr;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> D3D12CommandList = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Resource> D3D12Resource = nullptr;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> D3D12DescriptorHeap = nullptr;
        D3D12_CPU_DESCRIPTOR_HANDLE D3D12CPUDescriptorHandle;
    };
    struct F_DirectX12ImGuiSRVDescriptor
    {
        D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle;
    };
#endif
    
    class ABYTEK_ENGINE_IMGUI_API F_ImGuiContext : public A_Object
    {
    private:
        TW<A_Window> _Window;
        
        ImGuiContext* _ImGuiContext = nullptr;
        
        TS<A_RHIContextProxy> _RHIContextProxy;
        
        F_YieldSpinLock _FrameLock;
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_WIN32
        F_WinAPIOnProcessMsgEvent::F_ListenerHandle _OnWinAPIProcessMsgEventListenerHandle;
#endif
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> _D3D12CommandQueue = nullptr;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _D3D12DescriptorHeap_SRV = nullptr;
        TF_ConcurrentQueue<F_DirectX12ImGuiSRVDescriptor> _DirectX12SRVDescriptors;
        TF_ConcurrentQueue<F_DirectX12ImGuiPack> _DirectX12Packs;
#endif
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_WIN32
#endif
        
    public:
        struct F_FrameData
        {
            F_Vector2_U32 Size = F_Vector2_U32::Zero();
        } FrameData;

    public:
        ABYTEK_FORCE_INLINE const auto& GetWindow() const noexcept
        {
            return _Window;
        }
        
        ABYTEK_FORCE_INLINE auto GetImGuiContext() const noexcept
        {
            return _ImGuiContext;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetRHIContextProxy() const noexcept
        {
            return _RHIContextProxy;
        }
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
        ABYTEK_FORCE_INLINE const auto& GetD3D12DescriptorHeap_SRV() const noexcept
        {
            return _D3D12DescriptorHeap_SRV; 
        }
        ABYTEK_FORCE_INLINE auto& GetDirectX12SRVDescriptors() noexcept
        {
            return _DirectX12SRVDescriptors;
        }
#endif
        
    public:
        F_ImGuiContext(const F_ImGuiContextBuildParams& BuildParams);
        virtual ~F_ImGuiContext();
        
    public:
         void Release();
        
    public:
        B8 BeginFrame();
        TS<F_ImGuiRenderData> EndFrame();
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
        F_DirectX12ImGuiPack PopDirectX12Pack(const F_DirectX12ImGuiPackPopParams& PopParams);
        void PushDirectX12Pack(F_DirectX12ImGuiPack&& DirectX12Pack);
#endif
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_WIN32
#endif
    };
}
