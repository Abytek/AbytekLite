#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"


namespace Abytek
{
    using F_GeneralTypeHashCode = U64;
    struct H_GeneralTypeHashCode
    {
        static F_Name MakeStaticCategory()
        {
            return {};
        }
        static constexpr F_GeneralTypeHashCode MakeInvalid()
        {
            return HashCombineU64(HashStringView(ABYTEK_TEXT("INVALID")), 0);
        }
        static constexpr F_GeneralTypeHashCode MakeStatic(F_GeneralTypeHashCode StaticHashCode)
        {
            return HashCombineU64(F_Name::INVALID_STRING_HASH_CODE, StaticHashCode);
        }
        template<typename __F>
        static constexpr F_GeneralTypeHashCode MakeStatic()
        {
            return MakeStatic(TypeHashCode<__F>);
        }
        template<typename __F>
        static constexpr F_GeneralTypeHashCode StaticValue = MakeStatic(TypeHashCode<__F>);
        static F_GeneralTypeHashCode Make(const F_Name& Category, F_GeneralTypeHashCode RawHashCode)
        {
            if (!Category)
            {
                return MakeStatic(RawHashCode);
            }
            return HashCombineU64(Category.GetHashCode(), RawHashCode);
        }
    };
}