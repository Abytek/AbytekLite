#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief No-operation lock that performs no actual locking
     * 
     * F_NoLock provides a lock interface that doesn't perform any actual locking,
     * useful for code paths that need the lock interface but don't require synchronization.
     * This can be used as a performance optimization when thread safety is not needed.
     */
    struct F_NoLock
    {
        /**
         * @brief No-op consumer lock
         */
        ABYTEK_FORCE_INLINE void ConsumerLock() noexcept
        {
        }

        /**
         * @brief No-op consumer try lock
         * @return Always returns true
         */
        ABYTEK_FORCE_INLINE constexpr B8 ConsumerTryLock() noexcept
        {
            return true;
        }

        /**
         * @brief No-op consumer unlock
         */
        ABYTEK_FORCE_INLINE void ConsumerUnlock() noexcept
        {
        }

        /**
         * @brief No-op producer lock
         */
        ABYTEK_FORCE_INLINE void ProducerLock() noexcept
        {
        }

        /**
         * @brief No-op producer try lock
         * @return Always returns true
         */
        ABYTEK_FORCE_INLINE constexpr B8 ProducerTryLock() noexcept
        {
            return true;
        }

        /**
         * @brief No-op producer unlock
         */
        ABYTEK_FORCE_INLINE void ProducerUnlock() noexcept
        {
        }
    };
}