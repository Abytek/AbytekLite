#include "Abytek/StandardObjectManagement.hpp"


namespace Abytek
{
    void F_StandardObjectKeyGenerationBuffer::ResizeNonThreadSafe(U32 NewSize)
    {
        auto LastSize = Size;
        Size = NewSize;
        Generations.resize(NewSize);
        TrackingFlags.resize(NewSize);
        NextIndices.resize(NewSize);
        
        for (U32 Idx = LastSize; Idx < NewSize; ++Idx)
        {
            Generations[Idx] = 0;
            TrackingFlags[Idx] = E_StandardObjectTrackingFlag::NONE;
            NextIndices[Idx] = ABYTEK_U32_MAX;
        }
    }
    void F_StandardObjectKeyGenerationBuffer::EnsureMinSize(U32 MinSize)
    {
        while(MinSize > Size)
        {
            ResizeNonThreadSafe(Size ? (Size * 2) : 1);
        }
    }

    F_StandardObjectKeyPage::F_StandardObjectKeyPage()
    {
    }
    F_StandardObjectKeyPage::~F_StandardObjectKeyPage()
    {
    }

    F_StandardObjectKeyPage::F_StandardObjectKeyPage(F_StandardObjectKeyPage&& X)
    {
        X.Reset();
    }
    F_StandardObjectKeyPage& F_StandardObjectKeyPage::operator = (F_StandardObjectKeyPage&& X)
    {
        X.Reset();
        return *this;
    }

    void F_StandardObjectKeyPage::Reset()
    {
    }

    void F_StandardObjectKeyPage::OptainGeneration(U32& OutGenerationIndex, U32& OutGeneration) noexcept
    {
        if(_FreeGenerationTailIndex == ABYTEK_U32_MAX)
        {
            OutGenerationIndex = _GenerationCapacity;
            OutGeneration = 0;
            ++_GenerationCapacity;

            _GenerationBuffer.EnsureMinSize(_GenerationCapacity);

            _GenerationBuffer.Generations[OutGenerationIndex] = 0;
            _GenerationBuffer.TrackingFlags[OutGenerationIndex] = E_StandardObjectTrackingFlag::NONE;

        }
        else
        {
            OutGenerationIndex = _FreeGenerationTailIndex;
            OutGeneration = _GenerationBuffer.Generations[OutGenerationIndex];
            
            _GenerationBuffer.TrackingFlags[OutGenerationIndex] = E_StandardObjectTrackingFlag::NONE;
            
            _FreeGenerationTailIndex = _GenerationBuffer.NextIndices[_FreeGenerationTailIndex];
        }
    }
    void F_StandardObjectKeyPage::FreeGeneration(U32 GenerationIndex) noexcept
	{
		// increase Generation
		++(_GenerationBuffer.Generations[GenerationIndex]);

        // Link and set new free generation tail
        _GenerationBuffer.NextIndices[GenerationIndex] = _FreeGenerationTailIndex;
        _FreeGenerationTailIndex = GenerationIndex;
	}

    F_StandardObjectKey F_StandardObjectKeyPage::Register()
    {
        TF_ScopeLock<F_SpinLock> _(_GenerationBuffer.Lock);
        U32 GenerationIndex = ABYTEK_U32_MAX;
        U32 Generation = ABYTEK_U32_MAX;
        OptainGeneration(GenerationIndex, Generation);

        ABYTEK_BASE_OBJECT_ASSERT(GenerationIndex != ABYTEK_U32_MAX) << "invalid generation index";
        ABYTEK_BASE_OBJECT_ASSERT(Generation != ABYTEK_U32_MAX) << "invalid generation";

        F_StandardObjectKey Result;
        ForwardStandardObjectId(Result.IdValue).LocalIndex = GenerationIndex;
        Result.Generation = Generation;

        return Result;
    }
    void F_StandardObjectKeyPage::Deregister(F_StandardObjectKey Key)
    {
        TF_ScopeLock<F_SpinLock> _(_GenerationBuffer.Lock);
        FreeGeneration(ForwardStandardObjectId(Key.IdValue).LocalIndex);
    }

