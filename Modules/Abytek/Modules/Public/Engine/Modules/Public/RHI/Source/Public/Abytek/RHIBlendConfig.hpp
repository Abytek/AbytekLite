#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHIBlendFactor
    {
        NONE,
        
        ZERO,
        ONE,
        SRC_COLOR,
        INV_SRC_COLOR,
        SRC_ALPHA,
        INV_SRC_ALPHA,
        DEST_ALPHA,
        INV_DEST_ALPHA,
        DEST_COLOR,
        INV_DEST_COLOR,
        SRC_ALPHA_SAT,
        BLEND_FACTOR,
        INV_BLEND_FACTOR,
        SRC1_COLOR,
        INV_SRC1_COLOR,
        SRC1_ALPHA,
        INV_SRC1_ALPHA,
        ALPHA_FACTOR,
        INV_ALPHA_FACTOR
    };
    
    enum class E_RHIBlendOperation
    {
        NONE,
        
        ADD,
        SUBTRACT,
        REV_SUBTRACT,
        MIN,
        MAX
    };
    
    struct F_RHIBlendRenderTargetConfig
    {
        B8 EnableBlend = false;
        
        E_RHIBlendFactor SrcBlendFactorColor;
        E_RHIBlendFactor DestBlendFactorColor;
        E_RHIBlendOperation BlendOperationColor;
        
        E_RHIBlendFactor SrcBlendFactorAlpha;
        E_RHIBlendFactor DestBlendFactorAlpha;
        E_RHIBlendOperation BlendOperationAlpha;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIBlendRenderTargetConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.EnableBlend);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SrcBlendFactorColor);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DestBlendFactorColor);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.BlendOperationColor);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SrcBlendFactorAlpha);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DestBlendFactorAlpha);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.BlendOperationAlpha);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIBlendRenderTargetConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.EnableBlend);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SrcBlendFactorColor);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DestBlendFactorColor);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.BlendOperationColor);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SrcBlendFactorAlpha);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DestBlendFactorAlpha);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.BlendOperationAlpha);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIBlendRenderTargetConfig& A, const F_RHIBlendRenderTargetConfig& B)
        {
            return (
                (A.EnableBlend == B.EnableBlend)
                && (A.SrcBlendFactorColor == B.SrcBlendFactorColor)
                && (A.DestBlendFactorColor == B.DestBlendFactorColor)
                && (A.BlendOperationColor == B.BlendOperationColor)
                && (A.SrcBlendFactorAlpha == B.SrcBlendFactorAlpha)
                && (A.DestBlendFactorAlpha == B.DestBlendFactorAlpha)
                && (A.BlendOperationAlpha == B.BlendOperationAlpha)
            );
        }
        friend B8 operator != (const F_RHIBlendRenderTargetConfig& A, const F_RHIBlendRenderTargetConfig& B)
        {
            return !(A == B);
        }
    };
    
    struct F_RHIBlendConfig
    {
        TF_SmallVector<F_RHIBlendRenderTargetConfig, 8> RenderTargets;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIBlendConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RenderTargets);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIBlendConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RenderTargets);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIBlendConfig& A, const F_RHIBlendConfig& B)
        {
            return (
                (A.RenderTargets == B.RenderTargets)
            );
        }
        friend B8 operator != (const F_RHIBlendConfig& A, const F_RHIBlendConfig& B)
        {
            return !(A == B);
        }
    };
}

namespace boost
{
    template<>
    struct hash<Abytek::F_RHIBlendRenderTargetConfig>
    {
        size_t operator()(const Abytek::F_RHIBlendRenderTargetConfig& A) const
        {
            size_t Result = 0;
            hash_combine(Result, A.EnableBlend);
            hash_combine(Result, A.SrcBlendFactorColor);
            hash_combine(Result, A.DestBlendFactorColor);
            hash_combine(Result, A.BlendOperationColor);
            hash_combine(Result, A.SrcBlendFactorAlpha);
            hash_combine(Result, A.DestBlendFactorAlpha);
            hash_combine(Result, A.BlendOperationAlpha);
            return Result;
        }
    };
    
    template<>
    struct hash<Abytek::F_RHIBlendConfig>
    {
        size_t operator()(const Abytek::F_RHIBlendConfig& A) const
        {
            size_t Result = 0;
            hash_combine(Result, A.RenderTargets);
            return Result;
        }
    };
}

