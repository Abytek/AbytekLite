#pragma once

#include "Abytek/Base.Serializable.config.hpp"
#include "Abytek/Base.Core.hpp"
#include "Abytek/Base.Object.hpp"
#include "Abytek/Base.Reflection.hpp"
#include "Abytek/Base.Concurrency.hpp"


namespace Abytek
{
    ABYTEK_BASE_SERIALIZABLE_API void SetupReflectionSession_Base_Serializable(const TW_Valid<F_ReflectionSession>& Session);
}