#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Multiple Producer Single Consumer spin lock implementation
     *
     * F_MPSCSpinLock provides synchronization between multiple producers 
     * and a single consumer. It uses atomic operations to coordinate 
     * access to shared resources without blocking threads.
     */
    struct F_MPSCSpinLock
    {
        AU32 Counter = 1;
        F_AtomicFlag WaitForConsumerFlag;

        /**
         * @brief Acquires lock from producer side
         * 
         * Blocks until the lock can be acquired by a producer.
         */
        void ProducerLock() noexcept
        {
            while(WaitForConsumerFlag.test(boost::memory_order_acquire));

            U32 ExpectedCounter = 1;
            U32 TargetCounter = 2;

            while(
                !Counter.compare_exchange_weak(
                    ExpectedCounter,
                    TargetCounter,
                    boost::memory_order_acq_rel
                )
            ) {
                if(ExpectedCounter == 0)
                {
                    ExpectedCounter = 1;
                    TargetCounter = 2;
                }
                else
                {
                    TargetCounter = ExpectedCounter + 1;
                }
            }
        }
        
        /**
         * @brief Tries to acquire lock from producer side without blocking
         * 
         * @return True if the lock was acquired, false otherwise
         */
        B8 ProducerTryLock() noexcept
        {
            if(WaitForConsumerFlag.test(boost::memory_order_acquire))
                return false;

            U32 ExpectedCounter = 1;
            U32 TargetCounter = 2;

            while(
                !Counter.compare_exchange_weak(
                    ExpectedCounter,
                    TargetCounter,
                    boost::memory_order_acq_rel
                )
            )
            {
                if(ExpectedCounter == 0)
                {
                    return false;
                }
                else
                {
                    TargetCounter = ExpectedCounter + 1;
                }
            }

            return true;
        }
        
        /**
         * @brief Releases lock from producer side
         */
        void ProducerUnlock() noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(Counter.load(boost::memory_order_acquire) > 1);

            Counter.fetch_sub(1, boost::memory_order_acq_rel);
        }

        /**
         * @brief Acquires lock from consumer side
         * 
         * Blocks until the lock can be acquired by the consumer.
         */
        void ConsumerLock() noexcept
        {
            while(WaitForConsumerFlag.test_and_set(boost::memory_order_acq_rel));

            U32 ExpectedCounter = 1;

            while(
                !Counter.compare_exchange_weak(
                    ExpectedCounter,
                    0,
                    boost::memory_order_acq_rel
                )
            ) {
                ExpectedCounter = 1;
            }

            WaitForConsumerFlag.clear(boost::memory_order_release);
        }
        
        /**
         * @brief Releases lock from consumer side
         */
        void ConsumerUnlock() noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(Counter.load(boost::memory_order_acquire) == 0);

            Counter.store(1, boost::memory_order_release);
        }
    };
}