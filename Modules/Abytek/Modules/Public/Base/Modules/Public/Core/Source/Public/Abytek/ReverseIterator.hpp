/**
 * @file ReverseIterator.hpp
 * @brief Reverse iterator implementation
 * 
 * This file provides a reverse iterator implementation that can wrap any forward iterator
 * and make it iterate in the reverse direction. It overloads operators to make this transparent.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/LogStream.hpp"


namespace Abytek
{
    /**
     * @brief Reverse iterator class that adapts a forward iterator to iterate backwards
     * @tparam __F_Iterator The underlying iterator type to reverse
     */
    template<typename __F_Iterator>
    class TF_ReverseIterator;
    
    /**
     * @brief Creates a reverse iterator from a forward iterator
     * @tparam __F_Iterator The underlying iterator type
     * @param Iterator The forward iterator to reverse
     * @return Reverse iterator wrapping the provided iterator
     */
    template<typename __F_Iterator>
    TF_ReverseIterator<__F_Iterator> MakeReverseIterator(const __F_Iterator& Iterator) noexcept;

    /**
     * @brief Flag type to indicate reverse iterator creation
     * Used internally for constructor overloading
     */
    struct F_ReverseIteratorCreationFlag {}; 
    
    /**
     * @brief Reverse iterator implementation
     * @tparam __F_Iterator The underlying iterator type to reverse
     * 
     * This class wraps a forward iterator and reverses its behavior by
     * overloading the increment/decrement operators to do the opposite operation
     * on the underlying iterator.
     */
    template<typename __F_Iterator>
    class TF_ReverseIterator : public __F_Iterator 
    {
    public:
        template<typename __F_Iterator2>
        friend TF_ReverseIterator<__F_Iterator2> MakeReverseIterator(const __F_Iterator2& Iterator) noexcept;
        
        /// @brief Type of the underlying iterator
        using F_Iterator = __F_Iterator;

        /**
         * @brief Default constructor
         */
        ABYTEK_FORCE_INLINE TF_ReverseIterator() noexcept = default;

        /**
         * @brief Constructs a reverse iterator from a forward iterator
         * @param Flag Reverse iterator creation flag (for disambiguation)
         * @param X The forward iterator to reverse
         */
        ABYTEK_FORCE_INLINE TF_ReverseIterator(F_ReverseIteratorCreationFlag, const F_Iterator& X) noexcept :
            __F_Iterator(X - 1)
        {
        }

        /**
         * @brief Copy constructor
         * @param X Another reverse iterator to copy
         */
        ABYTEK_FORCE_INLINE TF_ReverseIterator(const TF_ReverseIterator& X) noexcept :
            __F_Iterator(X)
        {
        }
        
        /**
         * @brief Copy assignment operator
         * @param X Another reverse iterator to copy
         * @return Reference to this
         */
        ABYTEK_FORCE_INLINE TF_ReverseIterator& operator = (const TF_ReverseIterator& X) noexcept
        {
            *((__F_Iterator*)this) = X;
            return *this;
        }

        /**
         * @brief Move constructor
         * @param X Another reverse iterator to move from
         */
        ABYTEK_FORCE_INLINE TF_ReverseIterator(TF_ReverseIterator&& X) noexcept :
            __F_Iterator(boost::move(X))
        {
        }
        
        /**
         * @brief Move assignment operator
         * @param X Another reverse iterator to move from
         * @return Reference to this
         */
        ABYTEK_FORCE_INLINE TF_ReverseIterator& operator = (TF_ReverseIterator&& X) noexcept
        {
            *((__F_Iterator*)this) = boost::move(X);
            return *this;
        }
        
        /**
         * @brief Pre-increment operator
         * @return Reference to this
         * 
         * Note: Pre-increments the reverse iterator, which decrements the underlying iterator
         */
        ABYTEK_FORCE_INLINE TF_ReverseIterator& operator ++ () noexcept
        {
            --*((__F_Iterator*)this);
            return *this;
        }
        
        /**
         * @brief Pre-decrement operator
         * @return Reference to this
         * 
         * Note: Pre-decrements the reverse iterator, which increments the underlying iterator
         */
        ABYTEK_FORCE_INLINE TF_ReverseIterator& operator -- () noexcept
        {
            ++*((__F_Iterator*)this);
            return *this;
        }
        
        /**
         * @brief Addition assignment operator
         * @param Num Number to add
         * @return Reference to this
         * 
         * Note: Adds to the reverse iterator, which subtracts from the underlying iterator
         */
        ABYTEK_FORCE_INLINE TF_ReverseIterator& operator += (PDiff Num) noexcept
        {
            *((__F_Iterator*)this) -= Num;
            return *this;
        }
        
        /**
         * @brief Subtraction assignment operator
         * @param Num Number to subtract
         * @return Reference to this
         * 
         * Note: Subtracts from the reverse iterator, which adds to the underlying iterator
         */
        ABYTEK_FORCE_INLINE TF_ReverseIterator& operator -= (PDiff Num) noexcept
        {
            *((__F_Iterator*)this) += Num;
            return *this;
        }
        
        /**
         * @brief Addition operator
         * @param Num Number to add
         * @return New iterator
         * 
         * Note: Adds to the reverse iterator, which subtracts from the underlying iterator
         */
        ABYTEK_FORCE_INLINE TF_ReverseIterator operator + (PDiff Num) const noexcept
        {
            return { (*((__F_Iterator*)this)) + Num };
        }
        
        /**
         * @brief Subtraction operator
         * @param Num Number to subtract
         * @return New iterator
         * 
         * Note: Subtracts from the reverse iterator, which adds to the underlying iterator
         */
        ABYTEK_FORCE_INLINE TF_ReverseIterator operator - (PDiff Num) const noexcept
        {
            return { (*((__F_Iterator*)this)) - Num };
        }
    };
    
    /**
     * @brief Creates a reverse iterator from a forward iterator
     * @tparam __F_Iterator The underlying iterator type
     * @param Iterator The forward iterator to reverse
     * @return Reverse iterator wrapping the provided iterator
     */
    template<typename __F_Iterator>
    TF_ReverseIterator<__F_Iterator> MakeReverseIterator(const __F_Iterator& Iterator) noexcept
    {
        return {
            F_ReverseIteratorCreationFlag {},
            Iterator
        };
    }
}