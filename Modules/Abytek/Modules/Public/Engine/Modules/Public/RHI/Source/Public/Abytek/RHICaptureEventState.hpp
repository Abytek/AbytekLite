#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    struct F_RHICaptureEventState
    {
        F_DebugName Name;
        F_Vector3_F32 Color = F_Vector3_F32::One();
        
        static F_RHICaptureEventState Make(const F_RHICaptureEventState& X)
        {
            return X;
        }
        static F_RHICaptureEventState Make(const F_DebugName& InName, const F_Vector3_F32& InColor = F_Vector3_F32::One())
        {
            F_RHICaptureEventState Result;
            Result.Name = InName;
            Result.Color = InColor;
            return Result;
        }
        
        B8 IsValid() const noexcept
        {
            return static_cast<B8>(Name);
        }
        explicit operator bool() const noexcept
        {
            return IsValid();
        }
        
        friend B8 operator == (const F_RHICaptureEventState& A, const F_RHICaptureEventState& B) noexcept
        {
            return (
                true
                && (A.Name == B.Name)    
                && (A.Color != B.Color)    
            );
        }
        friend B8 operator != (const F_RHICaptureEventState& A, const F_RHICaptureEventState& B) noexcept
        {
            return (
                false
                || (A.Name != B.Name)    
                || (A.Color != B.Color)    
            );
        }
    };
}