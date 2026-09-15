#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/BasicMath.hpp"
#include "Abytek/Assert.hpp"
#include "Abytek/Allocator.hpp"
#include "Abytek/SpinLock.hpp"
#include "Abytek/BoostContainerTemplates.hpp"


namespace Abytek::ContainerTemplates
{
	/**
	 * @brief A thread-safe fixed-size push-only stack data structure
	 * 
	 * This container allows concurrent pushing of items while maintaining thread safety.
	 * Items can only be pushed, not popped (Push-Only).
	 * 
	 * @tparam __F_Item The type of items stored in the stack
	 * @tparam __F_Allocator The allocator type used for memory management
	 */
	template<typename __F_Item, class __F_Allocator>
	class TF_ConcurrentFixedPOStack
	{
	public:
		using F_Item = __F_Item;
		using F_Allocator = __F_Allocator;

		using F_ItemVector = ContainerTemplates::TF_Vector<F_Item, TF_RebindAllocator<F_Item, F_Allocator>>;

	protected:
		F_ItemVector _ItemVector;

	private:
		Sz _Capacity = 0;
		ASz _EndIndex = 0;

	public:
		/**
		 * @brief Get the current number of items in the stack
		 * @return The size of the stack
		 */
		ABYTEK_FORCE_INLINE Sz GetSize() const { return _EndIndex.load(boost::memory_order_acquire); }
		
		/**
		 * @brief Get the maximum capacity of the stack
		 * @return The capacity of the stack
		 */
		ABYTEK_FORCE_INLINE Sz GetCapacity() const { return _Capacity; }
		
		/**
		 * @brief Check if the stack is empty
		 * @return True if the stack is empty, false otherwise
		 */
		ABYTEK_FORCE_INLINE bool IsEmpty() const { return !GetSize(); }
		
		/**
		 * @brief Check if the stack has been initialized
		 * @return True if the stack has not been initialized, false otherwise
		 */
		ABYTEK_FORCE_INLINE bool IsNull() const { return !_Capacity; }
        
		/**
		 * @brief Get the current end index position
		 * @return The end index
		 */
		ABYTEK_FORCE_INLINE Sz GetEndIndex() const { return _EndIndex.load(boost::memory_order_acquire); }
        
		/**
		 * @brief Get the underlying item vector (const version)
		 * @return Constant reference to the item vector
		 */
		ABYTEK_FORCE_INLINE const F_ItemVector& GetItemVector() const noexcept { return _ItemVector; }
		
		/**
		 * @brief Get the underlying item vector
		 * @return Reference to the item vector
		 */
		ABYTEK_FORCE_INLINE F_ItemVector& GetItemVector() noexcept { return _ItemVector; }
		
		/**
		 * @brief Get a span of the items in the stack (const version)
		 * @return Constant span of stack items
		 */
		ABYTEK_FORCE_INLINE boost::span<const F_Item> GetItemSpan() const noexcept
		{
			return { _ItemVector.data() + _Capacity - GetSize(), GetEndIndex() };
		}
		
		/**
		 * @brief Get a span of the items in the stack
		 * @return Span of stack items
		 */
		ABYTEK_FORCE_INLINE boost::span<F_Item> GetItemSpan() noexcept
		{
			return { _ItemVector.data() + _Capacity - GetSize(), GetEndIndex() };
		}

	public:
		/**
		 * @brief Default constructor
		 */
		ABYTEK_FORCE_INLINE TF_ConcurrentFixedPOStack() noexcept = default;
		
		/**
		 * @brief Construct with specified capacity and allocator
		 * @param Capacity The maximum number of items the stack can hold
		 * @param Allocator The allocator to use
		 */
		TF_ConcurrentFixedPOStack(Sz Capacity, const F_Allocator& Allocator) :
			_Capacity(Capacity),
			_ItemVector(Allocator)
		{
			_ItemVector.reserve(Capacity);
		}
		
		/**
		 * @brief Construct with specified capacity using default allocator
		 * @param Capacity The maximum number of items the stack can hold
		 */
		TF_ConcurrentFixedPOStack(Sz Capacity) :
			TF_ConcurrentFixedPOStack(Capacity, {})
		{
		}
		
		/**
		 * @brief Copy constructor
		 * @param x The stack to copy from
		 */
		TF_ConcurrentFixedPOStack(const TF_ConcurrentFixedPOStack& x) :
			_ItemVector(x._ItemVector),
			_Capacity(x._Capacity),
			_EndIndex(x.GetEndIndex())
		{
		}
		
		/**
		 * @brief Copy assignment operator
		 * @param x The stack to copy from
		 * @return Reference to this object
		 */
		TF_ConcurrentFixedPOStack& operator = (const TF_ConcurrentFixedPOStack& x)
		{
			_ItemVector = x._ItemVector;
			_Capacity = x._Capacity;
			_EndIndex = x.GetEndIndex();
            return *this;
		}
		
		/**
		 * @brief Move constructor
		 * @param x The stack to move from
		 */
		TF_ConcurrentFixedPOStack(TF_ConcurrentFixedPOStack&& x) :
			_ItemVector(std::move(x._ItemVector)),
			_Capacity(x._Capacity),
			_EndIndex(x.GetEndIndex())
		{
			x.Reset();
		}
		
		/**
		 * @brief Move assignment operator
		 * @param x The stack to move from
		 * @return Reference to this object
		 */
		TF_ConcurrentFixedPOStack& operator = (TF_ConcurrentFixedPOStack&& x)
		{
			_ItemVector = std::move(x._ItemVector);
			_Capacity = x._Capacity;
			_EndIndex = x.GetEndIndex();
			x.Reset();
            return *this;
		}
	
	private:
		/**
		 * @brief Internal implementation of push operation
		 * @tparam __F_PassedItem Type of the item being passed
		 * @param item The item to push
		 * @return The index where the item was placed
		 */
		template<typename __F_PassedItem>
		Sz _Push(__F_PassedItem&& item)
		{
			Sz Location = _Capacity - 1 - _EndIndex.fetch_add(1, boost::memory_order_relaxed);

			new(_ItemVector.data() + Location) F_Item(
				std::forward<__F_PassedItem>(item)
			);

			ABYTEK_BASE_CORE_ASSERT(Location < _Capacity) << "capacity exceeded";

			return Location;
		}
	
	public:
		/**
		 * @brief Push a copy of an item onto the stack
		 * @param item The item to push
		 * @return The index where the item was placed
		 */
		ABYTEK_FORCE_INLINE Sz Push(F_Item const& item)
		{
			return _Push(std::forward<F_Item const&>(item));
		}
		
		/**
		 * @brief Push an item onto the stack using move semantics
		 * @param item The item to push
		 * @return The index where the item was placed
		 */
		ABYTEK_FORCE_INLINE Sz Push(F_Item&& item)
		{
			return _Push(std::forward<F_Item>(item));
		}
		
		/**
		 * @brief Reset the stack to an empty state
		 * Destroys all items and resets the end index to 0
		 */
		ABYTEK_FORCE_INLINE void Reset()
		{
			Sz end_index = _EndIndex.load(boost::memory_order_relaxed);
			for(U32 Idx = 0; Idx < end_index; ++Idx)
			{
				_ItemVector.data()[_Capacity - 1 - Idx].~F_Item();
			}
			_EndIndex.store(0, boost::memory_order_relaxed);
		}
	};
}