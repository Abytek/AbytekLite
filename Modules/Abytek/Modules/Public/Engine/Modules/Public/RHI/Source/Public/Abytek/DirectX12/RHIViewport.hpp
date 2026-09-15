#pragma once

#include "Abytek/RHIViewport.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIViewport : public A_RHIViewport
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIViewport);
        virtual void Build(const F_RHIViewportBuildParams& BuildParams) override;
        virtual void Release() override;
        
    public:
        void Update();
        
    public:
        virtual void Resize(const F_Vector2_U32& NewSize) override;
    };
}
#endif