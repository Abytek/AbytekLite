#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIShaderFrequency.hpp"


namespace Abytek
{
    struct F_RHISamplerAccess
    {
        E_RHIShaderFrequency ShaderFrequency = E_RHIShaderFrequency::DEFAULT;

        static F_RHISamplerAccess MakeNone(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                InShaderFrequency
            };
        }
        static F_RHISamplerAccess MakeDefault(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                InShaderFrequency
            };
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHISamplerAccess& Value) noexcept
        {
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHISamplerAccess& Value) noexcept
        {
            Value = {};
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHISamplerAccess& A, const F_RHISamplerAccess& B)
        {
            return (
                (A.ShaderFrequency == B.ShaderFrequency)
            );
        }
        friend B8 operator != (const F_RHISamplerAccess& A, const F_RHISamplerAccess& B)
        {
            return (
                (A.ShaderFrequency != B.ShaderFrequency)
            );
        }
    };
}

namespace boost
{
    template<>
    struct hash<Abytek::F_RHISamplerAccess>
    {
        size_t operator ()(const Abytek::F_RHISamplerAccess& X) const
        {
            size_t Result = 0;
            hash_combine(Result, X.ShaderFrequency);
            return Result;
        }
    };
}