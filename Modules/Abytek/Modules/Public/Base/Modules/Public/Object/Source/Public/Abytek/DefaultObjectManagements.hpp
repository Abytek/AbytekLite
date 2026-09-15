/**
 * @file DefaultObjectManagements.hpp
 * @brief Defines default object management configurations for different allocator types.
 * 
 * This file establishes the default object management strategies based on the standard
 * object management implementation, adapted for different allocator contexts.
 */
#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/StandardObjectManagement.hpp"


namespace Abytek
{
    /**
     * @brief Default object management configuration when using the raw allocator.
     */
    namespace WithRawAllocator
    {
        using F_DefaultObjectManagement = F_StandardObjectManagement;
    }
    /**
     * @brief Default object management configuration when using the CRT allocator.
     */
    namespace WithCrtAllocator
    {
        using F_DefaultObjectManagement = F_StandardObjectManagement;
    }
}