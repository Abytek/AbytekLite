#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/BasicMath.hpp"
#include "Abytek/Assert.hpp"
#include "Abytek/Allocator.hpp"
#include "Abytek/BoostContainerTemplates.hpp"


namespace Abytek::ContainerTemplates
{
    /**
     * @brief Circular buffer implementation with fixed capacity
     * 
     * A ring buffer (circular buffer) is a fixed-size buffer that 
     * wraps around when full, making it efficient for FIFO operations
     * where old data can be overwritten.
     * 
     * @tparam __F_Item The type of items stored in the buffer
     * @tparam __F_Allocator The allocator type to use
     */
	template<typename __F_Item, class __F_Allocator>
	class TF_RingBuffer
	{
	public:
		using F_Item = __F_Item;
		using F_Allocator = __F_Allocator;

		using F_ItemVector = ContainerTemplates::TF_Vector<F_Item, TF_RebindAllocator<__F_Item, F_Allocator>>;

	protected:
		F_ItemVector _ItemVector;
		
	private:
		Sz _Capacity = 0;
		PDiff BeginIndex_ = 0;
		PDiff _EndIndex = 0;

	public:
	    /**
	     * @brief Gets the current number of elements in the buffer
	     * @return The size of the buffer
	     */
		ABYTEK_FORCE_INLINE Sz GetSize() const { return _EndIndex - BeginIndex_; }
		
		/**
		 * @brief Gets the maximum capacity of the buffer
		 * @return The capacity of the buffer
		 */
		ABYTEK_FORCE_INLINE Sz GetCapacity() const { return _Capacity; }
		
		/**
		 * @brief Checks if the buffer is empty
		 * @return True if the buffer is empty, false otherwise
		 */
		ABYTEK_FORCE_INLINE bool IsEmpty() const { return !GetSize(); }
		
		/**
		 * @brief Checks if the buffer has zero capacity
		 * @return True if the buffer has no capacity, false otherwise
		 */
		ABYTEK_FORCE_INLINE bool IsNull() const { return !_Capacity; }
		
		/**
		 * @brief Gets the current begin index
		 * @return The begin index
		 */
        ABYTEK_FORCE_INLINE PDiff GetBeginIndex() const { return BeginIndex_; }
        
        /**
         * @brief Gets the current end index
         * @return The end index
         */
        ABYTEK_FORCE_INLINE PDiff GetEndIndex() const { return _EndIndex; }
        
        /**
         * @brief Gets the underlying item vector (const version)
         * @return Const reference to the item vector
         */
        ABYTEK_FORCE_INLINE const F_ItemVector& GetItemVector() const noexcept { return _ItemVector; }
        
        /**
         * @brief Gets the underlying item vector
         * @return Reference to the item vector
         */
		ABYTEK_FORCE_INLINE F_ItemVector& GetItemVector() noexcept { return _ItemVector; }

	public:
		/** @brief Default constructor */
		ABYTEK_FORCE_INLINE TF_RingBuffer() noexcept = default;
		
		/**
		 * @brief Constructs a ring buffer with specified capacity and allocator
		 * @param Capacity The capacity of the buffer
		 * @param Allocator The allocator to use
		 */
		TF_RingBuffer(Sz Capacity, const F_Allocator& Allocator) :
			_Capacity(Capacity),
			_ItemVector(Allocator)
		{
			_ItemVector.reserve(Capacity);
		}
		
		/**
		 * @brief Constructs a ring buffer with specified capacity and default allocator
		 * @param Capacity The capacity of the buffer
		 */
		TF_RingBuffer(Sz Capacity) :
			TF_RingBuffer(Capacity, {})
		{
		}
		
		/**
		 * @brief Copy constructor
		 * @param x The ring buffer to copy from
		 */
		TF_RingBuffer(const TF_RingBuffer& x) :
			_ItemVector(x._ItemVector),
			_Capacity(x._Capacity),
			BeginIndex_(x.BeginIndex_),
			_EndIndex(x._EndIndex)
		{
		}
		
		/**
		 * @brief Copy assignment operator
		 * @param x The ring buffer to copy from
		 * @return Reference to this ring buffer
		 */
		TF_RingBuffer& operator = (const TF_RingBuffer& x)
		{
			_ItemVector = x._ItemVector;
			_Capacity = x._Capacity;
			BeginIndex_ = x.BeginIndex_;
			_EndIndex = x._EndIndex;
            return *this;
		}
		
		/**
		 * @brief Move constructor
		 * @param x The ring buffer to move from
		 */
		TF_RingBuffer(TF_RingBuffer&& x) :
			_ItemVector(std::move(x._ItemVector)),
			_Capacity(x._Capacity),
			BeginIndex_(x.BeginIndex_),
			_EndIndex(x._EndIndex)
		{
			x.Reset();
		}
		
		/**
		 * @brief Move assignment operator
		 * @param x The ring buffer to move from
		 * @return Reference to this ring buffer
		 */
		TF_RingBuffer& operator = (TF_RingBuffer&& x)
		{
			_ItemVector = std::move(x._ItemVector);
			_Capacity = x._Capacity;
			BeginIndex_ = x.BeginIndex_;
			_EndIndex = x._EndIndex;
			x.Reset();
            return *this;
		}
		
		/**
		 * @brief Destructor
		 */
		~TF_RingBuffer()
		{
			Reset();
		}
	
	private:
		/**
		 * @brief Internal push implementation
		 * @param item The item to push
		 * @return The location where the item was pushed
		 */
		template<typename __F_PassedItem>
		Sz _Push(__F_PassedItem&& item)
		{
			Sz Location = _EndIndex % _Capacity;

			++_EndIndex;

			new(_ItemVector.data() + Location) F_Item(
				std::forward<__F_PassedItem>(item)
			);
			return Location;
		}

	public:
		/**
		 * @brief Pushes a copy of an item into the buffer
		 * @param item The item to push
		 * @return The location where the item was pushed
		 */
		ABYTEK_FORCE_INLINE Sz Push(F_Item const& item)
		{
			return _Push(std::forward<F_Item const&>(item));
		}
		
		/**
		 * @brief Pushes an item by moving it into the buffer
		 * @param item The item to move and push
		 * @return The location where the item was pushed
		 */
		ABYTEK_FORCE_INLINE Sz Push(F_Item&& item)
		{
			return _Push(std::forward<F_Item>(item));
		}
		
		/**
		 * @brief Pops an item from the buffer
		 * @return The popped item
		 */
		F_Item Pop()
		{
			ABYTEK_BASE_CORE_ASSERT(GetSize() > 0) << "ring buffer is empty";

			Sz Location = BeginIndex_ % _Capacity;
			++BeginIndex_;

			return std::move(
				_ItemVector.data()[Location]
			);
		}
		
		/**
		 * @brief Tries to pop an item from the buffer
		 * @param item Reference to store the popped item
		 * @return True if an item was popped, false if the buffer was empty
		 */
		B8 TryPop(F_Item& item)
		{
			if (GetSize() > 0)
			{
				Sz Location = BeginIndex_ % _Capacity;
				++BeginIndex_;

				item = std::move(
					_ItemVector.data()[Location]
				);
				return true;
			}
			return false;
		}
		
		/**
		 * @brief Resets the buffer to empty state
		 */
		void Reset()
		{
			Sz Begin = GetBeginIndex();
			Sz End = GetEndIndex();

			for(Sz Idx = Begin; Idx < End; ++Idx)
			{
				Sz Location = Idx % _Capacity;
				_ItemVector.data()[Location].~F_Item();
			}

			BeginIndex_ = 0;
			_EndIndex = 0;
		}
	};
}