    B8 F_StandardObjectKeyPage::AddTrackingFlag(F_StandardObjectKey Key, E_StandardObjectTrackingFlag Flag) noexcept
    {
        _GenerationBuffer.Lock.ConsumerLock();

        U32 LocalIndex = ForwardStandardObjectId(Key.IdValue).LocalIndex;

        if(LocalIndex >= _GenerationBuffer.Size)
        {
            _GenerationBuffer.Lock.ConsumerUnlock();
            return false;
        }

        B8 Result = (_GenerationBuffer.Generations[LocalIndex] == Key.Generation);
        if (Result)
        {
            auto& DstTrackingFlag = _GenerationBuffer.TrackingFlags[LocalIndex];
            DstTrackingFlag = DstTrackingFlag | Flag;
        }
        _GenerationBuffer.Lock.ConsumerUnlock();

        return Result;
    }
    B8 F_StandardObjectKeyPage::RemoveTrackingFlag(F_StandardObjectKey Key, E_StandardObjectTrackingFlag Flag) noexcept
    {
        _GenerationBuffer.Lock.ConsumerLock();

        U32 LocalIndex = ForwardStandardObjectId(Key.IdValue).LocalIndex;

        if(LocalIndex >= _GenerationBuffer.Size)
        {
            _GenerationBuffer.Lock.ConsumerUnlock();
            return false;
        }

        B8 Result = (_GenerationBuffer.Generations[LocalIndex] == Key.Generation);
        if (Result)
        {
            auto& DstTrackingFlag = _GenerationBuffer.TrackingFlags[LocalIndex];
            DstTrackingFlag = DstTrackingFlag & ~Flag;
        }
        _GenerationBuffer.Lock.ConsumerUnlock();

        return Result;
    }

    E_StandardObjectTrackingFlag F_StandardObjectKeyPage::GetTrackingFlag(F_StandardObjectKey Key) noexcept
    {
        _GenerationBuffer.Lock.ConsumerLock();

        U32 LocalIndex = ForwardStandardObjectId(Key.IdValue).LocalIndex;

        if(LocalIndex >= _GenerationBuffer.Size)
        {
            _GenerationBuffer.Lock.ConsumerUnlock();
            return E_StandardObjectTrackingFlag::INVALID;
        }

        B8 Result = (_GenerationBuffer.Generations[LocalIndex] == Key.Generation);
        E_StandardObjectTrackingFlag TrackingFlag = E_StandardObjectTrackingFlag::INVALID;
        if (Result)
        {
            TrackingFlag = _GenerationBuffer.TrackingFlags[LocalIndex];
        }
        _GenerationBuffer.Lock.ConsumerUnlock();

        return TrackingFlag;
    }
    B8 F_StandardObjectKeyPage::Check(F_StandardObjectKey Key) noexcept
    {
        _GenerationBuffer.Lock.ConsumerLock();

        U32 LocalIndex = ForwardStandardObjectId(Key.IdValue).LocalIndex;

        if(LocalIndex >= _GenerationBuffer.Size)
        {
            _GenerationBuffer.Lock.ConsumerUnlock();
            return false;
        }

        B8 Result = (_GenerationBuffer.Generations[LocalIndex] == Key.Generation);
        _GenerationBuffer.Lock.ConsumerUnlock();

        return Result;
    }
    B8 F_StandardObjectKeyPage::CheckExclusive(F_StandardObjectKey Key) noexcept
    {
        return !FlagHasAny(GetTrackingFlag(Key), E_StandardObjectTrackingFlag::FORBBIDEN_BY_EXCLUSIVELY_SAFE);
    }

    F_StandardObjectKeyPool F_StandardObjectKeyPool::_Instance;
    
    F_StandardObjectKeyPool::F_StandardObjectKeyPool() :
#ifdef ABYTEK_DISABLE_THREADS
        _Pages(1)
#else
        _Pages(boost::thread::physical_concurrency())
#endif
    {
    }
    F_StandardObjectKeyPool::~F_StandardObjectKeyPool()
    {
    }

    F_StandardObjectKey F_StandardObjectKeyPool::Register()
    {
        U32 PageOffsetNonRounded = ++_NextPageOffsetNonRounded;
        U32 PageIndex = PageOffsetNonRounded % _Pages.size();

        F_StandardObjectKey Result = _Pages[PageIndex].Register();
        ForwardStandardObjectId(Result.IdValue).PageIndex = PageIndex;
        
        return Result;
    }
    void F_StandardObjectKeyPool::Deregister(F_StandardObjectKey Key)
    {
        return _Pages[ForwardStandardObjectId(Key.IdValue).PageIndex].Deregister(Key);
    }
}