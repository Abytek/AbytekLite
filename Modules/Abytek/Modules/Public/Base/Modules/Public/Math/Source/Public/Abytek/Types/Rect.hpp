/**
 * @file Rect.hpp
 * @brief Rectangle type definitions for the Abytek math system.
 * 
 * This file defines rectangle types that represent axis-aligned 2D rectangles
 * using minimum and maximum coordinates.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Vecma.hpp"


namespace Abytek
{
    /**
     * @struct TF_Rect
     * @brief Template for 2D axis-aligned rectangles.
     * 
     * Defines a rectangle using minimum and maximum points in 2D space.
     * 
     * @tparam __F_Element The scalar type used for rectangle coordinates.
     */
    template<typename __F_Element>
    struct TF_Rect
    {
        using F_Element = __F_Element;
        using F_Vector2 = TF_Vector2<F_Element>;

        F_Vector2 Min;  ///< Minimum point (bottom-left corner)
        F_Vector2 Max;  ///< Maximum point (top-right corner)
        
        /**
         * @brief Calculates the size of the rectangle.
         * @return A vector containing the width and height of the rectangle.
         */
        ABYTEK_FORCE_INLINE F_Vector2 GetSize() const noexcept
        {
            return Max - Min;
        }
        
        ABYTEK_FORCE_INLINE B8 operator == (const TF_Rect& X) const noexcept
        {
            return (
                (Min == X.Min)
                && (Max == X.Max)
            );
        }
        ABYTEK_FORCE_INLINE B8 operator != (const TF_Rect& X) const noexcept
        {
            return (
                (Min != X.Min)
                || (Max != X.Max)
            );
        }
    };
    
    using F_Rect = TF_Rect<I32>;        ///< Integer rectangle type (default)
    using F_Rect_F32 = TF_Rect<F32>;    ///< Floating-point rectangle type
    using F_Rect_I32 = TF_Rect<I32>;    ///< 32-bit signed integer rectangle type
    using F_Rect_U32 = TF_Rect<U32>;    ///< 32-bit unsigned integer rectangle type
    using F_Rect_U16 = TF_Rect<U16>;    ///< 16-bit unsigned integer rectangle type
}