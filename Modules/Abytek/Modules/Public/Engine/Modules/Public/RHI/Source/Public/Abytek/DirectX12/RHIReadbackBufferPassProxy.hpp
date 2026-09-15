#pragma once

#include "Abytek/RHIReadbackBufferPassProxy.hpp"
#include "Abytek/DirectX12/RHIPassProxyExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIReadbackBufferPassProxy : public A_RHIReadbackBufferPassProxy, public A_DirectX12RHIPassProxyExtension
    {
    private:
        TS<A_RHIResourceProxy> _ReadbackBufferProxy;
        U64 _ReadbackBufferOffsetInBytes = 0;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetReadbackBufferProxy() const noexcept
        {
            return _ReadbackBufferProxy;
        }
        ABYTEK_FORCE_INLINE auto GetReadbackBufferOffsetInBytes() const noexcept
        {
            return _ReadbackBufferOffsetInBytes;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIReadbackBufferPassProxy);
        virtual void Build(const TW_Valid<A_RHIReadbackBufferPass>& Pass) override;
        virtual void Release() override;
        
    public:
        virtual void Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams) override;
    };
}
#endif