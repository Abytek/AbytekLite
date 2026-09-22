#include "Abytek/RHIViewportPresentation.hpp"
#include "Abytek/RHIViewport.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/DirectX12/RHIViewportPresentation.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIViewportPresentation)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIViewportPresentation>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIViewportPresentation::Build(const F_RHIViewportPresentationBuildParams& BuildParams)
    {
        BuildSubmissionItem();
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.Viewport) << "Invalid viewport";
        _Viewport = BuildParams.Viewport;
    }   
    void A_RHIViewportPresentation::Release()
    {
        _Viewport = {};
        A_RHISubmissionItem::Release();
    }

    TW_Valid<A_RHIProcess> A_RHIViewportPresentation::GetProcess() const
    {
        return _Viewport->GetContext()->GetCurrentProcess();
    }
}
