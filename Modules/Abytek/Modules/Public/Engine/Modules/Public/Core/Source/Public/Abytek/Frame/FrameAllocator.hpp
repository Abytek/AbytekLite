#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/Frame/FrameCommon.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    template<typename __F_Value, E_FrameParamType __DefaultFrameParamType = E_FrameParamType::DEFAULT>
    struct TF_FrameAllocator :
        TF_InlineAllocator<__F_Value>
    {
        ABYTEK_DEFINE_ALLOCATOR_REBIND(
            __F_OtherValue,
            TF_FrameAllocator<__F_OtherValue, __DefaultFrameParamType>
        );
        
        static constexpr E_FrameParamType DefaultFrameParamType = __DefaultFrameParamType;
        
        TF_FrameAllocator(E_FrameParamType FrameParamType = DefaultFrameParamType) noexcept :
            TF_InlineAllocator<__F_Value>(
                &(H_Frame::GetArena(FrameParamType)->Heap)
            )
        {}

        TF_FrameAllocator(const TF_FrameAllocator&) noexcept = default;
        TF_FrameAllocator& operator = (const TF_FrameAllocator&) noexcept = default;

        template<typename __F_OtherValue, E_FrameParamType __OtherDefaultFrameParamType>
        TF_FrameAllocator(const TF_FrameAllocator<__F_OtherValue, __OtherDefaultFrameParamType>& X) noexcept :
            TF_InlineAllocator<__F_Value>(X)
        {}
        template<typename __F_OtherValue, E_FrameParamType __OtherDefaultFrameParamType>
        TF_FrameAllocator& operator = (const TF_FrameAllocator<__F_OtherValue, __OtherDefaultFrameParamType>& X) noexcept
        {
            static_cast<TF_InlineAllocator<__F_Value>&>(*this) = X;
            return *this;
        }
    };
}