#pragma once

#include "Abytek/RHIBindGroupTemplateRuntime.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIBindGroupTemplateRuntime : public A_RHIBindGroupTemplateRuntime
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIBindGroupTemplateRuntime);
        virtual void Build(const F_RHIBindGroupTemplateRuntimeBuildParams& BuildParams) override;
        virtual void Release() override;
        
    public:
        virtual void FinalizeActivation() override;
    };
}
#endif