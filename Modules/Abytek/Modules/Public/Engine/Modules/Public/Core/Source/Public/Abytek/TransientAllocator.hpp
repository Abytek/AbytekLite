#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ByteAllocator.hpp"


namespace Abytek
{
    using F_TransientMemoryParamIndex = U32;
    enum class E_TransientMemoryParam : F_TransientMemoryParamIndex
    {
        MAIN,
        RENDER,

        NUM
    };

    struct ABYTEK_ENGINE_CORE_API ABYTEK_ALIGN(ABYTEK_MIN_MEMORY_ALIGNMENT) F_TransientMemoryChunk
    {
    private:
        E_TransientMemoryParam _Param = E_TransientMemoryParam::MAIN;
        Sz _Capacity = 0;
        Sz _PayloadCapacity = 0;
        U8* _PayloadP = 0;
        Sz _CurrentUsage = 0;

    public:
        ABYTEK_FORCE_INLINE E_TransientMemoryParam GetParam() const noexcept
        {
            return _Param;
        }
        ABYTEK_FORCE_INLINE Sz GetCapacity() const noexcept
        {
            return _Capacity;
        }
        ABYTEK_FORCE_INLINE Sz GetPayloadCapacity() const noexcept
        {
            return _PayloadCapacity;
        }
        ABYTEK_FORCE_INLINE U8* GetPayloadP() const noexcept
        {
            return _PayloadP;
        }
        ABYTEK_FORCE_INLINE Sz GetCurrentUsage() const noexcept
        {
            return _CurrentUsage;
        }
        
    private:
        F_TransientMemoryChunk(E_TransientMemoryParam Param, Sz Capacity, Sz PayloadCapacity);
        ~F_TransientMemoryChunk();

    public:
        ABYTEK_FORCE_INLINE void AddUsage(Sz Delta)
        {
            _CurrentUsage += Delta;
        }
        ABYTEK_FORCE_INLINE void ResetUsage()
        {
            _CurrentUsage = 0;
        }
        
    public:
        static F_TransientMemoryChunk* Create(E_TransientMemoryParam Param, Sz PayloadCapacity);
        static void Destroy(F_TransientMemoryChunk* ChunkP);
    };

    class ABYTEK_ENGINE_CORE_API F_TransientMemoryHeap
    {
    private:
        static F_TransientMemoryHeap* _Instance;

    public:
        static ABYTEK_FORCE_INLINE F_TransientMemoryHeap& GetInstance() { return *_Instance; }

    private:
        TF_Array<
            TF_ConcurrentStack<F_TransientMemoryChunk*>,
            Sz(E_TransientMemoryParam::NUM)
        > _ChunkPStacks;
        Sz _ChunkPayloadCapacities[Sz(E_TransientMemoryParam::NUM)];
        Sz _ChunkCacheCapacities[Sz(E_TransientMemoryParam::NUM)];
        I16 _ChunkResidentFrames[Sz(E_TransientMemoryParam::NUM)];
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetChunkPStacks() const noexcept
        {
            return _ChunkPStacks;
        }
        ABYTEK_FORCE_INLINE auto GetChunkPayloadCapacities() const noexcept
        {
            return _ChunkPayloadCapacities;
        }
        ABYTEK_FORCE_INLINE auto GetChunkCacheCapacities() const noexcept
        {
            return _ChunkCacheCapacities;
        }
        ABYTEK_FORCE_INLINE auto GetChunkResidentFrames() const noexcept
        {
            return _ChunkResidentFrames;
        }
        
    public:
        F_TransientMemoryHeap();
        ~F_TransientMemoryHeap();

    public:
        F_TransientMemoryChunk* CreateChunk(E_TransientMemoryParam Param);
        void DestroyChunk(F_TransientMemoryChunk* ChunkP);

    public:
        void FlushCachedChunks();
    };

    class ABYTEK_ENGINE_CORE_API F_TransientMemoryAdapter final : public A_Object
    {
    public:
        friend class F_TransientMemoryManager;
        
    private:
        F_TransientMemoryHeap& _Heap;
        Sz _ChunkPayloadCapacities[Sz(E_TransientMemoryParam::NUM)];
        I16 _ChunkResidentFrames[Sz(E_TransientMemoryParam::NUM)];
        TF_Array<TF_Stack<F_TransientMemoryChunk*>, Sz(E_TransientMemoryParam::NUM)> _ChunkPStacks;
        TF_Array<
            TF_Vector<
                TF_Vector<F_TransientMemoryChunk*>
            >,
            Sz(E_TransientMemoryParam::NUM)
        > _ChunkPsToDestroy;

        TF_List<TU<F_TransientMemoryAdapter>>::iterator _Handle;
        
        F_TransientMemoryAdapter* _LastP;

    public:
        ABYTEK_FORCE_INLINE F_TransientMemoryHeap& GetHeap() const noexcept
        {
            return _Heap;
        }
        ABYTEK_FORCE_INLINE const auto& GetChunkPStacks() const noexcept
        {
            return _ChunkPStacks;
        }
        ABYTEK_FORCE_INLINE const auto& GetChunkPsToDestroy() const noexcept
        {
            return _ChunkPsToDestroy;
        }
        
