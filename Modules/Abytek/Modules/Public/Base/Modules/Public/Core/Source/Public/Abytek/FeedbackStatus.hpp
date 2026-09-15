#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/CrtUseAllocatorTypedefs.hpp"


namespace Abytek
{
    struct F_FeedbackStatus
    {
        B8 IsSucceeded = false;
        F_Text ErrorMessage;

        static F_FeedbackStatus MakeSucceeded()
        {
            F_FeedbackStatus Result;
            Result.IsSucceeded = true;
            return Result;
        }
        static F_FeedbackStatus MakeFailed(const F_Text& InErrorMessage = {})
        {
            F_FeedbackStatus Result;
            Result.IsSucceeded = false;
            Result.ErrorMessage = InErrorMessage;
            return Result;
        }

        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return IsSucceeded;
        }
        ABYTEK_FORCE_INLINE explicit operator bool() const noexcept
        {
            return IsValid();
        }
    };
}

#define ABYTEK_FEEDBACK_STATUS_CHECK(...) \
    if (auto ABYTEK_GLUE(___ABYTEK_STATUS_CHECK_, ABYTEK_LINE) = __VA_ARGS__; !ABYTEK_GLUE(___ABYTEK_STATUS_CHECK_, ABYTEK_LINE)) \
    { \
        return ABYTEK_GLUE(___ABYTEK_STATUS_CHECK_, ABYTEK_LINE); \
    }

#define ABYTEK_FEEDBACK_STATUS_CHECK_HARD(...) \
    if (auto ABYTEK_GLUE(___ABYTEK_STATUS_CHECK_, ABYTEK_LINE) = __VA_ARGS__; !ABYTEK_GLUE(___ABYTEK_STATUS_CHECK_, ABYTEK_LINE)) \
    { \
        ABYTEK_LOG_FATAL() << ABYTEK_GLUE(___ABYTEK_STATUS_CHECK_, ABYTEK_LINE).ErrorMessage; \
    }