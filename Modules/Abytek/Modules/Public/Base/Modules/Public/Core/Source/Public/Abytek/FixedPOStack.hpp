#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/BasicMath.hpp"
#include "Abytek/Assert.hpp"
#include "Abytek/Allocator.hpp"
#include "Abytek/BoostContainerTemplates.hpp"


namespace Abytek::ContainerTemplates
{
	/**
	 * @brief A fixed-size push-only stack data structure
	 * 
	 * This container provides a stack with a maximum capacity where items
	 * can only be pushed (not popped). It's optimized for last-in-first-out access
	 * and has no concurrent access protection.
	 * 
	 * @tparam __F_Item The type of items stored in the stack
	 * @tparam __F_Allocator The allocator type used for memory management
	 */
	template<typename __F_Item, class __F_Allocator>
	class TF_FixedPOStack
	{
	public:
		using F_Item = __F_Item;
		using F_Allocator = __F_Allocator;

		using F_ItemVector = ContainerTemplates::TF_Vector<F_Item, TF_RebindAllocator<F_Item, F_Allocator>>;

	protected:
		F_ItemVector _ItemVector;

	private:
		Sz _Capacity = 0;
		Sz _EndIndex = 0;

	public:
		/**
		 * @brief Get the current number of items in the stack
		 * @return The size of the stack
		 */
		ABYTEK_FORCE_INLINE Sz GetSize() const { return _EndIndex; }
		
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
		ABYTEK_FORCE_INLINE Sz GetEndIndex() const { return _EndIndex; }
        
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
			return { _ItemVector.data() + _Capacity - GetSize(), _EndIndex };
		}
		
		/**
		 * @brief Get a span of the items in the stack
		 * @return Span of stack items
		 */
		ABYTEK_FORCE_INLINE boost::span<F_Item> GetItemSpan() noexcept
		{
			return { _ItemVector.data() + _Capacity - GetSize(), _EndIndex };
		}

	public:
		/**
		 * @brief Default constructor
		 */
		ABYTEK_FORCE_INLINE TF_FixedPOStack() noexcept = default;
		
		/**
		 * @brief Construct with specified capacity and allocator
		 * @param Capacity The maximum number of items the stack can hold
		 * @param Allocator The allocator to use
		 */
		TF_FixedPOStack(Sz Capacity, const F_Allocator& Allocator) :
			_Capacity(Capacity),
			_ItemVector(Allocator)
		{
			_ItemVector.reserve(Capacity);
		}
		
		/**
		 * @brief Construct with specified capacity using default allocator
		 * @param Capacity The maximum number of items the stack can hold
		 */
		TF_FixedPOStack(Sz Capacity) :
			TF_FixedPOStack(Capacity, {})
		{
		}
		
		/**
		 * @brief Copy constructor
		 * @param x The stack to copy from
		 */
		TF_FixedPOStack(const TF_FixedPOStack& x) :
			_ItemVector(x._ItemVector),
			_Capacity(x._Capacity),
			_EndIndex(x._EndIndex)
		{
		}
		
		/**
		 * @brief Copy assignment operator
		 * @param x The stack to copy from
		 * @return Reference to this object
		 */
		TF_FixedPOStack& operator = (const TF_FixedPOStack& x)
		{
			_ItemVector = x._ItemVector;
			_Capacity = x._Capacity;
			_EndIndex = x._EndIndex;
            return *this;
		}
		
		/**
		 * @brief Move constructor
		 * @param x The stack to move from
		 */
		TF_FixedPOStack(TF_FixedPOStack&& x) :
			_ItemVector(std::move(x._ItemVector)),
			_Capacity(x._Capacity),
			_EndIndex(x._EndIndex)
		{
			x.Reset();
		}
		
		/**
		 * @brief Move assignment operator
		 * @param x The stack to move from
		 * @return Reference to this object
		 */
		TF_FixedPOStack& operator = (TF_FixedPOStack&& x)
		{
			_ItemVector = std::move(x._ItemVector);
			_Capacity = x._Capacity;
			_EndIndex = x._EndIndex;
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
			Sz Location = _Capacity - 1 - _EndIndex;

			++_EndIndex;

			ABYTEK_BASE_CORE_ASSERT(_EndIndex <= _Capacity) << "Capacity exceeded";

			_ItemVector.data()[Location] = std::forward<__F_PassedItem>(item);

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
			for(U32 Idx = 0; Idx < _EndIndex; ++Idx)
			{
				_ItemVector.data()[_Capacity - 1 - Idx].~F_Item();
			}
			_EndIndex = 0;
		}
	};
}