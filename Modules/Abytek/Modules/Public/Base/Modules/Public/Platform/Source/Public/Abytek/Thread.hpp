/**
 * @file Thread.hpp
 * @brief Defines thread management functionality for the Abytek engine
 * 
 * This file provides platform-independent threading abstractions using Boost.Thread
 * under the hood. It includes thread creation, management, and utility functions.
 */
#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @typedef F_ThreadId
     * @brief Thread identifier type
     * 
     * Unique identifier for distinguishing between different threads
     */
    using F_ThreadId = boost::thread::id;

    /**
     * @class F_Thread
     * @brief Thread management class
     * 
     * Provides a platform-independent wrapper around Boost.Thread functionality
     * for creating and managing threads. Supports move semantics to transfer
     * thread ownership.
     */
    class ABYTEK_BASE_PLATFORM_API F_Thread final
    {
    private:
        boost::thread _BoostThread; ///< Underlying Boost thread implementation
        F_Name _Name;
        
    public:
        /**
         * @brief Get the underlying Boost thread reference
         * @return Reference to the underlying Boost thread
         */
        ABYTEK_FORCE_INLINE boost::thread& GetBoostThread() noexcept { return _BoostThread; }
        
        /**
         * @brief Get the underlying Boost thread as a const reference
         * @return Const reference to the underlying Boost thread
         */
        ABYTEK_FORCE_INLINE const boost::thread& GetBoostThread() const noexcept { return _BoostThread; }
        
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept { return _Name; }

    public:
        /**
         * @brief Default constructor
         * 
         * Creates an empty thread object that doesn't represent any thread of execution
         */
        F_Thread();
        
        /**
         * @brief Destructor
         * 
         * If the thread is still joinable, it might cause termination of the program
         */
        ~F_Thread();

        /**
         * @brief Move constructor
         * @param X Thread to move from
         * 
         * Transfers ownership of the thread from X to the newly constructed object
         */
        F_Thread(F_Thread&& X) noexcept;
        
        /**
         * @brief Move assignment operator
         * @param X Thread to move from
         * @return Reference to this thread object
         * 
         * Transfers ownership of the thread from X to this object
         */
        F_Thread& operator = (F_Thread&& X) noexcept;

        /**
         * @brief Constructor with a function to execute
         * @param Callback Function to execute in the new thread
         * 
         * Creates a new thread that executes the provided callback function
         */
        F_Thread(TF_Function<void()>&& Callback, const F_Name& Name = {});
        
        /**
         * @brief Constructor with a Boost thread
         * @param BoostThread Boost thread to wrap
         * 
         * Creates a thread object that takes ownership of the provided Boost thread
         */
        explicit F_Thread(boost::thread&& BoostThread) noexcept;

    public:
        /**
         * @brief Join the thread
         * 
         * Blocks the calling thread until this thread finishes execution
         * If the thread is not joinable, behavior is undefined
         */
        void Join() noexcept;
        
        /**
         * @brief Detach the thread
         * 
         * Separates the thread of execution from the thread object, allowing
         * the thread to continue independently
         */
        void Detach() noexcept;

    public:
        void SetName(const F_Name& Name) noexcept;

    public:
        /**
         * @brief Get the number of hardware thread contexts available
         * @return Number of hardware thread contexts
         * 
         * Returns the number of hardware thread contexts available on the current system
         * This may include hyperthreaded cores
         */
        static ABYTEK_FORCE_INLINE U32 HardwareConcurrency() noexcept
        {
            return boost::thread::hardware_concurrency();
        }
        
        /**
         * @brief Get the number of physical cores available
         * @return Number of physical cores
         * 
         * Returns the number of physical cores available on the current system
         * This excludes hyperthreaded cores when possible
         */
        static ABYTEK_FORCE_INLINE U32 PhysicalConcurrency() noexcept
        {
            return boost::thread::physical_concurrency();
        }
    };

    /**
     * @struct H_ThisThread
     * @brief Static utilities for the current thread
     * 
     * Provides utility functions for operating on the current thread
     */
    struct ABYTEK_BASE_PLATFORM_API H_ThisThread
    {
        /**
         * @brief Get the ID of the current thread
         * @return ID of the current thread
         */
        static F_ThreadId GetId() noexcept;
        
        /**
         * @brief Yield execution to another thread
         * 
         * Suggests that the scheduler run other threads before continuing
         * execution of the current thread
         */
        static void YieldThread() noexcept;

        static F_Name GetName() noexcept;
        static void SetName(const F_Name& Name) noexcept;
    };
}
