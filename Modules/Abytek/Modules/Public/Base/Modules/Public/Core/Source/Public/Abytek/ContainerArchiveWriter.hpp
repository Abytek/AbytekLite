#pragma once

#include "Abytek/Archive.hpp"
#include "Abytek/BoostContainerArchiveWriter.hpp"
#include "Abytek/StringId.hpp"
#include "Abytek/DebugName.hpp"


namespace Abytek
{
    template<class __F_StringIdDatabase>
    inline F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_StringId<__F_StringIdDatabase>& Value) noexcept
    {
        return (View << *Value);
    }
}