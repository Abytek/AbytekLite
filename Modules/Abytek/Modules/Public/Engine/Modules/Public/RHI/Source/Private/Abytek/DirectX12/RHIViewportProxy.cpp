#include "Abytek/DirectX12/RHIViewportProxy.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIResourceViewProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHICommandQueueProxy.hpp"
#include "Abytek/DirectX12/RHIContextProxy.hpp" 
#include "Abytek/DirectX12/RHIFormat.hpp"
#include "Abytek/DirectX12/RHIProcess.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"
#include "Abytek/Windows/Window.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIViewportProxy::LateBuildViewport_DirectX12()
    {
        auto Viewport = GetViewport();
        for (const auto& Frame : Viewport->GetFrames())
        {
            InjectFrameProxies().push_back(Frame->GetProxy().FastCast<A_RHIResourceProxy>());
        } 
        for (const auto& RTV : Viewport->GetRenderTargetViews())
        {
            InjectRenderTargetViewProxies().push_back(RTV->GetProxy().FastCast<A_RHIResourceViewProxy>());
        }
    }
    void F_DirectX12RHIViewportProxy::Release()
    {
        // Sync
        LockDXGISwapChain();
        UnlockDXGISwapChain();
        
        if (_DXGISwapChain)
        {
            _DXGISwapChain->Release();
            _DXGISwapChain = nullptr;
        }
        A_RHIViewportProxy::Release();
    }

    void F_DirectX12RHIViewportProxy::InitDXGISwapchain()
    {
        LockDXGISwapChain();
        
        auto RHISubsystemProxy = A_RHISubsystemProxy::GetInstance().FastCast<F_DirectX12RHISubsystemProxy>();
        
        auto ContextProxy = GetContextProxy().FastCast<F_DirectX12RHIContextProxy>();
        auto CommandQueueProxy = ContextProxy->GetCommandQueueProxy_DIRECT();
        auto D3D12CommandQueue = CommandQueueProxy->GetD3D12CommandQueue();

        U32 RefreshRate = GetRefreshRate();
        if(RefreshRate == 0)
        {
            RefreshRate = 10000;
        }
        
        auto Size = GetSize();
        
        auto CastedWindow = GetWindow().FastCast<F_WindowsWindow>();

        DXGI_SWAP_CHAIN_DESC DXGISwapChainDesc = {};
        DXGISwapChainDesc.BufferDesc.Width = Size.X;
        DXGISwapChainDesc.BufferDesc.Height = Size.Y;
        DXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = RefreshRate;
        DXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        DXGISwapChainDesc.BufferDesc.Format = RHIFormatToD3DFormat(GetFormat());
        DXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        DXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        DXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        DXGISwapChainDesc.BufferCount = GetNumFrames();
        DXGISwapChainDesc.OutputWindow = CastedWindow->GetWinAPIHandle();
        DXGISwapChainDesc.SampleDesc.Count = GetSampleDesc().Count;
        DXGISwapChainDesc.SampleDesc.Quality = GetSampleDesc().Quality;
        DXGISwapChainDesc.Windowed = true;
        DXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        DXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
        
        HRESULT HR = RHISubsystemProxy->GetDXGIFactory()->CreateSwapChain(
            D3D12CommandQueue.Get(),
            &DXGISwapChainDesc,
            (IDXGISwapChain**)&_DXGISwapChain
        );
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "can't create swapchain";
        
        _LateSetupDXGISwapchain();
        _AssignViewportFrames();
        UnlockDXGISwapChain();
    }
    void F_DirectX12RHIViewportProxy::ResizeDXGISwapchain(F_Vector2_U32 NewSize)
    {
        LockDXGISwapChain();
        
        InjectSize() = NewSize;
        
        for (const auto& FrameProxy : GetFrameProxies())
        {
            FrameProxy.FastCast<F_DirectX12RHIResourceProxy>()->ReleaseD3D12Resource();
        }
        
        HRESULT HR = _DXGISwapChain->ResizeBuffers(
            GetNumFrames(),
            NewSize.X,
            NewSize.Y,
            RHIFormatToD3DFormat(GetFormat()),
            DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING
        );
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "can't resize swapchain";
        
        _LateSetupDXGISwapchain();
        _AssignViewportFrames();
        UnlockDXGISwapChain();
    }
    
    void F_DirectX12RHIViewportProxy::_LateSetupDXGISwapchain()
    {
    }
    void F_DirectX12RHIViewportProxy::_AssignViewportFrames()
    {
        auto CastedCurrentProcess = GetContextProxy()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>();
        
        U32 NumFrames = GetNumFrames();
        for (U32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
        {
            DirectX12RHIProcessQueries::Execution::F_AssignViewportFrame Query;
            Query.ViewportProxy = ABYTEK_STHIS();
            Query.ResourceProxy = GetFrameProxies()[FrameIndex];
            Query.FrameIndex = FrameIndex;
            CastedCurrentProcess->Queues.Execution.AssignViewportFrame.Push(Query);
        }
    }

    void F_DirectX12RHIViewportProxy::LockDXGISwapChain()
    {
        _DXGISwapChainLock.Lock();
    }
    void F_DirectX12RHIViewportProxy::UnlockDXGISwapChain()
    {
        _DXGISwapChainLock.Unlock();
    }
}
#endif