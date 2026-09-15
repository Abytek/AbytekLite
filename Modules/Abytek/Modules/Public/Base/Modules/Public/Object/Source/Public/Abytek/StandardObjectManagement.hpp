/**
 * @file StandardObjectManagement.hpp
 * @brief Defines standard object management structures and classes for object lifetime tracking and reference management.
 * 
 * This file provides core components for the Abytek object reference management system, including object identifiers,
 * keys, and pools for tracking object validity throughout their lifecycle.
 */
#pragma once

#include <Abytek/Base.Object.prerequisites.pch.hpp>
#include "Abytek/RawUseAllocatorTypedefs.hpp"
#include "Abytek/SpinLock.hpp"
#include "Abytek/ObjectSmartPointerTemplates.hpp"


namespace Abytek
{
    /**
     * @brief Object identifier structure combining page and local indices.
     * 
     * Represents an object's location in the object management system with two components:
     * - PageIndex: Identifies which page contains the object
     * - LocalIndex: Identifies the object within its page
     */
    struct F_StandardObjectId
    {
        union
        {
            struct 
            {    
                U32 PageIndex : 8;
                U32 LocalIndex : 24;
            };
            U32 Value;
        };

        ABYTEK_FORCE_INLINE F_StandardObjectId() noexcept {}

        ABYTEK_FORCE_INLINE constexpr F_StandardObjectId(U32 InValue) noexcept :
            Value(InValue)
        {}
        ABYTEK_FORCE_INLINE constexpr F_StandardObjectId(U8 InPageIndex, U32 InLocalIndex) noexcept :
            PageIndex(InPageIndex),
            LocalIndex(InLocalIndex)
        {}

        friend ABYTEK_FORCE_INLINE constexpr B8 operator == (F_StandardObjectId A, F_StandardObjectId B) noexcept
        {
            return (A.PageIndex == B.PageIndex) && (A.LocalIndex == B.LocalIndex);
        }
        friend ABYTEK_FORCE_INLINE constexpr B8 operator != (F_StandardObjectId A, F_StandardObjectId B) noexcept
        {
            return (A.PageIndex != B.PageIndex) || (A.LocalIndex != B.LocalIndex);
        }

        [[nodiscard]] ABYTEK_FORCE_INLINE constexpr B8 IsValid() const noexcept
        {
            return (Value != ABYTEK_U32_MAX);
        }
        [[nodiscard]] ABYTEK_FORCE_INLINE constexpr B8 IsNull() const noexcept
        {
            return (Value == ABYTEK_U32_MAX);
        }

        [[nodiscard]] ABYTEK_FORCE_INLINE constexpr operator U64 () const noexcept
        {
            return Value;
        }
        [[nodiscard]] ABYTEK_FORCE_INLINE constexpr operator B8 () const noexcept
        {
            return IsValid();
        }

        static constexpr F_StandardObjectId Null() noexcept
        {
            return { ABYTEK_U32_MAX };
        }
    };

    using F_StandardObjectIdValue = TF_UInt<sizeof(F_StandardObjectId)>;

    ABYTEK_FORCE_INLINE F_StandardObjectId& ForwardStandardObjectId(F_StandardObjectIdValue& Value) noexcept
    {
        return *((F_StandardObjectId*)&Value);
    }
    ABYTEK_FORCE_INLINE const F_StandardObjectId& ForwardStandardObjectId(const F_StandardObjectIdValue& Value) noexcept
    {
        return *((const F_StandardObjectId*)&Value);
    }
    ABYTEK_FORCE_INLINE F_StandardObjectId&& ForwardStandardObjectId(F_StandardObjectIdValue&& Value) noexcept
    {
        return (F_StandardObjectId&&)*((F_StandardObjectId*)&Value);
    }
    ABYTEK_FORCE_INLINE const F_StandardObjectId&& ForwardStandardObjectId(const F_StandardObjectIdValue&& Value) noexcept
    {
        return (const F_StandardObjectId&&)*((const F_StandardObjectId*)&Value);
    }
    
