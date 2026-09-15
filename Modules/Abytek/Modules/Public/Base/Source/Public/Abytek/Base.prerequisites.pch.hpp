#pragma once

#include "Abytek/Base.config.hpp"
#include "Abytek/Base.Minimal.hpp"
#include "Abytek/Base.Core.hpp"
#include "Abytek/Base.Platform.hpp"
#include "Abytek/Base.Math.hpp"
#include "Abytek/Base.Object.hpp"
#include "Abytek/Base.Reflection.hpp"
#include "Abytek/Base.PureDOD.hpp"
#include "Abytek/Base.Serializable.hpp"
#include "Abytek/Base.Runtime.hpp"
#include "Abytek/Base.Concurrency.hpp"


namespace Abytek 
{
    ABYTEK_BASE_API void SetupReflectionSession_Base(const TW_Valid<F_ReflectionSession>& Session);
}