    public:
        F_TransientMemoryAdapter(F_TransientMemoryHeap& Heap);
        ~F_TransientMemoryAdapter();

    private:
        ABYTEK_FORCE_INLINE void* _OptainBytes(E_TransientMemoryParam Param, Sz ActualSize)
        {
            auto ParamIdx = static_cast<F_TransientMemoryParamIndex>(Param);
            
            auto& ChunkPStack = _ChunkPStacks[ParamIdx];

            if(ChunkPStack.GetSize())
            {
                F_TransientMemoryChunk* ChunkP = ChunkPStack.GetTop();

                Sz MemoryOffset = ChunkP->GetCurrentUsage();
                Sz FreeSize = _ChunkPayloadCapacities[ParamIdx] - MemoryOffset;

                if(FreeSize >= ActualSize)
                {
                    ChunkP->AddUsage(ActualSize);
                    return ChunkP->GetPayloadP() + MemoryOffset;
                }
            }
            
            F_TransientMemoryChunk* ChunkP = _Heap.CreateChunk(Param);
            ChunkPStack.Push(ChunkP);

            ChunkP->AddUsage(ActualSize);
            return ChunkP->GetPayloadP();
        }

    public:
        void Bind();
        void Unbind();
        void ResetParam(E_TransientMemoryParam Param);
        void ResetParams();
        void DestroyChunks();
        void* AllocateBytes(E_TransientMemoryParam Param, Sz Size, Sz Alignment, Sz AlignmentOffset)
        {
            auto ParamIdx = static_cast<F_TransientMemoryParamIndex>(Param);

            Sz ActualSize = Size + Alignment;

            ABYTEK_ENGINE_CORE_ASSERT(ActualSize < _Heap.GetChunkPayloadCapacities()[ParamIdx]) << "the allocation size is too big, the limit is " << _Heap.GetChunkPayloadCapacities()[ParamIdx] << "(bytes)";

            void* RawMemory = _OptainBytes(Param, ActualSize);
            return (void*)(
                AlignAddress_PO2(
                    reinterpret_cast<uintptr_t>(RawMemory) + AlignmentOffset,
                    Alignment
                ) - AlignmentOffset
            );
        }

    public:
        void BeginFrame();
        void EndFrame();

    public:
        static F_TransientMemoryAdapter& GetCurrent();
    };
    
    struct F_TransientAllocatorOptions : F_ByteAllocatorOptions
    {
        static constexpr B8 EnableManualAlignment = true;
    };

    template<typename __F_Value, E_TransientMemoryParam __Param = E_TransientMemoryParam::MAIN>
    struct TF_StaticTransientAllocator :
        TA_ByteAllocator<TF_StaticTransientAllocator<__F_Value>, __F_Value, F_TransientAllocatorOptions>
    {
        static constexpr E_TransientMemoryParam Param = __Param;
        
        ABYTEK_DEFINE_ALLOCATOR_REBIND(
            __F_OtherValue,
            TF_StaticTransientAllocator<__F_OtherValue>
        );

        TF_StaticTransientAllocator() noexcept = default;

        TF_StaticTransientAllocator(const TF_StaticTransientAllocator&) noexcept = default;
        TF_StaticTransientAllocator& operator = (const TF_StaticTransientAllocator&) noexcept = default;

        template<typename __F_OtherValue>
        TF_StaticTransientAllocator(const TF_StaticTransientAllocator<__F_OtherValue>&) noexcept {}
        template<typename __F_OtherValue>
        TF_StaticTransientAllocator& operator = (const TF_StaticTransientAllocator<__F_OtherValue>& X) noexcept
        {
            return *this;
        }

        ABYTEK_FORCE_INLINE void* AllocateBytes(Sz Size, Sz Alignment, Sz AlignmentOffset)
        {
            ABYTEK_BASE_CORE_ASSERT(Size);
            ABYTEK_BASE_CORE_ASSERT(Alignment <= ABYTEK_MIN_MEMORY_ALIGNMENT);
            return F_TransientMemoryAdapter::GetCurrent().AllocateBytes(
                Param,
                Size,
                Alignment,
                AlignmentOffset
            );
        }
        ABYTEK_FORCE_INLINE void DeallocateBytes(void* Pointer)
        {
            ABYTEK_BASE_CORE_ASSERT(Pointer);
        }
    };

    class ABYTEK_ENGINE_CORE_API F_TransientMemoryManager : public A_Object
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_TransientMemoryManager);

    private:
        F_TransientMemoryHeap _Heap;

        F_SpinLock _AdapterLifetimeLock;
        TF_List<TU<F_TransientMemoryAdapter>> _Adapters;
        
    public:
        ABYTEK_FORCE_INLINE F_TransientMemoryHeap& GetHeap() noexcept
        {
            return _Heap;
        }

    public:
        F_TransientMemoryManager();
        ~F_TransientMemoryManager();

    public:
        TW_Valid<F_TransientMemoryAdapter> RegisterAdapter();
        void UnregisterAdapter(const TW_Valid<F_TransientMemoryAdapter>& Adapter);
    };
}