    ABYTEK_FORCE_INLINE constexpr F_StandardObjectId ForwardStandardObjectId_Constexpr(F_StandardObjectIdValue Value) noexcept
    {
        return F_StandardObjectId(Value);
    }
    
    /**
     * @brief Extended object key combining object ID and generation counter.
     * 
     * Provides a unique identifier for an object throughout its lifecycle:
     * - Id: The object's location identifier
     * - Generation: A counter that changes when object slots are recycled, preventing use-after-free issues
     */
    struct F_StandardObjectKey
    {
        union
        {
            struct 
            {
                F_StandardObjectIdValue IdValue;
                U32 Generation;
            };
            U64 Value;
        };

        ABYTEK_FORCE_INLINE F_StandardObjectKey() noexcept {}

        ABYTEK_FORCE_INLINE constexpr F_StandardObjectKey(U64 InValue) noexcept :
            Value(InValue)
        {}
        ABYTEK_FORCE_INLINE constexpr F_StandardObjectKey(U32 InId, U32 InGeneration) noexcept :
            IdValue(InId),
            Generation(InGeneration)
        {}

        friend ABYTEK_FORCE_INLINE constexpr B8 operator == (F_StandardObjectKey A, F_StandardObjectKey B) noexcept
        {
            return (A.IdValue == B.IdValue) && (A.Generation == B.Generation);
        }
        friend ABYTEK_FORCE_INLINE constexpr B8 operator != (F_StandardObjectKey A, F_StandardObjectKey B) noexcept
        {
            return (A.IdValue != B.IdValue) || (A.Generation != B.Generation);
        }

        [[nodiscard]] ABYTEK_FORCE_INLINE constexpr B8 IsValid() const noexcept
        {
            return ForwardStandardObjectId_Constexpr(IdValue).IsValid();
        }
        [[nodiscard]] ABYTEK_FORCE_INLINE constexpr B8 IsNull() const noexcept
        {
            return ForwardStandardObjectId_Constexpr(IdValue).IsNull();
        }

        [[nodiscard]] ABYTEK_FORCE_INLINE constexpr operator U64 () const noexcept
        {
            return Value;
        }
        [[nodiscard]] ABYTEK_FORCE_INLINE constexpr operator B8 () const noexcept
        {
            return IsValid();
        }

        static constexpr F_StandardObjectKey Null() noexcept
        {
            return { ABYTEK_U64_MAX };
        }
    };

    enum class E_StandardObjectTrackingFlag : U8
    {
        NONE = 0x0,
        EARLY_DESTRUCTED = 0x1,
        INVALID = 0x2,
        FORBBIDEN_BY_EXCLUSIVELY_SAFE = EARLY_DESTRUCTED | INVALID
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_StandardObjectTrackingFlag);

    /**
     * @brief Thread-safe buffer to store generation information for object keys.
     * 
     * Manages arrays of generations, next indices, and flags used to track object validity
     * and safely recycle object slots.
     */
    struct ABYTEK_BASE_OBJECT_API F_StandardObjectKeyGenerationBuffer
    {
        WithRawAllocator::TF_Vector<U32> Generations;
        WithRawAllocator::TF_Vector<E_StandardObjectTrackingFlag> TrackingFlags;
        WithRawAllocator::TF_Vector<U32> NextIndices;
        U32 Size = 0;
        F_SpinLock Lock;

        void ResizeNonThreadSafe(U32 NewSize);
        void EnsureMinSize(U32 MinSize);
    };

    /**
     * @brief Manages a page of object keys with thread-safe generation tracking.
     * 
     * Handles registration and deregistration of objects within a single page,
     * managing generation counters to ensure object validity across their lifetime.
     */
    class ABYTEK_BASE_OBJECT_API F_StandardObjectKeyPage final
    {
    private:
        U32 _Index = 0;
        U32 _BeginIndex = 0;
        U32 _MaxGenerationCount = 0;
        U32 _FreeGenerationTailIndex = ABYTEK_U32_MAX;
        U32 _GenerationCapacity = 0;
        F_StandardObjectKeyGenerationBuffer _GenerationBuffer;

