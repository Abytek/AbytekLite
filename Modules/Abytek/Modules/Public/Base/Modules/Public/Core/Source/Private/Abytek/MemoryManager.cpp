#include "Abytek/MemoryManager.hpp"
#include "Abytek/DynamicAnalyableAllocator.hpp"



namespace Abytek
{
    namespace Internal
    {
        AB8 IsMemoryLeakDebuggingEnabled = 0;
    }
    B8 IsMemoryLeakDebuggingEnabled()
    {
        return Internal::IsMemoryLeakDebuggingEnabled.load(boost::memory_order_acquire);
    }
    void EnableMemoryLeakDebugging()
    {
        Internal::IsMemoryLeakDebuggingEnabled.store(true, boost::memory_order_release);
    }
    void DisableMemoryLeakDebugging()
    {
        Internal::IsMemoryLeakDebuggingEnabled.store(false, boost::memory_order_release);
    }
    
    F_MemoryManager* F_MemoryManager::_Instance = 0;

    F_MemoryAnalyzedState F_MemoryAnalyzedState::Compare(const F_MemoryAnalyzedState& Prev) const
    {
        auto ToAllocatorStateMap = [](const F_MemoryAnalyzedState& State) -> F_MemoryManager::F_AllocatorStateMap
        {
            F_MemoryManager::F_AllocatorStateMap AllocatorStateMap;
            for (const auto& AllocatorState : State.Allocators)
            {
                AllocatorStateMap[AllocatorState.TypeHashCode] = AllocatorState;
            }
            return boost::move(AllocatorStateMap);
        };

        F_MemoryManager::F_AllocatorStateMap CurrentAllocatorStateMap = ToAllocatorStateMap(*this);
        F_MemoryManager::F_AllocatorStateMap PrevAllocatorStateMap = ToAllocatorStateMap(Prev);
        
        F_MemoryManager::F_AllocatorStateMap ResultAllocatorStateMap;
        for(const auto& [AllocatorTypeHashCode, AllocatorAnalyzedState] : CurrentAllocatorStateMap)
        {
            auto PrevIt = PrevAllocatorStateMap.find(AllocatorTypeHashCode);
            if(PrevIt == PrevAllocatorStateMap.end())
            {
                ResultAllocatorStateMap[AllocatorTypeHashCode] = AllocatorAnalyzedState;
            }
            else
            {
                const F_AllocatorAnalyzedState& PrevAllocatorAnalyzedState = PrevIt->second;

                F_AllocatorAnalyzedState ChangedAllocatorAnalyzedState = AllocatorAnalyzedState;
                ChangedAllocatorAnalyzedState.ActualUsage -= PrevAllocatorAnalyzedState.ActualUsage;
                ChangedAllocatorAnalyzedState.PayloadUsage -= PrevAllocatorAnalyzedState.PayloadUsage;
                
                if(!ChangedAllocatorAnalyzedState.IsEmpty())
                {
                    ResultAllocatorStateMap[AllocatorTypeHashCode] = ChangedAllocatorAnalyzedState;
                }
            }
        }

        F_MemoryAnalyzedState Result;
        for(const auto& [AllocatorTypeHashCode, AllocatorAnalyzedState] : ResultAllocatorStateMap)
        {
            Result.Allocators.push_back(AllocatorAnalyzedState);
        }
        return boost::move(Result);
    }

    F_MemoryManager::F_MemoryManager()
    {
        _Instance = this;
    }
    F_MemoryManager::~F_MemoryManager()
    {
    }

    F_MemoryAnalyzedState F_MemoryManager::Analyze()
    {
        _Lock.ProducerLock();
        F_MemoryAnalyzedState Result;
        for (const auto& [_, AllocatorState] : _AllocatorStateMap)
        {
            Result.Allocators.push_back(AllocatorState);   
        }
        _Lock.ProducerUnlock();
        return boost::move(Result);
    }

#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
    void F_MemoryManager::Track(const F_MemoryAllocationInfo& MemoryAllocationInfo)
    {
        if (MemoryAllocationInfo.IsEmpty())
        {
            return;
        }
        _Lock.ProducerLock();
        
        F_AllocatorAnalyzedState* AllocatorStateHandle = nullptr;
        
        auto It = _AllocatorStateMap.find(MemoryAllocationInfo.AllocatorTypeHashCode);
        if (It == _AllocatorStateMap.end())
        {
            AllocatorStateHandle = &_AllocatorStateMap[MemoryAllocationInfo.AllocatorTypeHashCode];
            AllocatorStateHandle->TypeHashCode = MemoryAllocationInfo.AllocatorTypeHashCode;
            AllocatorStateHandle->TypeFullName = MemoryAllocationInfo.AllocatorTypeFullName;
        }
        else
        {
            AllocatorStateHandle = &It->second;
        }
        
        AllocatorStateHandle->ActualUsage += MemoryAllocationInfo.ActualSize;
        AllocatorStateHandle->PayloadUsage += MemoryAllocationInfo.PayloadSize;
        
        _Lock.ProducerUnlock();
    }
    void F_MemoryManager::Untrack(const F_MemoryAllocationInfo& MemoryAllocationInfo)
    {
        if (MemoryAllocationInfo.IsEmpty())
        {
            return;
        }
        _Lock.ProducerLock();
        
        F_AllocatorAnalyzedState* AllocatorStateHandle = nullptr;
        
        auto It = _AllocatorStateMap.find(MemoryAllocationInfo.AllocatorTypeHashCode);
        ABYTEK_BASE_CORE_ASSERT(It != _AllocatorStateMap.end()) 
            << "Not found allocator analyzed state: hash code = " 
            << MemoryAllocationInfo.AllocatorTypeHashCode
            << ", full name = "
            << MemoryAllocationInfo.AllocatorTypeFullName;
        
        AllocatorStateHandle = &It->second;
        
        AllocatorStateHandle->ActualUsage -= MemoryAllocationInfo.ActualSize;
        AllocatorStateHandle->PayloadUsage -= MemoryAllocationInfo.PayloadSize;
        
        if (AllocatorStateHandle->IsEmpty())
        {
            _AllocatorStateMap.erase(It);
        }
        
        _Lock.ProducerUnlock();
    }
#endif
}