#include "Abytek/DirectX12/RHIDescriptorManager.hpp"
#include "Abytek/DirectX12/RHIDescriptorManagerProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHIDescriptorManager);
    void F_DirectX12RHIDescriptorManager::Build(const F_DirectX12RHIDescriptorManagerBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _DescriptorHeapType = BuildParams.DescriptorHeapType;
        _IsShaderVisible = BuildParams.IsShaderVisible;
        
        GetProxy().FastCast<F_DirectX12RHIDescriptorManagerProxy>()->LateBuildDescriptorManager_DirectX12();
    }
    void F_DirectX12RHIDescriptorManager::Release()
    {
        _Distributor = {};
        _IsShaderVisible = false;
        _DescriptorHeapType = DirectX12SharedAPIWrapper::E_DescriptorHeapType::NONE;
        A_RHIContextChild::Release();
    }

    F_DirectX12RHIDescriptorAllocation F_DirectX12RHIDescriptorManager::Allocate(U32 Num)
    {
        F_DirectX12RHIDescriptorAllocation Result;
        Result.Manager = ABYTEK_WTHIS();
        Result.BeginOffset = *_Distributor.Allocate(Num);
        Result.EndOffset = Result.BeginOffset + Num;
        return Result;
    }
    void F_DirectX12RHIDescriptorManager::Deallocate(const F_DirectX12RHIDescriptorAllocation& Allocation)
    {
        _Distributor.Deallocate(Allocation.BeginOffset);
    }

    TS<A_RHIContextChildProxy> F_DirectX12RHIDescriptorManager::CreateProxy()
    {
        return RACreateAndBuildShared<F_DirectX12RHIDescriptorManagerProxy>(ABYTEK_WTHIS());
    }
}
#endif