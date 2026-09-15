#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/Assert.hpp"


namespace Abytek
{
    /**
     * @brief Single Producer Single Consumer spin lock implementation
     *
     * F_SPSCSpinLock provides the simplest form of synchronization between
     * a single producer and a single consumer. It uses atomic operations
     * to coordinate access to shared resources without blocking threads.
     */
    struct F_SPSCSpinLock
    {
        F_AtomicFlag IsLocked;

        /**
         * @brief Acquires lock from consumer side
         * 
         * Blocks until the lock can be acquired by the consumer.
         */
        void ConsumerLock() noexcept
        {
            while(IsLocked.test_and_set(boost::memory_order_acquire));
        }
        
        /**
         * @brief Tries to acquire lock from consumer side without blocking
         * 
         * @return True if the lock was acquired, false otherwise
         */
        B8 ConsumerTryLock() noexcept
        {
            return !IsLocked.test_and_set(boost::memory_order_acquire);
        }
        
        /**
         * @brief Releases lock from consumer side
         */
        void ConsumerUnlock() noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(IsLocked.test(boost::memory_order_acquire));

            IsLocked.clear(boost::memory_order_release);
        }

        /**
         * @brief Acquires lock from producer side
         * 
         * Blocks until the lock can be acquired by the producer.
         */
        void ProducerLock() noexcept
        {
            while(IsLocked.test_and_set(boost::memory_order_acquire));
        }
        
        /**
         * @brief Tries to acquire lock from producer side without blocking
         * 
         * @return True if the lock was acquired, false otherwise
         */
        B8 ProducerTryLock() noexcept
        {
            return !IsLocked.test_and_set(boost::memory_order_acquire);
        }
        
        /**
         * @brief Releases lock from producer side
         */
        void ProducerUnlock() noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(IsLocked.test(boost::memory_order_acquire));

            IsLocked.clear(boost::memory_order_release);
        }
    };
}