    public:
        ABYTEK_FORCE_INLINE const auto& GetGenerationBuffer() const noexcept { return _GenerationBuffer; }
        
    public:
        F_StandardObjectKeyPage();
        ~F_StandardObjectKeyPage();

        F_StandardObjectKeyPage(F_StandardObjectKeyPage&& X);
        F_StandardObjectKeyPage& operator = (F_StandardObjectKeyPage&& X);

    public:
        void Reset();

    private:
        void OptainGeneration(U32& OutGenerationIndex, U32& OutGeneration) noexcept;
        void FreeGeneration(U32 GenerationIndex) noexcept;

    public:
        F_StandardObjectKey Register();
        void Deregister(F_StandardObjectKey Key);

    public:
        B8 AddTrackingFlag(F_StandardObjectKey Key, E_StandardObjectTrackingFlag Flag) noexcept;
        B8 RemoveTrackingFlag(F_StandardObjectKey Key, E_StandardObjectTrackingFlag Flag) noexcept;

    public:
        E_StandardObjectTrackingFlag GetTrackingFlag(F_StandardObjectKey Key) noexcept;
        B8 Check(F_StandardObjectKey Key) noexcept;
        template<typename __F_Callback>
        ABYTEK_FORCE_INLINE B8 CheckAndCallbackIfValid(F_StandardObjectKey Key, __F_Callback&& Callback) noexcept
        {
            TF_ScopeLock<F_SpinLock> _(_GenerationBuffer.Lock);
            U32 LocalIndex = ForwardStandardObjectId(Key.IdValue).LocalIndex;
            if(LocalIndex >= _GenerationBuffer.Size)
            {
                return false;
            }
            if (_GenerationBuffer.Generations[LocalIndex] == Key.Generation)
            {
                Callback();
                return true;
            }
            return false;
        }
        template<typename __F_Callback>
        ABYTEK_FORCE_INLINE B8 CheckAndCallbackIfNull(F_StandardObjectKey Key, __F_Callback&& Callback) noexcept
        {
            TF_ScopeLock<F_SpinLock> _(_GenerationBuffer.Lock);
            U32 LocalIndex = ForwardStandardObjectId(Key.IdValue).LocalIndex;
            if (LocalIndex >= _GenerationBuffer.Size)
            {
                Callback();
                return false;
            }
            if (_GenerationBuffer.Generations[LocalIndex] == Key.Generation)
            {
                return true;
            }
            Callback();
            return false;
        }
        B8 CheckExclusive(F_StandardObjectKey Key) noexcept;
        template<typename __F_Callback>
        ABYTEK_FORCE_INLINE B8 CheckExclusiveAndCallbackIfValid(F_StandardObjectKey Key, __F_Callback&& Callback) noexcept
        {
            TF_ScopeLock<F_SpinLock> _(_GenerationBuffer.Lock);
            U32 LocalIndex = ForwardStandardObjectId(Key.IdValue).LocalIndex;
            if(LocalIndex >= _GenerationBuffer.Size)
            {
                return false;
            }
            if(_GenerationBuffer.Generations[LocalIndex] == Key.Generation)
            {
                if (!FlagHasAny(_GenerationBuffer.TrackingFlags[LocalIndex], E_StandardObjectTrackingFlag::FORBBIDEN_BY_EXCLUSIVELY_SAFE))
                {
                    Callback();
                    return true;
                }
            }
            return false;
        }
    };

    /**
     * @brief Singleton pool managing multiple pages of object keys.
     * 
     * Distributes object registrations across multiple pages to improve concurrency
     * and scale to large numbers of objects.
     */
    class ABYTEK_BASE_OBJECT_API F_StandardObjectKeyPool final
    {
    private:
        static F_StandardObjectKeyPool _Instance;

    public:
        static ABYTEK_FORCE_INLINE F_StandardObjectKeyPool& GetInstance() noexcept { return _Instance; }
        
    private:
        WithRawAllocator::TF_Vector<F_StandardObjectKeyPage> _Pages;
        U32 _NextPageOffsetNonRounded = 0;

