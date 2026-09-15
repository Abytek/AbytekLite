#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    /**
     * @brief Forward declarations for sample description structures
     */
    struct F_RHISampleDescBase;
    struct F_RHISampleDesc;
    
    /**
     * @brief Base structure for multisampling description
     * 
     * Defines multisampling parameters with count and quality settings
     */
    struct F_RHISampleDescBase
    {
        /** @brief Number of samples per pixel */
        U32 Count;
        
        /** @brief Quality level of multisampling */
        U32 Quality;

        F_RHISampleDescBase() = default;
        
        /**
         * @brief Constructs a sample description with specified count and quality
         * @param InCount Number of samples per pixel
         * @param InQuality Quality level of multisampling
         */
        F_RHISampleDescBase(U32 InCount, U32 InQuality) noexcept :
            Count(InCount),
            Quality(InQuality)
        {}
        F_RHISampleDescBase(const F_RHISampleDescBase&) = default;
        F_RHISampleDescBase& operator = (const F_RHISampleDescBase&) = default;
    };
    
    /**
     * @brief Functional RHI sample description with default initialization
     * 
     * Extends A_RHISampleDesc with default values and conversion capabilities
     */
    struct F_RHISampleDesc : F_RHISampleDescBase
    {
        /**
         * @brief Default constructor initializes to 1 sample with 0 quality
         */
        F_RHISampleDesc() :
            F_RHISampleDescBase(1, 0)
        {
        }
        
        /**
         * @brief Constructs with specified count and quality
         * @param InCount Number of samples per pixel
         * @param InQuality Quality level of multisampling
         */
        F_RHISampleDesc(U32 InCount, U32 InQuality) noexcept :
            F_RHISampleDescBase(InCount, InQuality)
        {}
        F_RHISampleDesc(const F_RHISampleDesc&) = default;
        F_RHISampleDesc& operator = (const F_RHISampleDesc&) = default;
        
        /**
         * @brief Conversion constructor from base class
         * @param X Source sample description
         */
        F_RHISampleDesc(const F_RHISampleDescBase& X) :
            F_RHISampleDescBase(X)
        {
        }
        
        /**
         * @brief Assignment operator from base class
         * @param X Source sample description
         * @return Reference to this object
         */
        F_RHISampleDesc& operator = (const F_RHISampleDescBase& X) 
        {
            Count = X.Count;
            Quality = X.Quality;
            return *this;
        }
    };
}
