/**
 * @file MemoryManager.hpp
 * @brief Provides a central system for tracking and analyzing memory usage.
 * 
 * The memory manager keeps track of all allocators and their memory usage.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/TypeInfo.hpp"
#include "Abytek/BoostContainerTemplates.hpp"
#include "Abytek/SPSCSpinLock.hpp"
#include "Abytek/DynamicAnalyableAllocator.hpp"
#include "Abytek/MemoryAllocationInfo.hpp"
#include "Abytek/LogStream.hpp"


namespace Abytek
{
    ABYTEK_BASE_CORE_API B8 IsMemoryLeakDebuggingEnabled();
    ABYTEK_BASE_CORE_API void EnableMemoryLeakDebugging();
    ABYTEK_BASE_CORE_API void DisableMemoryLeakDebugging();

    struct F_AllocatorAnalyzedState
    {
        U64 TypeHashCode = 0;
        const F_Char* TypeFullName = nullptr;
        
        Sz ActualUsage = 0;
        Sz PayloadUsage = 0;
        
        B8 IsEmpty() const noexcept
        {
            return (
                (ActualUsage == 0)    
                && (PayloadUsage == 0)    
            );
        }
    };
    
    /**
     * @brief Structure that holds the memory analysis state for a point in time.
     * 
     * Contains information about allocator types and their memory usage statistics.
     */
    struct ABYTEK_BASE_CORE_API F_MemoryAnalyzedState
    {
        ContainerTemplates::TF_Vector<F_AllocatorAnalyzedState> Allocators;

        F_MemoryAnalyzedState Compare(const F_MemoryAnalyzedState& Prev) const;
    };
    
    /**
     * @brief Log stream operator for memory analysis state.
     * 
     * @param LogStream The log stream to write to.
     * @param MemoryAnalyzedState The memory analysis state to log.
     * @return Reference to the log stream.
     */
    inline A_LogStream& operator << (A_LogStream& LogStream, const F_MemoryAnalyzedState& MemoryAnalyzedState)
    {
        auto AllocatorTypeCount = MemoryAnalyzedState.Allocators.size();
        for(U32 Idx = 0; Idx < AllocatorTypeCount; ++Idx)
        {
            LogStream
                << "Allocator Type "
                    << F_UpdateLogColor::LowLight()
                    << "{"
                    << F_UpdateLogColor::UInt()
                    << Idx
                    << F_UpdateLogColor::LowLight()
                    << "}: "
                    << F_NewLine {}
                    << F_ResetLogColor {}
                << "    Type Hash Code"
                    << F_UpdateLogColor::LowLight()
                    << ": "
                    << F_UpdateLogColor::UInt()
                    << MemoryAnalyzedState.Allocators[Idx].TypeHashCode
                    << F_NewLine {}
                    << F_ResetLogColor {}
                << "    Type Name"
                    << F_UpdateLogColor::LowLight()
                    << ": "
                    << F_UpdateLogColor::String()
                    << "\""
                    << MemoryAnalyzedState.Allocators[Idx].TypeFullName
                    << "\""
                    << F_NewLine {}
                    << F_ResetLogColor {}
                << "    Actual Usage"
                    << F_UpdateLogColor::LowLight()
                    << ": "
                    << F_UpdateLogColor::UInt()
                    << MemoryAnalyzedState.Allocators[Idx].ActualUsage
                    << F_NewLine {}
                    << F_ResetLogColor {}
                << "    Payload Usage"
                    << F_UpdateLogColor::LowLight()
                    << ": "
                    << F_UpdateLogColor::UInt()
                    << MemoryAnalyzedState.Allocators[Idx].PayloadUsage
                    << F_NewLine {}
                    << F_ResetLogColor {};
        }
        return LogStream;
    }


    /**
     * @brief Singleton class that manages memory allocation and tracking.
     * 
     * Provides tools for registering allocators, tracking memory usage,
     * and analyzing memory consumption across the application.
     */
    class ABYTEK_BASE_CORE_API F_MemoryManager final
    {
    public:
        using F_AllocatorStateMap = ContainerTemplates::TF_Map<U64, F_AllocatorAnalyzedState>;
        
    private:
        static F_MemoryManager* _Instance; ///< Singleton instance pointer

    public:
        /**
         * @brief Gets the singleton instance of the memory manager.
         * 
         * @return Reference to the singleton memory manager instance.
         */
        static ABYTEK_FORCE_INLINE F_MemoryManager& GetInstance() noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(_Instance);
            return *_Instance;
        }

    private:
        F_SPSCSpinLock _Lock; ///< Lock for thread-safe operations
        F_AllocatorStateMap _AllocatorStateMap;
        
    public:
        /**
         * @brief Gets the lock for thread-safe operations.
         * @return Reference to the spin lock.
         */
        ABYTEK_FORCE_INLINE auto& GetLock() noexcept { return _Lock; }
        
        /**
         * @brief Gets the lock for thread-safe operations (const version).
         * @return Const reference to the spin lock.
         */
        ABYTEK_FORCE_INLINE const auto& GetLock() const noexcept { return _Lock; }
        
        ABYTEK_FORCE_INLINE const auto& GetAllocatorStateMap() const noexcept { return _AllocatorStateMap; }

    public:
        /**
         * @brief Constructor that initializes the memory manager.
         */
        F_MemoryManager();
        
        /**
         * @brief Destructor that cleans up the memory manager.
         */
        ~F_MemoryManager();

    public:
        /**
         * @brief Analyzes the current memory usage state.
         * @return Memory analysis state containing information about all allocators and their usage.
         */
        F_MemoryAnalyzedState Analyze();
        
#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
    public:
        void Track(const F_MemoryAllocationInfo& MemoryAllocationInfo);
        void Untrack(const F_MemoryAllocationInfo& MemoryAllocationInfo);
    };
#endif
}