#include "Abytek/DirectX12/RHIBindGroupTemplateRuntimeProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIBindGroupTemplateRuntimeProxy::LateBuildBindGroupTemplateRuntime_DirectX12()
    {
    }
    void F_DirectX12RHIBindGroupTemplateRuntimeProxy::Release()
    {
        A_RHIBindGroupTemplateRuntimeProxy::Release();
    }
}
#endif