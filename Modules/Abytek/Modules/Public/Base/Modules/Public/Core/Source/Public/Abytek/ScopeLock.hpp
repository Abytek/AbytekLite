#pragma once
 
#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief RAII consumer lock wrapper that automatically acquires and releases lock
     * 
     * Provides a scoped lock implementation for the consumer side of the lock.
     * The lock is acquired in the constructor and released in the destructor.
     * 
     * @tparam __F_TargetLock The lock type to wrap
     */
    template<typename __F_TargetLock>
    struct TF_ConsumerScopeLock
    {
    public:
        using F_TargetLock = __F_TargetLock;
        
    private:
        F_TargetLock& _TargetLock;

    public:
        /**
         * @brief Gets the target lock
         * @return Reference to the target lock
         */
        ABYTEK_FORCE_INLINE F_TargetLock& GetTargetLock() noexcept
        {
            return _TargetLock;
        }
        
    public:
        /**
         * @brief Constructor that acquires the lock
         * @param TargetLock The lock to acquire
         */
        TF_ConsumerScopeLock(F_TargetLock& TargetLock) :
            _TargetLock(TargetLock)
        {
            _TargetLock.ConsumerLock();
        }
        
        /**
         * @brief Destructor that releases the lock
         */
        ~TF_ConsumerScopeLock()
        {
            _TargetLock.ConsumerUnlock();
        }
    };
    
    /**
     * @brief RAII producer lock wrapper that automatically acquires and releases lock
     * 
     * Provides a scoped lock implementation for the producer side of the lock.
     * The lock is acquired in the constructor and released in the destructor.
     * 
     * @tparam __F_TargetLock The lock type to wrap
     */
    template<typename __F_TargetLock>
    struct TF_ProducerScopeLock
    {
    public:
        using F_TargetLock = __F_TargetLock;
        
    private:
        F_TargetLock& _TargetLock;

    public:
        /**
         * @brief Gets the target lock
         * @return Reference to the target lock
         */
        ABYTEK_FORCE_INLINE F_TargetLock& GetTargetLock() noexcept
        {
            return _TargetLock;
        }
        
    public:
        /**
         * @brief Constructor that acquires the lock
         * @param TargetLock The lock to acquire
         */
        TF_ProducerScopeLock(F_TargetLock& TargetLock) :
            _TargetLock(TargetLock)
        {
            _TargetLock.ProducerLock();
        }
        
        /**
         * @brief Destructor that releases the lock
         */
        ~TF_ProducerScopeLock()
        {
            _TargetLock.ProducerUnlock();
        }
    };
    
    /**
     * @brief Alias for producer scope lock for backward compatibility
     * 
     * @tparam __F_TargetLock The lock type to wrap
     */
    template<typename __F_TargetLock>
    using TF_ScopeLock = TF_ProducerScopeLock<__F_TargetLock>;
}