    public:
        ABYTEK_FORCE_INLINE const auto& GetPages() const noexcept { return _Pages; }
        
    public:
        F_StandardObjectKeyPool();
        ~F_StandardObjectKeyPool();

    public:
        F_StandardObjectKey Register();
        void Deregister(F_StandardObjectKey Key);

    public:
        ABYTEK_FORCE_INLINE B8 AddTrackingFlag(F_StandardObjectKey Key, E_StandardObjectTrackingFlag Flag) noexcept
        {
            return _Pages[ForwardStandardObjectId(Key.IdValue).PageIndex].AddTrackingFlag(Key, Flag);
        }
        ABYTEK_FORCE_INLINE B8 RemoveTrackingFlag(F_StandardObjectKey Key, E_StandardObjectTrackingFlag Flag) noexcept
        {
            return _Pages[ForwardStandardObjectId(Key.IdValue).PageIndex].RemoveTrackingFlag(Key, Flag);
        }

    public:
        ABYTEK_FORCE_INLINE E_StandardObjectTrackingFlag GetTrackingFlag(F_StandardObjectKey Key) noexcept
        {
            if (ForwardStandardObjectId(Key.IdValue).PageIndex >= _Pages.size())
            {
                return E_StandardObjectTrackingFlag::INVALID;
            }
            return _Pages[ForwardStandardObjectId(Key.IdValue).PageIndex].GetTrackingFlag(Key);
        }
        ABYTEK_FORCE_INLINE B8 Check(F_StandardObjectKey Key) noexcept
        {
            if (ForwardStandardObjectId(Key.IdValue).PageIndex >= _Pages.size())
            {
                return false;
            }
            return _Pages[ForwardStandardObjectId(Key.IdValue).PageIndex].Check(Key);
        }
        template<typename __F_Callback>
        ABYTEK_FORCE_INLINE B8 CheckAndCallbackIfValid(F_StandardObjectKey Key, __F_Callback&& Callback) noexcept
        {
            return _Pages[ForwardStandardObjectId(Key.IdValue).PageIndex].CheckAndCallbackIfValid(Key, Callback);
        }
        template<typename __F_Callback>
        ABYTEK_FORCE_INLINE B8 CheckAndCallbackIfNull(F_StandardObjectKey Key, __F_Callback&& Callback) noexcept
        {
            return _Pages[ForwardStandardObjectId(Key.IdValue).PageIndex].CheckAndCallbackIfNull(Key, Callback);
        }
        ABYTEK_FORCE_INLINE B8 CheckExclusive(F_StandardObjectKey Key) noexcept
        {
            return _Pages[ForwardStandardObjectId(Key.IdValue).PageIndex].CheckExclusive(Key);
        }
        template<typename __F_Callback>
        ABYTEK_FORCE_INLINE B8 CheckExclusiveAndCallbackIfValid(F_StandardObjectKey Key, __F_Callback&& Callback) noexcept
        {
            return _Pages[ForwardStandardObjectId(Key.IdValue).PageIndex].CheckExclusiveAndCallbackIfValid(Key, Callback);
        }
    };

    

    /**
     * @brief Configuration type providing standard object management for smart pointers.
     * 
     * Used by the object smart pointer templates to access the standard object key
     * and pool implementations for reference tracking.
     */
    struct F_StandardObjectManagement
    {
        using F_ObjectKey = F_StandardObjectKey;
        using F_ObjectKeyPool = F_StandardObjectKeyPool;
        using E_ObjectTrackingFlag = E_StandardObjectTrackingFlag;
    };
    
    /**
     * @brief Pre-configured object pointer configuration using standard object management.
     */
    using F_StandardObjectPointerConfig = TF_ObjectPointerConfig<F_StandardObjectManagement>;
}

namespace boost
{
    template<>
    struct hash<Abytek::F_StandardObjectKey>
    {
        size_t operator()(Abytek::F_StandardObjectKey X) const
        {
            return X.Value;
        }
    };
}