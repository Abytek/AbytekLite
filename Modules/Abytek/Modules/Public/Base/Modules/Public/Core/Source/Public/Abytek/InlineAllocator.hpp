#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/ByteAllocator.hpp"
#include "Abytek/CrtUseAllocatorTypedefs.hpp"


namespace Abytek
{
    class ABYTEK_BASE_CORE_API F_InlineHeap
    {
    public:
        struct F_Page
        {
            TF_Vector<U8> Data;
            Sz Usage = 0;
            Sz Capacity = 0;
            
            U8* Allocate(Sz Size)
            {
                U8* Result = Data.data() + Usage;
                Usage += Size;
                return Result;
            }
        };
        
    private:
        TF_Vector<F_Page> _Pages;
        Sz _MinPageCapacity = 0;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetPages() const noexcept
        {
            return _Pages;
        }
        ABYTEK_FORCE_INLINE Sz GetMinPageCapacity() const noexcept
        {
            return _MinPageCapacity;
        }
        
    public:
        F_InlineHeap(Sz MinPageCapacity) :
            _MinPageCapacity(MinPageCapacity)
        {}
        ~F_InlineHeap()
        {}
        
    public:
        void AddNewPage(Sz MinCapacity)
        {
            Sz Capacity = Max<Sz>(
                MinCapacity,
                _MinPageCapacity
            );
            if (_Pages.size() > 0)
            {
                Capacity = Max<Sz>(
                    Capacity,
                    _Pages.back().Capacity * 2
                );
            }
            
            _Pages.push_back({});
            auto& Page =  _Pages.back();
            Page.Capacity = Capacity;
            Page.Data.resize(Capacity);
        }
        F_Page* GetPage(Sz Free)
        {
            if (_Pages.size() == 0)
            {
                AddNewPage(Free);
                return &_Pages.back();
            }
            {
                auto& Page = _Pages.back();
                Sz PageFree = Page.Capacity - Page.Usage;
                if (PageFree >= Free)
                {
                    return &Page;
                }
            }
            AddNewPage(Free);
            return &_Pages.back();
        }
        void* AllocateBytes(Sz Size, Sz Alignment, Sz AlignmentOffset)
        {
            Sz ActualSize = Size + Alignment - 1;
            auto* Page = GetPage(ActualSize);
            U8* RawMemory = Page->Allocate(ActualSize);
            return (void*)(
                AlignAddress_PO2((uintptr_t)(RawMemory + AlignmentOffset), Alignment) 
                - AlignmentOffset
            );
        }
        void Reset()
        {
            _Pages.clear();
        }
    };
    
    struct F_InlineAllocatorOptions : F_ByteAllocatorOptions
    {
        static constexpr B8 EnableManualAlignment = true;
        static constexpr B8 EnableTracking = false;
    };

    template<typename __F_Value>
    struct TF_InlineAllocator :
        TA_ByteAllocator<TF_InlineAllocator<__F_Value>, __F_Value, F_InlineAllocatorOptions>
    {
        ABYTEK_DEFINE_ALLOCATOR_REBIND(
            __F_OtherValue,
            TF_InlineAllocator<__F_OtherValue>
        );
        
        F_InlineHeap* Heap = nullptr;

        TF_InlineAllocator() noexcept = default;
        TF_InlineAllocator(F_InlineHeap* InHeap) noexcept :
            Heap(InHeap)
        {}

        TF_InlineAllocator(const TF_InlineAllocator&) noexcept = default;
        TF_InlineAllocator& operator = (const TF_InlineAllocator&) noexcept = default;

        template<typename __F_OtherValue>
        TF_InlineAllocator(const TF_InlineAllocator<__F_OtherValue>& X) noexcept :
            Heap(X.Heap)
        {}
        template<typename __F_OtherValue>
        TF_InlineAllocator& operator = (const TF_InlineAllocator<__F_OtherValue>& X) noexcept
        {
            Heap = X.Heap;
            return *this;
        }

        ABYTEK_FORCE_INLINE void* AllocateBytes(Sz Size, Sz Alignment, Sz AlignmentOffset)
        {
            ABYTEK_BASE_CORE_ASSERT(Heap) << "Invalid inline heap, cannot allocate";
            return Heap->AllocateBytes(Size, Alignment, AlignmentOffset);
        }
        ABYTEK_FORCE_INLINE void DeallocateBytes(void* Pointer)
        {
            ABYTEK_BASE_CORE_ASSERT(Pointer);
        }
    };
    
    struct F_Arena
    {
        F_InlineHeap Heap;
        mutable F_SpinLock Lock;
        
        TF_ConcurrentQueue<TF_Function<void()>> CallbacksOnReset;
        
        F_Arena(Sz MinPageCapacity = 0x10000) :
            Heap(MinPageCapacity)
        {
        }
        ~F_Arena()
        {
            Reset();
        }
        
        void ResetUnsafe()
        {
            {
                TF_Function<void()> Callback;
                while (CallbacksOnReset.TryPop(Callback))
                {
                    Callback();
                }
            }
            Heap.Reset();
        }
        void Reset()
        {
            TF_ScopeLock<F_SpinLock> _(Lock);
            ResetUnsafe();
        }
        
