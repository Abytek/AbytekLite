/**
 * @file Base.Core.prerequisites.hpp
 * @brief Core prerequisite includes and namespace declarations
 * 
 * This header provides base includes required by the Core module and
 * establishes the Abytek namespace for Core functionality.
 */
#pragma once

#include "Abytek/Base.Core.config.hpp"
#include "Abytek/Base.Minimal.hpp"


namespace Abytek
{
    namespace WithCrtAllocator
    {}
    using namespace Abytek::WithCrtAllocator;
}