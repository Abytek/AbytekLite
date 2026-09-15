#pragma once

#include "Abytek/RHIBindGroupTemplateRuntimeProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIBindGroupTemplateRuntimeProxy : public A_RHIBindGroupTemplateRuntimeProxy
    {
    private:
        
    public:
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIBindGroupTemplateRuntimeProxy);
        void LateBuildBindGroupTemplateRuntime_DirectX12();
        virtual void Release() override;
    };
}
#endif