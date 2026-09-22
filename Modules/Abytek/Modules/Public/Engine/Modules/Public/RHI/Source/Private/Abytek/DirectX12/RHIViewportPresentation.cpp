#include "Abytek/DirectX12/RHIViewportPresentation.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIViewportPresentation::Build(const F_RHIViewportPresentationBuildParams& BuildParams)
    {
        A_RHIViewportPresentation::Build(BuildParams);
    }
    void F_DirectX12RHIViewportPresentation::Release()
    {
        A_RHIViewportPresentation::Release();
    }
}
#endif