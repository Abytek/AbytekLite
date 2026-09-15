#include "Abytek/TransientAllocator.hpp"
#include "Abytek/DefaultAllocators.hpp"


namespace Abytek
{
    F_TransientMemoryHeap* F_TransientMemoryHeap::_Instance = 0;

    F_TransientMemoryChunk::F_TransientMemoryChunk(E_TransientMemoryParam Param, Sz Capacity, Sz PayloadCapacity) :
        _Param(Param),
        _Capacity(Capacity),
        _PayloadCapacity(PayloadCapacity),
        _PayloadP((U8*)(this + 1))
    {
        ABYTEK_ENGINE_CORE_ASSERT(Capacity > PayloadCapacity);
        ABYTEK_ENGINE_CORE_ASSERT(PayloadCapacity);
    }
    F_TransientMemoryChunk::~F_TransientMemoryChunk()
    {
    }

    F_TransientMemoryChunk* F_TransientMemoryChunk::Create(E_TransientMemoryParam Param, Sz PayloadCapacity)
    {
        Sz Capacity = sizeof(F_TransientMemoryChunk) + PayloadCapacity;

        TF_DefaultAllocator<U8> CrtAllocator;
        U8* Memory = CrtAllocator.Allocate(Capacity);

        return new(Memory) F_TransientMemoryChunk(Param, Capacity, PayloadCapacity);
    }
    void F_TransientMemoryChunk::Destroy(F_TransientMemoryChunk* ChunkP)
    {
        ChunkP->~F_TransientMemoryChunk();
        
        TF_DefaultAllocator<U8> CrtAllocator;
        CrtAllocator.Deallocate((U8*)ChunkP);
    }

    F_TransientMemoryHeap::F_TransientMemoryHeap()
    {
        _Instance = this;

        _ChunkPayloadCapacities[static_cast<I32>(E_TransientMemoryParam::MAIN)] = 2 * 1024 * 1024;
        _ChunkCacheCapacities[static_cast<I32>(E_TransientMemoryParam::MAIN)] = 1024;
        _ChunkResidentFrames[static_cast<I32>(E_TransientMemoryParam::MAIN)] = 2;
        
        _ChunkPayloadCapacities[static_cast<I32>(E_TransientMemoryParam::RENDER)] = 2 * 1024 * 1024;
        _ChunkCacheCapacities[static_cast<I32>(E_TransientMemoryParam::RENDER)] = 1024;
        _ChunkResidentFrames[static_cast<I32>(E_TransientMemoryParam::RENDER)] = 2;
    }
    F_TransientMemoryHeap::~F_TransientMemoryHeap()
    {
        FlushCachedChunks();
    }

    F_TransientMemoryChunk* F_TransientMemoryHeap::CreateChunk(E_TransientMemoryParam Param)
    {
        auto ParamIdx = static_cast<F_TransientMemoryParamIndex>(Param);

        auto& ChunkPStack = _ChunkPStacks[ParamIdx];
        
        F_TransientMemoryChunk* ChunkP;
        if(ChunkPStack.TryPop(ChunkP))
        {
            return ChunkP;
        }
        
        return F_TransientMemoryChunk::Create(Param, _ChunkPayloadCapacities[ParamIdx]);
    }
    void F_TransientMemoryHeap::DestroyChunk(F_TransientMemoryChunk* ChunkP)
    {
        auto ParamIdx = static_cast<F_TransientMemoryParamIndex>(ChunkP->GetParam());

        auto& ChunkPStack = _ChunkPStacks[ParamIdx];
        Sz ChunkCacheCapacity = _ChunkCacheCapacities[ParamIdx];

        if(ChunkPStack.GetSize() >= ChunkCacheCapacity)
        {
            F_TransientMemoryChunk::Destroy(ChunkP);
            return;
        }
        
        ChunkPStack.Push(ChunkP);
    }

    void F_TransientMemoryHeap::FlushCachedChunks()
    {
        for(auto& ChunkPStack : _ChunkPStacks)
        {
            F_TransientMemoryChunk* ChunkP = 0;
            while(ChunkPStack.TryPop(ChunkP))
            {
                F_TransientMemoryChunk::Destroy(ChunkP);
            }
        }
    }

