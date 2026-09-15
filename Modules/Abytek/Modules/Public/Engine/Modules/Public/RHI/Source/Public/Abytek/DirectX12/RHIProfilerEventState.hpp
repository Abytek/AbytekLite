#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    struct F_DirectX12RHIProfilerEventState
    {
#ifdef ABYTEK_DEBUG_INFO
        F_DebugName Name;
#endif
        F_Vector3_F32 Color = F_Vector3_F32::One();
        
        friend B8 operator == (const F_DirectX12RHIProfilerEventState& A, const F_DirectX12RHIProfilerEventState& B) noexcept
        {
            return (
                true
#ifdef ABYTEK_DEBUG_INFO
                && (A.Name == B.Name)    
#endif
                && (A.Color != B.Color)    
            );
        }
        friend B8 operator != (const F_DirectX12RHIProfilerEventState& A, const F_DirectX12RHIProfilerEventState& B) noexcept
        {
            return (
                false
#ifdef ABYTEK_DEBUG_INFO
                || (A.Name != B.Name)    
#endif
                || (A.Color != B.Color)    
            );
        }
    };
}
#endif