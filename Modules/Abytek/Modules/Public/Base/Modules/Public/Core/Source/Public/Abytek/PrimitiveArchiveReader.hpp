#pragma once

#include "Abytek/Archive.hpp"


namespace Abytek
{
    template<typename __F, std::enable_if_t<std::is_enum_v<__F>, bool> = true>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, __F& Value) noexcept
    {
        if (View.CheckSize<__F>())
        {
            Value = View.Access<__F>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, B8& Value) noexcept
    {
        if (View.CheckSize<B8>())
        {
            Value = View.Access<B8>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, U8& Value) noexcept
    {
        if (View.CheckSize<U8>())
        {
            Value = View.Access<U8>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, U16& Value) noexcept
    {
        if (View.CheckSize<U16>())
        {
            Value = View.Access<U16>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, U32& Value) noexcept
    {
        if (View.CheckSize<U32>())
        {
            Value = View.Access<U32>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, U64& Value) noexcept
    {
        if (View.CheckSize<U64>())
        {
            Value = View.Access<U64>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, I8& Value) noexcept
    {
        if (View.CheckSize<I8>())
        {
            Value = View.Access<I8>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, I16& Value) noexcept
    {
        if (View.CheckSize<I16>())
        {
            Value = View.Access<I16>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, I32& Value) noexcept
    {
        if (View.CheckSize<I32>())
        {
            Value = View.Access<I32>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, I64& Value) noexcept
    {
        if (View.CheckSize<I64>())
        {
            Value = View.Access<I64>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (
        F_ArchiveReadOnlyView& View, 
        F_SzForSpecifications& Value
    ) noexcept
    {
        if (View.CheckSize<Sz>())
        {
            Value = View.Access<F_SzForSpecifications>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (
        F_ArchiveReadOnlyView& View, 
        F_PDiffForSpecifications& Value
    ) noexcept
    {
        if (View.CheckSize<PDiff>())
        {
            Value = View.Access<F_PDiffForSpecifications>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F32& Value) noexcept
    {
        if (View.CheckSize<F32>())
        {
            Value = View.Access<F32>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
    inline F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F64& Value) noexcept
    {
        if (View.CheckSize<F64>())
        {
            Value = View.Access<F64>();
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
}