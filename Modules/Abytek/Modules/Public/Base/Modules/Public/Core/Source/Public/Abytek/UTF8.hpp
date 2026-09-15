#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    struct ABYTEK_BASE_CORE_API H_UTF8
    {
        static boost::container::vector<F_Char> From(const F_TextChar& TextChar);
        static boost::container::vector<F_Char> From(const boost::container::vector<F_TextChar>& Text);
        static boost::container::vector<F_TextChar> ToText(const F_Char& Char);
        static boost::container::vector<F_TextChar> ToText(const boost::container::vector<F_Char>& String);
    };
}