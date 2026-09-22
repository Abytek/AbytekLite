#pragma once

#include "Abytek/RHIReadbackBufferPass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"
#include "Abytek/RHITransientReadbackBuffer.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class A_RHICopyBufferPass;

    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIReadbackBufferPass : public A_RHIReadbackBufferPass, public A_DirectX12RHIPassExtension
    {
    private:
        F_RHITransientReadbackBufferRange_V2 _TransientReadbackBufferRange;

    public:
        ABYTEK_FORCE_INLINE const auto& GetTransientReadbackBufferRange() const noexcept
        {
            return _TransientReadbackBufferRange;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIReadbackBufferPass);
        virtual void Build(const F_RHIReadbackBufferPassBuildParams& BuildParams);
        virtual void Release() override;
        
    protected:
        virtual void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    
    public:
        virtual E_DirectX12RHIPassBatchType GetPassBatchType() override;
    };
}
#endif