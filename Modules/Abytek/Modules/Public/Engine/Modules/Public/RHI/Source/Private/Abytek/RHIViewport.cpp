#include "Abytek/RHIViewport.hpp"
#include "Abytek/RHIViewportProxy.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIViewport.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIViewport)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIViewport>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHIViewport::Build(const F_RHIViewportBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);

        _Window = BuildParams.Window;
        _Size = BuildParams.Size;

        ABYTEK_ENGINE_RHI_ASSERT(_Window) << "RHI viewport requires a RHI window";
            
        _Format = BuildParams.Format;
        _SampleDesc = BuildParams.SampleDesc;
        _RefreshRate = BuildParams.RefreshRate;

        _NumFrames = BuildParams.NumFrames;
            
        _CurrentBackFrameIndex = 0;
            
        GetProxy().FastCast<A_RHIViewportProxy>()->LateBuildViewport();
    }
    void A_RHIViewport::Release()
    {
        _CurrentBackFrameIndex = {};
            
        _RenderTargetViews = {};
        _Frames = {};
            
        _NumFrames = 2;
            
        _RefreshRate = 0;
        _SampleDesc = F_RHISampleDesc {};
        _Format = E_RHIFormat::NONE;
            
        _Size = F_Vector2_U32::Zero();
        _Window = {};
            
        A_RHIContextChild::Release();
    }

    void A_RHIViewport::Resize(const F_Vector2_U32& NewSize)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
            
        _Size = NewSize;
    }

    TS<A_RHIContextChildProxy> A_RHIViewport::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIViewportProxy>(ABYTEK_WTHIS());
    }
        
    F_RHIViewportScissorConfig A_RHIViewport::GetDefaultViewportScissorConfig() const
    {
        return F_RHIViewportScissorConfig::Make(F_Vector2_F32(_Size));
    }
}
