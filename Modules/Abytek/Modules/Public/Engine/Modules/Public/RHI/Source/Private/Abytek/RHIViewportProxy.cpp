#include "Abytek/RHIViewportProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/RHIViewport.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIResourceView.hpp"
#include "Abytek/DirectX12/RHIViewportProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIViewportProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIViewportProxy>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()

    void A_RHIViewportProxy::LateBuildViewport()
    { 
        auto Viewport = GetViewport();

        _Window = Viewport->GetWindow();
        _Size = Viewport->GetSize();

        ABYTEK_ENGINE_RHI_ASSERT(_Window) << "RHI viewport requires a RHI window";
        
        _Format = Viewport->GetFormat();
        _SampleDesc = Viewport->GetSampleDesc();
        _RefreshRate = Viewport->GetRefreshRate();

        _NumFrames = Viewport->GetNumFrames();
        
        _CurrentBackFrameIndex = 0;
    }
    void A_RHIViewportProxy::Release()
    {
        _CurrentBackFrameIndex = {};
        
        _RenderTargetViewProxies = {};
        _FrameProxies = {};
        
        _NumFrames = 2;
        
        _RefreshRate = 0;
        _SampleDesc = F_RHISampleDesc {};
        _Format = E_RHIFormat::NONE;
        
        _Size = F_Vector2_U32::Zero();
        _Window = {};
            
        A_RHIContextChildProxy::Release();
    }
}
