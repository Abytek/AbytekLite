#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHICullMode : U8
    {
        NONE,
        
        FRONT,
        BACK,
        
        DEFAULT = BACK
    };
    enum class E_RHIFillMode : U8
    {
        NONE,
        
        WIREFRAME,
        SOLID,
        
        DEFAULT = SOLID
    };
    enum class E_RHIConservativeRasterMode : U8
    {
        NONE,
        
        ENABLED,
        
        DEFAULT = NONE
    };
    
    struct F_RHIRasterizerConfig
    {
        E_RHICullMode CullMode = E_RHICullMode::DEFAULT;
        E_RHIFillMode FillMode = E_RHIFillMode::DEFAULT;
        B8 FrontCounterClockwise = false;
        I32 DepthBias = 0;
        E_RHIConservativeRasterMode ConservativeRasterMode = E_RHIConservativeRasterMode::DEFAULT;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIRasterizerConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.CullMode);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.FillMode);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.FrontCounterClockwise);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DepthBias);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ConservativeRasterMode);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIRasterizerConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.CullMode);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.FillMode);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.FrontCounterClockwise);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DepthBias);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ConservativeRasterMode);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIRasterizerConfig& A, const F_RHIRasterizerConfig& B)
        {
            return (
                (A.CullMode == B.CullMode)
                && (A.FillMode == B.FillMode)
                && (A.FrontCounterClockwise == B.FrontCounterClockwise)
                && (A.DepthBias == B.DepthBias)
                && (A.ConservativeRasterMode == B.ConservativeRasterMode)
            );
        }
        friend B8 operator != (const F_RHIRasterizerConfig& A, const F_RHIRasterizerConfig& B)
        {
            return !(A == B);
        }
    };
}

namespace boost
{
    template<>
    struct hash<Abytek::F_RHIRasterizerConfig>
    {
        size_t operator()(const Abytek::F_RHIRasterizerConfig& A) const
        {
            size_t Result = 0;
            hash_combine(Result, A.CullMode);
            hash_combine(Result, A.FillMode);
            hash_combine(Result, A.FrontCounterClockwise);
            hash_combine(Result, A.DepthBias);
            hash_combine(Result, A.ConservativeRasterMode);
            return Result;
        }
    };
}