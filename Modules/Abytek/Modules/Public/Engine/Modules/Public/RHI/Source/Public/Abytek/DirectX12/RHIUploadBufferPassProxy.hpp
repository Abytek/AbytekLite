#pragma once

#include "Abytek/RHIUploadBufferPassProxy.hpp"
#include "Abytek/DirectX12/RHIPassProxyExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIUploadBufferPassProxy : public A_RHIUploadBufferPassProxy, public A_DirectX12RHIPassProxyExtension
    {
    private:
        TS<A_RHIResourceProxy> _UploadBufferProxy;
        U64 _UploadBufferOffsetInBytes = 0;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetUploadBufferProxy() const noexcept
        {
            return _UploadBufferProxy;
        }
        ABYTEK_FORCE_INLINE auto GetUploadBufferOffsetInBytes() const noexcept
        {
            return _UploadBufferOffsetInBytes;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIUploadBufferPassProxy);
        virtual void Build(const TW_Valid<A_RHIUploadBufferPass>& Pass) override;
        virtual void Release() override;
        
    public:
        virtual void Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams) override;
    };
}
#endif