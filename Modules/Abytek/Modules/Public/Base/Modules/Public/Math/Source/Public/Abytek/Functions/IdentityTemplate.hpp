/**
 * @file IdentityTemplate.hpp
 * @brief Template for identity functions
 * 
 * Provides a template declaration for identity functions that return
 * the identity element for a given type. Specific implementations are 
 * provided in other files for different types.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Template function that returns the identity element for a given type
     * 
     * This template function needs to be specialized for each type that requires
     * an identity element. The identity element is a value that, when used with
     * the type's primary operation, leaves other elements unchanged.
     * 
     * For example:
     * - For numeric types with addition: 0
     * - For numeric types with multiplication: 1
     * - For matrices: Identity matrix
     * - For quaternions: Identity quaternion (0,0,0,1)
     * 
     * @tparam __F The type for which to return the identity element
     * @return The identity element for the specified type
     */
    template<typename __F>
    __F Identity() noexcept; 
}