        TF_Span<U8> CacheData(const TF_Span<U8>& Data)
        {
            TF_ScopeLock<F_SpinLock> _(Lock);
        
            TF_InlineAllocator<U8> Allocator(&Heap);
            TF_Span<U8> Result = {
                Allocator.Allocate(Data.size()),
                Data.size()
            };
            memcpy(
                Result.data(),
                Data.data(),
                Data.size()
            );
            return Result;
        }
        template<typename __F>
        TF_Span<U8> CacheData(const TF_Vector<__F>& Data)
        {
            return CacheData(
                TF_Span<U8>(
                    (U8*)Data.data(), 
                    sizeof(__F) * Data.size()
                )
            );
        }
        template<typename __F, auto __N>
        TF_Span<U8> CacheData(const __F (&Data)[__N])
        {
            return CacheData(
                TF_Span<U8>(
                    (U8*)Data, 
                    sizeof(Data)
                )
            );
        }
        template<typename __F>
        TF_Span<U8> CacheData(const __F& Data)
        {
            return CacheData(
                TF_Span<U8>(
                    (U8*)&Data, 
                    sizeof(__F)
                )
            );
        }
        
        void* AllocateData(Sz Size)
        {
            TF_ScopeLock<F_SpinLock> _(Lock);
        
            TF_InlineAllocator<U8> Allocator(&Heap);
            return Allocator.Allocate(Size);
        }
        template<typename __F>
        __F* AllocateData(Sz Count = 1)
        {
            TF_ScopeLock<F_SpinLock> _(Lock);
        
            TF_InlineAllocator<__F> Allocator(&Heap);
            return Allocator.Allocate(Count);
        }
        template<typename __F, typename... __F_Args>
        __F* New(__F_Args&&... Args)
        {
            auto RawPtr = AllocateData<__F>();
            return new(RawPtr) __F(ABYTEK_FORWARD(Args)...);
        }
        template<typename __F, typename... __F_Args>
        __F* New_ReleaseOnReset(__F_Args&&... Args)
        {
            auto Result = New<__F>(ABYTEK_FORWARD(Args)...);
            EnqueueReleaseOnReset(Result);
            return Result;
        }
        
        template<typename __F>
        void EnqueueCallbackOnReset(__F&& Callback)
        {
            CallbacksOnReset.Push(ABYTEK_FORWARD(Callback));
        }
        template<typename __F>
        void EnqueueReleaseOnReset(__F* Object)
        {
            EnqueueCallbackOnReset(
                [Object]()
                {
                    Object->~__F();
                }
            );
        }
    };
    
    struct F_DoubleArena
    {
        TF_Array<F_Arena, 2> Arenas;
        U32 WriteArenaIndex = 0;
        U32 ReadArenaIndex = 1;
        
        F_DoubleArena()
        {
        }
        ~F_DoubleArena()
        {
            Reset();
        }
        
        void Reset()
        {
            ResetSingle();
            ResetSingle();
        }
        void ResetSingle()
        {
            for (auto& Arena : Arenas)
            {
                Arena.Lock.ProducerLock();
            }
            std::swap(WriteArenaIndex, ReadArenaIndex);
            Arenas[WriteArenaIndex].ResetUnsafe();
            for (auto& Arena : Arenas)
            {
                Arena.Lock.ProducerUnlock();
            }
        }
        
        TF_Span<U8> CacheData(const TF_Span<U8>& Data)
        {
            return Arenas[WriteArenaIndex].CacheData(Data);
        }
        template<typename __F>
        TF_Span<U8> CacheData(const TF_Vector<__F>& Data)
        {
            return Arenas[WriteArenaIndex].CacheData(Data);
        }
        template<typename __F>
        TF_Span<U8> CacheData(const __F& Data)
        {
            return Arenas[WriteArenaIndex].CacheData(Data);
        }
        
        void* AllocateData(Sz Size)
        {
            return Arenas[WriteArenaIndex].AllocateData(Size);
        }
        template<typename __F>
        __F* AllocateData(Sz Count = 1)
        {
            return Arenas[WriteArenaIndex].AllocateData<__F>(Count);
        }
        template<typename __F, typename... __F_Args>
        __F* New(__F_Args&&... Args)
        {
            return Arenas[WriteArenaIndex].New<__F>(ABYTEK_FORWARD(Args)...);
        }
        template<typename __F, typename... __F_Args>
        __F* New_ReleaseOnReset(__F_Args&&... Args)
        {
            return Arenas[WriteArenaIndex].New_ReleaseOnReset<__F>(ABYTEK_FORWARD(Args)...);
        }
        
        template<typename __F>
        void EnqueueCallbackOnReset(__F&& Callback)
        {
            Arenas[WriteArenaIndex].EnqueueCallbackOnReset(ABYTEK_FORWARD(Callback));
        }
        template<typename __F>
        void EnqueueReleaseOnReset(__F* Object)
        {
            Arenas[WriteArenaIndex].EnqueueReleaseOnReset(Object);
        }
    };
}
