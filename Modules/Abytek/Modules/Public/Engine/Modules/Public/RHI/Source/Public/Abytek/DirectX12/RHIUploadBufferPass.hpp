#pragma once

#include "Abytek/RHIUploadBufferPass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"
#include "Abytek/RHITransientUploadBuffer.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class A_RHICopyBufferPass;

    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIUploadBufferPass : public A_RHIUploadBufferPass, public A_DirectX12RHIPassExtension
    {
    private:
        F_RHITransientUploadBufferRange_V2 _TransientUploadBufferRange;

    public:
        ABYTEK_FORCE_INLINE const auto& GetTransientUploadBufferRange() const noexcept
        {
            return _TransientUploadBufferRange;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIUploadBufferPass);
        virtual void Build(const F_RHIUploadBufferPassBuildParams& BuildParams);
        void Release() override;
        
    protected:
        void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    };
}
#endif