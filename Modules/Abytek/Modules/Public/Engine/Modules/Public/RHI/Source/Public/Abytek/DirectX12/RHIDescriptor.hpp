#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHIDescriptorManagerProxy;
    
    using F_DirectX12RHIDescriptorIndex = U32;
    
    struct ABYTEK_ENGINE_RHI_API F_DirectX12RHIDescriptorAllocationProxy
    {
        TW<F_DirectX12RHIDescriptorManagerProxy> ManagerProxy;
        F_DirectX12RHIDescriptorIndex BeginOffset = 0;
        F_DirectX12RHIDescriptorIndex EndOffset = 0;
    
        ABYTEK_FORCE_INLINE F_DirectX12RHIDescriptorIndex GetSize() const noexcept
        {
            return EndOffset - BeginOffset;
        }
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return (
                ManagerProxy
                && (GetSize() > 0)
            );
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(B8 IsShaderVisible) const noexcept;
        D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const noexcept;
    };
}
#endif