    F_TransientMemoryAdapter::F_TransientMemoryAdapter(F_TransientMemoryHeap& Heap) :
        _Heap(Heap)
    {
        memcpy(
            _ChunkPayloadCapacities,
            _Heap.GetChunkPayloadCapacities(),
            sizeof(Sz) * static_cast<F_TransientMemoryParamIndex>(E_TransientMemoryParam::NUM)
        );
        memcpy(
            _ChunkResidentFrames,
            _Heap.GetChunkResidentFrames(),
            sizeof(I16) * static_cast<F_TransientMemoryParamIndex>(E_TransientMemoryParam::NUM)
        );
        for(
            F_TransientMemoryParamIndex ParamIdx = 0;
            ParamIdx < static_cast<F_TransientMemoryParamIndex>(E_TransientMemoryParam::NUM);
            ++ParamIdx
        )
        {
            I16 NumResidentFrames = _ChunkResidentFrames[ParamIdx];
            
            auto& ChunkPsToDestroy = _ChunkPsToDestroy[ParamIdx];
            ChunkPsToDestroy.resize(NumResidentFrames);
        }
    }
    F_TransientMemoryAdapter::~F_TransientMemoryAdapter()
    {
    }

    thread_local F_TransientMemoryAdapter* CurrentTransientMemoryAdapter;
    
    void F_TransientMemoryAdapter::Bind()
    {
        _LastP = CurrentTransientMemoryAdapter;
        CurrentTransientMemoryAdapter = this;
    }
    void F_TransientMemoryAdapter::Unbind()
    {
        CurrentTransientMemoryAdapter = _LastP;
    }

    void F_TransientMemoryAdapter::ResetParam(E_TransientMemoryParam Param)
    {
        auto ParamIdx = static_cast<F_TransientMemoryParamIndex>(Param);
        
        auto& ChunkPStack = _ChunkPStacks[ParamIdx];

        F_TransientMemoryChunk* ChunkP;
        while (ChunkPStack.TryPop(ChunkP))
        {
            ChunkP->ResetUsage();
            
            auto& ChunkPsToDestroy = _ChunkPsToDestroy[ParamIdx];
            ChunkPsToDestroy[0].push_back(ChunkP);
        }
    }
    void F_TransientMemoryAdapter::ResetParams()
    {
        for(auto ParamIdx = 0; ParamIdx < static_cast<F_TransientMemoryParamIndex>(E_TransientMemoryParam::NUM); ++ParamIdx)
        {
            ResetParam(static_cast<E_TransientMemoryParam>(ParamIdx));
        }
    }
    void F_TransientMemoryAdapter::DestroyChunks()
    {
        for(
            F_TransientMemoryParamIndex ParamIdx = 0;
            ParamIdx < static_cast<F_TransientMemoryParamIndex>(E_TransientMemoryParam::NUM);
            ++ParamIdx
        )
        {
            I16 NumResidentFrames = _ChunkResidentFrames[ParamIdx];
            
            auto& ChunkPsToDestroy = _ChunkPsToDestroy[ParamIdx];
            
            auto& TargetChunkPsToDestroy = ChunkPsToDestroy[NumResidentFrames - 1];
            for(F_TransientMemoryChunk* ChunkP : TargetChunkPsToDestroy)
            {
                _Heap.DestroyChunk(ChunkP);
            }
            TargetChunkPsToDestroy.resize(0);
            
            auto TempChunkPsToDestroy = ChunkPsToDestroy;
            for(I16 ResidentFrameIdx = 0; ResidentFrameIdx < NumResidentFrames; ++ResidentFrameIdx)
            {
                ChunkPsToDestroy[ResidentFrameIdx] = TempChunkPsToDestroy[
                    (ResidentFrameIdx + NumResidentFrames - 1) % NumResidentFrames
                ];
            }
        }
    }

    void F_TransientMemoryAdapter::BeginFrame()
    {
        Bind();
    }
    void F_TransientMemoryAdapter::EndFrame()
    {
        ResetParams();
        DestroyChunks();
        Unbind();
    }

    F_TransientMemoryAdapter& F_TransientMemoryAdapter::GetCurrent()
    {
        return *CurrentTransientMemoryAdapter;
    }
    
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_TransientMemoryManager);

    F_TransientMemoryManager::F_TransientMemoryManager()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    F_TransientMemoryManager::~F_TransientMemoryManager()
    {
    }

    TW_Valid<F_TransientMemoryAdapter> F_TransientMemoryManager::RegisterAdapter()
    {
        TF_ScopeLock<F_SpinLock> _(_AdapterLifetimeLock);
        
        auto OwnedAdapter = TU<F_TransientMemoryAdapter>()(_Heap);
        auto Adapter = OwnedAdapter.Weak();

        _Adapters.push_back(boost::move(OwnedAdapter));
        Adapter->_Handle = --(_Adapters.end());
        return Adapter;
    }
    void F_TransientMemoryManager::UnregisterAdapter(const TW_Valid<F_TransientMemoryAdapter>& Adapter)
    {
        _Adapters.erase(Adapter->_Handle);
    }
}
