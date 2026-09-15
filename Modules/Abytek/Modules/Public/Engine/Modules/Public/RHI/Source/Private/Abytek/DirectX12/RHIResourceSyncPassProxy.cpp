#include "Abytek/DirectX12/RHIResourceSyncPassProxy.hpp"

#include "Abytek/DirectX12/RHIResourceViewProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIResourceSyncPassProxy::Build(const TW_Valid<A_RHIResourceSyncPass>& Pass)
    {
        A_RHIResourceSyncPassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
    }   
    void F_DirectX12RHIResourceSyncPassProxy::Release()
    {
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIResourceSyncPassProxy::Release();
    }
}
#endif