/**
 * @file DefaultAllocators.hpp
 * @brief Provides type definitions for default allocators.
 * 
 * This file defines commonly used allocator types for different contexts.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/CrtAllocator.hpp"


namespace Abytek
{
    /**
     * @brief Namespace containing everything using raw Boost allocator.
     */
    namespace WithRawAllocator
    {
        /**
         * @brief Default allocator template that uses the raw Boost allocator.
         * 
         * @tparam __F_Element The element type this allocator works with.
         */
        template<typename __F_Element>
        using TF_DefaultAllocator = boost::default_allocator<__F_Element>;
    }
    
    /**
     * @brief Namespace containing everything using CRT allocator.
     */
    namespace WithCrtAllocator
    {
        /**
         * @brief Default allocator template that uses the Abytek CRT allocator.
         * 
         * @tparam __F_Element The element type this allocator works with.
         */
        template<typename __F_Element>
        using TF_DefaultAllocator = TF_CrtAllocator<__F_Element>;
    }
}