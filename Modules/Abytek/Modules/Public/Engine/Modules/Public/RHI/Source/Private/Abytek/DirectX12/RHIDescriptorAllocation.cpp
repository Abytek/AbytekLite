#include "Abytek/DirectX12/RHIDescriptorAllocation.hpp"
#include "Abytek/DirectX12/RHIDescriptorManager.hpp"
#include "Abytek/DirectX12/RHIDescriptorManagerProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    F_DirectX12RHIDescriptorAllocationProxy F_DirectX12RHIDescriptorAllocation::ConvertToRange() const noexcept
    {
        F_DirectX12RHIDescriptorAllocationProxy Result;
        Result.ManagerProxy = Manager->GetProxy().FastCast<F_DirectX12RHIDescriptorManagerProxy>().Weak();
        Result.BeginOffset = BeginOffset;
        Result.EndOffset = EndOffset;
        return Result;
    }
}
#endif