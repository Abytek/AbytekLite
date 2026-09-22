#pragma once

#include "Abytek/Archive.hpp"
#include "Abytek/BoostContainerArchiveReader.hpp"
#include "Abytek/StringId.hpp"
#include "Abytek/DebugName.hpp"


namespace Abytek
{
    template<class __F_StringIdDatabase>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, TF_StringId<__F_StringIdDatabase>& Value) noexcept
    {
        F_Text Text;
        if (View >> Text)
        {
            Value = Text;
            return F_FeedbackStatus::MakeSucceeded();
        }
        return F_FeedbackStatus::MakeFailed();
    }
}