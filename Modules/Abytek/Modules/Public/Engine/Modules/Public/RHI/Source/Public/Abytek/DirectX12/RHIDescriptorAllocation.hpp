#pragma once

#include "Abytek/DirectX12/RHIDescriptor.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHIDescriptorManager;
    
    struct ABYTEK_ENGINE_RHI_API F_DirectX12RHIDescriptorAllocation
    {
        TW<F_DirectX12RHIDescriptorManager> Manager;
        F_DirectX12RHIDescriptorIndex BeginOffset = 0;
        F_DirectX12RHIDescriptorIndex EndOffset = 0;
    
        ABYTEK_FORCE_INLINE F_DirectX12RHIDescriptorIndex GetSize() const noexcept
        {
            return EndOffset - BeginOffset;
        }
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return (
                Manager
                && (GetSize() > 0)
            );
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        F_DirectX12RHIDescriptorAllocationProxy ConvertToRange() const noexcept;
    };
}
#endif