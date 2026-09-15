#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHICompareOperation.hpp"
#include "Abytek/RHIFormat.hpp"


namespace Abytek
{
    struct F_RHIDepthStencilConfig
    {
        B8 EnableDepthTest = false;
        E_RHICompareOperation DepthCompareOperation = E_RHICompareOperation::GREATER;
        B8 DepthBufferWrite = true;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIDepthStencilConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.EnableDepthTest);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DepthCompareOperation);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DepthBufferWrite);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIDepthStencilConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.EnableDepthTest);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DepthCompareOperation);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DepthBufferWrite);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIDepthStencilConfig& A, const F_RHIDepthStencilConfig& B)
        {
            return (
                (A.EnableDepthTest == B.EnableDepthTest)
                && (A.DepthCompareOperation == B.DepthCompareOperation)
                && (A.DepthBufferWrite == B.DepthBufferWrite)
            );
        }
        friend B8 operator != (const F_RHIDepthStencilConfig& A, const F_RHIDepthStencilConfig& B)
        {
            return !(A == B);
        }
    };
}

namespace boost
{
    template<>
    struct hash<Abytek::F_RHIDepthStencilConfig>
    {
        size_t operator()(const Abytek::F_RHIDepthStencilConfig& A) const
        {
            size_t Result = 0;
            hash_combine(Result, A.EnableDepthTest);
            hash_combine(Result, A.DepthCompareOperation);
            hash_combine(Result, A.DepthBufferWrite);
            return Result;
        }
    };
}
