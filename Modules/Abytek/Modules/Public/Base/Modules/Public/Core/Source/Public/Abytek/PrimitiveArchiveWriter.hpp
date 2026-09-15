#pragma once

#include "Abytek/Archive.hpp"


namespace Abytek
{
    template<typename __F, std::enable_if_t<std::is_enum_v<__F>, bool> = true>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, __F Value) noexcept
    {
        View.Access<__F>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const B8& Value) noexcept
    {
        View.Access<B8>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const U8& Value) noexcept
    {
        View.Access<U8>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const U16& Value) noexcept
    {
        View.Access<U16>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const U32& Value) noexcept
    {
        View.Access<U32>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const U64& Value) noexcept
    {
        View.Access<U64>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const I8& Value) noexcept
    {
        View.Access<I8>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const I16& Value) noexcept
    {
        View.Access<I16>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const I32& Value) noexcept
    {
        View.Access<I32>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const I64& Value) noexcept
    {
        View.Access<I64>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (
        F_ArchiveReadWriteView& View,
        const F_SzForSpecifications& Value
    ) noexcept
    {
        View.Access<F_SzForSpecifications>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (
        F_ArchiveReadWriteView& View,
        const F_PDiffForSpecifications& Value
    ) noexcept
    {
        View.Access<F_PDiffForSpecifications>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F32& Value) noexcept
    {
        View.Access<F32>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
    inline F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F64& Value) noexcept
    {
        View.Access<F64>() = Value;
        return F_FeedbackStatus::MakeSucceeded();
    }
}