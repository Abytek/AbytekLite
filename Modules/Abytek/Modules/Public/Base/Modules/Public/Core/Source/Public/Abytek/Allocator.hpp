/**
 * @file Allocator.hpp
 * @brief Provides template-based allocator utilities for memory management.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/MemoryAllocationInfo.hpp"
#include "Abytek/Assert.hpp"


namespace Abytek
{
    /**
     * @brief Contains basic allocation options that control allocator behavior.
     */
    struct F_AllocatorOptions
    {
        static constexpr B8 PropagateOnContainerCopyAssignment = true;
        static constexpr B8 PropagateOnContainerMoveAssignment = true;
        static constexpr B8 IsAlwaysEqual = true;
        static constexpr B8 EnableTracking = true;
    };

    /**
     * @brief Generic allocator template that serves as a base for specific allocators.
     * 
     * @tparam __F_Allocator The derived allocator type implementing the allocation logic.
     * @tparam __F_Value The value type this allocator works with.
     * @tparam __F_Options Configuration options for the allocator.
     */
    template<
        class __F_Allocator,
        class __F_Value,
        typename __F_Options = F_AllocatorOptions
    >
    struct TA_Allocator
    {
    private:
        /**
         * @brief Helper template for boolean constants.
         * @tparam V Boolean value.
         */
        template<bool V>
        struct TF_BoolConstant
        {
            typedef bool value_type;
            typedef TF_BoolConstant type;

            static const bool value = V;

            operator bool() const BOOST_NOEXCEPT
            {
                return V;
            }
            bool operator()() const BOOST_NOEXCEPT
            {
                return V;
            }
        };

    public:
        using F_Allocator = __F_Allocator;

        using F_Value = __F_Value;
        using F_ValueP = __F_Value*;
        using F_ValueConstP = __F_Value const*;
        using F_ValueR = __F_Value&;
        using F_ValueConstR = __F_Value const&;
        static constexpr Sz ValueSize = sizeof(F_Value);
        static constexpr Sz ValueAlignment = ABYTEK_ALIGNOF(F_Value);

        using F_Options = __F_Options;

        // STL-compatible type definitions
        typedef __F_Value value_type;
        typedef __F_Value* pointer;
        typedef const __F_Value* const_pointer;
        typedef __F_Value& reference;
        typedef const __F_Value& const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef TF_BoolConstant<F_Options::PropagateOnContainerCopyAssignment> propagate_on_container_copy_assignment;
        typedef TF_BoolConstant<F_Options::PropagateOnContainerMoveAssignment> propagate_on_container_move_assignment;
        typedef TF_BoolConstant<F_Options::IsAlwaysEqual> is_always_equal;

    protected:
        constexpr TA_Allocator() noexcept = default;

    public:
        /**
         * @brief Allocates memory for n objects of type F_Value.
         * @param n Number of objects to allocate memory for.
         * @return Pointer to the allocated memory.
         */
        ABYTEK_FORCE_INLINE F_Value* allocate(std::size_t n)
        {
            return ((F_Allocator*)this)->Allocate(n);
        }
        
        /**
         * @brief Deallocates memory previously allocated by this allocator.
         * @param p Pointer to the memory to deallocate.
         * @param n Number of objects that were allocated (optional).
         */
        ABYTEK_FORCE_INLINE void deallocate(F_Value* p, std::size_t n = 0)
        {
            ((F_Allocator*)this)->Deallocate(p, n);
        }

    public:
        /**
         * @brief Allocates memory for Count objects of type F_Value.
         * 
         * This version includes memory tracking functionality when enabled.
         * 
         * @param Count Number of objects to allocate memory for.
         * @return Pointer to the allocated memory.
         */
        ABYTEK_FORCE_INLINE F_Value* Allocate(Sz Count)
        {
            ABYTEK_BASE_CORE_ASSERT(Count);

            Sz SizeInBytes = Count * sizeof(F_Value);

#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
            Sz PayloadSize = SizeInBytes;
            Sz ActualSize = PayloadSize + HeaderSize;
            SizeInBytes = ActualSize;

            Analyze_IncreaseActualUsage(ActualSize);
            Analyze_IncreasePayloadUsage(PayloadSize);

            void* RawP = (void*)malloc(SizeInBytes);
            ((Sz*)RawP)[0] = ActualSize;
            ((Sz*)RawP)[1] = PayloadSize;

            return (F_Value*)(((U8*)RawP) + HeaderSize);
#else
            return (F_Value*)malloc(SizeInBytes);
#endif
        }
        
        /**
         * @brief Deallocates memory previously allocated by this allocator.
         * 
         * This version includes memory tracking functionality when enabled.
         * 
         * @param Pointer Pointer to the memory to deallocate.
         * @param Optional size parameter (unused).
         */
        ABYTEK_FORCE_INLINE void Deallocate(void* Pointer, Sz = 0)
        {
            ABYTEK_BASE_CORE_ASSERT(Pointer);

#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
            Sz ActualSize = _Analyze_GetActualSize(Pointer);
            Sz PayloadSize = _Analyze_GetPayloadSize(Pointer);
            Analyze_DecreaseActualUsage(ActualSize);
            Analyze_DecreasePayloadUsage(PayloadSize);

            void* RawP = ((U8*)Pointer) - HeaderSize;

            free(RawP);
#else
            free(Pointer);
#endif
        }

#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
    public:
        /**
         * @brief Tracks increase in actual memory usage.
         * @param Bytes Number of bytes to add to usage counter.
         */
        ABYTEK_FORCE_INLINE void Analyze_IncreaseActualUsage(Sz Bytes)
        {
        }
        
        /**
         * @brief Tracks increase in payload memory usage.
         * @param Bytes Number of bytes to add to usage counter.
         */
        ABYTEK_FORCE_INLINE void Analyze_IncreasePayloadUsage(Sz Bytes)
        {
        }
        
        /**
         * @brief Tracks decrease in actual memory usage.
         * @param Bytes Number of bytes to subtract from usage counter.
         */
        ABYTEK_FORCE_INLINE void Analyze_DecreaseActualUsage(Sz Bytes)
        {
        }
        
        /**
         * @brief Tracks decrease in payload memory usage.
         * @param Bytes Number of bytes to subtract from usage counter.
         */
        ABYTEK_FORCE_INLINE void Analyze_DecreasePayloadUsage(Sz Bytes)
        {
        }

    private:
        static constexpr Sz HeaderSize = Max<Sz>(ValueAlignment, 2 * sizeof(Sz));
        
        /**
         * @brief Gets the actual allocation size from a memory pointer.
         * @param Pointer Pointer to allocated memory.
         * @return The actual size of the allocation in bytes.
         */
        ABYTEK_FORCE_INLINE Sz _Analyze_GetActualSize(void* Pointer)
        {
            void* RawP = ((U8*)Pointer) - HeaderSize;
            return ((Sz*)RawP)[0];
        }
        
        /**
         * @brief Gets the payload size from a memory pointer.
         * @param Pointer Pointer to allocated memory.
         * @return The payload size of the allocation in bytes.
         */
        ABYTEK_FORCE_INLINE Sz _Analyze_GetPayloadSize(void* Pointer)
        {
            void* RawP = ((U8*)Pointer) - HeaderSize;
            return ((Sz*)RawP)[1];
        }
#endif

    public:
        /**
         * @brief Creates a new object of type F_Value with the given arguments.
         * @tparam __Fs Argument types for constructor.
         * @param Args Arguments to forward to the constructor.
         * @return Pointer to the new object.
         */
        template<typename... __Fs>
        F_Value* New(__Fs&&... Args)
        {
            return new(allocate(1)) F_Value(ABYTEK_FORWARD(Args)...);
        }
        
        /**
         * @brief Creates an array of objects of type F_Value.
         * @param Count Number of objects to create.
         * @return Pointer to the array.
         */
        F_Value* NewArray(Sz Count)
        {
            return new(allocate(Count)) F_Value[Count];
        }
        
        /**
         * @brief Destroys an object and deallocates its memory.
         * @param Pointer Pointer to the object to delete.
         */
        void Delete(F_Value* Pointer)
        {
            Pointer->~F_Value();
            deallocate(Pointer);
        }
        
        /**
         * @brief Destroys an array of objects and deallocates their memory.
         * @param Pointer Pointer to the array to delete.
         * @param Count Number of objects in the array.
         */
        void DeleteArray(F_Value* Pointer, Sz Count)
        {
            for(Sz Idx = 0; Idx < Count; ++Idx)
            {
                Pointer[Idx].~F_Value();
            }
            deallocate(Pointer);
        }
    };

    /**
     * @brief Specialization of TA_Allocator for void type.
     * 
     * @tparam __F_Allocator The derived allocator type.
     * @tparam __F_Options Configuration options for the allocator.
     */
    template<
        class __F_Allocator,
        typename __F_Options
    >
    struct TA_Allocator<__F_Allocator, void, __F_Options>
    {
    private:
        /**
         * @brief Helper template for boolean constants.
         * @tparam V Boolean value.
         */
        template<bool V>
        struct TF_BoolConstant
        {
            typedef bool value_type;
            typedef TF_BoolConstant type;

            static const bool value = V;

            operator bool() const BOOST_NOEXCEPT
            {
                return V;
            }
            bool operator()() const BOOST_NOEXCEPT
            {
                return V;
            }
        };

    public:
        using F_Allocator = __F_Allocator;

        using F_Value = void;
        using F_ValueP = F_Value*;
        using F_ValueConstP = F_Value const*;
        static constexpr Sz ValueSize = 1;
        static constexpr Sz ValueAlignment = ABYTEK_MIN_MEMORY_ALIGNMENT;

        using F_Options = __F_Options;

        // STL-compatible type definitions
        typedef F_Value value_type;
        typedef F_Value* pointer;
        typedef const F_Value* const_pointer;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef TF_BoolConstant<F_Options::PropagateOnContainerCopyAssignment> propagate_on_container_copy_assignment;
        typedef TF_BoolConstant<F_Options::PropagateOnContainerMoveAssignment> propagate_on_container_move_assignment;
        typedef TF_BoolConstant<F_Options::IsAlwaysEqual> is_always_equal;

    protected:
        constexpr TA_Allocator() noexcept = default;

    public:
        /**
         * @brief Allocates memory block of n bytes.
         * @param n Number of bytes to allocate.
         * @return Pointer to the allocated memory.
         */
        ABYTEK_FORCE_INLINE void* allocate(std::size_t n)
        {
            return ((F_Allocator*)this)->Allocate(n);
        }
        
        /**
         * @brief Deallocates memory previously allocated by this allocator.
         * @param p Pointer to the memory to deallocate.
         * @param n Number of bytes that were allocated (optional).
         */
        ABYTEK_FORCE_INLINE void deallocate(void* p, std::size_t n = 0)
        {
            ((F_Allocator*)this)->Deallocate(p, n);
        }

    public:
        /**
         * @brief Allocates a memory block of Count bytes.
         * 
         * This version includes memory tracking functionality when enabled.
         * 
         * @param Count Number of bytes to allocate.
         * @return Pointer to the allocated memory.
         */
        ABYTEK_FORCE_INLINE void* Allocate(Sz Count)
        {
            ABYTEK_BASE_CORE_ASSERT(Count);

            Sz SizeInBytes = Count * 1;

#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_COUNTING
            Sz PayloadSize = SizeInBytes;
            Sz ActualSize = PayloadSize + HeaderSize;
            SizeInBytes = ActualSize;

            Analyze_IncreaseActualUsage(ActualSize);
            Analyze_IncreasePayloadUsage(PayloadSize);

            void* RawP = (void*)malloc(SizeInBytes);
            ((Sz*)RawP)[0] = ActualSize;
            ((Sz*)RawP)[1] = PayloadSize;

            return (F_Value*)(((U8*)RawP) + HeaderSize);
#else
            return (F_Value*)malloc(SizeInBytes);
#endif
        }
        
        /**
         * @brief Deallocates memory previously allocated by this allocator.
         * 
         * This version includes memory tracking functionality when enabled.
         * 
         * @param Pointer Pointer to the memory to deallocate.
         * @param Optional size parameter (unused).
         */
        ABYTEK_FORCE_INLINE void Deallocate(void* Pointer, Sz = 0)
        {
            ABYTEK_BASE_CORE_ASSERT(Pointer);

#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_COUNTING
            Sz ActualSize = _Analyze_GetActualSize(Pointer);
            Sz PayloadSize = _Analyze_GetPayloadSize(Pointer);
            Analyze_DecreaseActualUsage(ActualSize);
            Analyze_DecreasePayloadUsage(PayloadSize);

            void* RawP = ((U8*)Pointer) - HeaderSize;

            free(RawP);
#else
            free(Pointer);
#endif
        }

#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_COUNTING
    public:
        /**
         * @brief Tracks increase in actual memory usage.
         * @param Bytes Number of bytes to add to usage counter.
         */
        ABYTEK_FORCE_INLINE void Analyze_IncreaseActualUsage(Sz Bytes)
        {
        }
        
        /**
         * @brief Tracks increase in payload memory usage.
         * @param Bytes Number of bytes to add to usage counter.
         */
        ABYTEK_FORCE_INLINE void Analyze_IncreasePayloadUsage(Sz Bytes)
        {
        }
        
        /**
         * @brief Tracks decrease in actual memory usage.
         * @param Bytes Number of bytes to subtract from usage counter.
         */
        ABYTEK_FORCE_INLINE void Analyze_DecreaseActualUsage(Sz Bytes)
        {
        }
        
        /**
         * @brief Tracks decrease in payload memory usage.
         * @param Bytes Number of bytes to subtract from usage counter.
         */
        ABYTEK_FORCE_INLINE void Analyze_DecreasePayloadUsage(Sz Bytes)
        {
        }

    private:
        static constexpr Sz HeaderSize = Max<Sz>(ValueAlignment, 2 * sizeof(Sz));
        
        /**
         * @brief Gets the actual allocation size from a memory pointer.
         * @param Pointer Pointer to allocated memory.
         * @return The actual size of the allocation in bytes.
         */
        ABYTEK_FORCE_INLINE Sz _Analyze_GetActualSize(void* Pointer)
        {
            void* RawP = ((U8*)Pointer) - HeaderSize;
            return ((Sz*)RawP)[0];
        }
        
        /**
         * @brief Gets the payload size from a memory pointer.
         * @param Pointer Pointer to allocated memory.
         * @return The payload size of the allocation in bytes.
         */
        ABYTEK_FORCE_INLINE Sz _Analyze_GetPayloadSize(void* Pointer)
        {
            void* RawP = ((U8*)Pointer) - HeaderSize;
            return ((Sz*)RawP)[1];
        }
#endif

    public:
        /**
         * @brief Placeholder for creating objects (not used for void type).
         * @return Always returns null pointer.
         */
        template<typename... __Fs>
        F_Value* New(__Fs&&... Args)
        {
            return 0;
        }
        
        /**
         * @brief Placeholder for creating arrays (not used for void type).
         * @return Always returns null pointer.
         */
        F_Value* NewArray(Sz Count)
        {
            return 0;
        }
        
        /**
         * @brief Placeholder for destroying objects (not used for void type).
         */
        void Delete(F_Value* Pointer)
        {
        }
        
        /**
         * @brief Placeholder for destroying arrays (not used for void type).
         */
        void DeleteArray(F_Value* Pointer, Sz Count)
        {
        }
    };

    /**
     * @brief Equality comparison operator for allocators.
     * 
     * @tparam __F_Allocator The allocator type.
     * @tparam __F_Value The value type.
     * @tparam __F_Options Configuration options.
     * @param a First allocator to compare.
     * @param b Second allocator to compare.
     * @return True if allocators are equal.
     */
    template<
        class __F_Allocator,
        class __F_Value,
        typename __F_Options
    >
    ABYTEK_FORCE_INLINE B8 operator == (
        const TA_Allocator<__F_Allocator, __F_Value, __F_Options>& a,
        const TA_Allocator<__F_Allocator, __F_Value, __F_Options>& b
    ) noexcept
    {
        return (&a) == (&b);
    }
    
    /**
     * @brief Inequality comparison operator for allocators.
     * 
     * @tparam __F_Allocator The allocator type.
     * @tparam __F_Value The value type.
     * @tparam __F_Options Configuration options.
     * @param a First allocator to compare.
     * @param b Second allocator to compare.
     * @return True if allocators are not equal.
     */
    template<
        class __F_Allocator,
        class __F_Value,
        typename __F_Options
    >
    ABYTEK_FORCE_INLINE B8 operator != (
        const TA_Allocator<__F_Allocator, __F_Value, __F_Options>& a,
        const TA_Allocator<__F_Allocator, __F_Value, __F_Options>& b
    ) noexcept
    {
        return (&a) != (&b);
    }

    /**
     * @brief Type definition for rebinding an allocator to work with a different value type.
     * 
     * @tparam __F_OtherValue The new value type.
     * @tparam __F_Allocator The allocator type to rebind.
     */
    template<typename __F_OtherValue, class __F_Allocator>
    using TF_RebindAllocator = typename __F_Allocator::template rebind<__F_OtherValue>::other;

    /**
     * @brief Macro to define allocator rebind functionality.
     * 
     * This macro creates a rebind template struct that allows an allocator to be rebound to a different value type.
     */
#define ABYTEK_DEFINE_ALLOCATOR_REBIND(OtherValue, ...) \
            template<typename OtherValue>\
            struct rebind \
            { \
                using other = __VA_ARGS__; \
            }
}