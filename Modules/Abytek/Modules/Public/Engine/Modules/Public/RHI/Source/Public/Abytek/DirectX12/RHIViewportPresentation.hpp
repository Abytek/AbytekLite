#pragma once

#include "Abytek/RHIViewportPresentation.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIViewportPresentation : public A_RHIViewportPresentation
    {
    private:

    public:
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIViewportPresentation);
        void Build(const F_RHIViewportPresentationBuildParams& BuildParams) override;
        void Release() override;
    };
}
#endif