#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/BasicMath.hpp"
#include "Abytek/Assert.hpp"
#include "Abytek/SpinLock.hpp"
#include "Abytek/Allocator.hpp"
#include "Abytek/BoostContainerTemplates.hpp"


namespace Abytek::ContainerTemplates
{
	/**
	 * @brief A thread-safe circular buffer implementation
	 * 
	 * This container provides a fixed-size circular buffer with thread-safe
	 * push and pop operations. It's designed to efficiently handle producer-consumer
	 * scenarios.
	 * 
	 * @tparam __F_Item The type of items stored in the buffer
	 * @tparam __F_Allocator The allocator type used for memory management
	 * @tparam __F_Lock The lock type for synchronization (defaults to SPSC lock)
	 */
	template<typename __F_Item, class __F_Allocator, class __F_Lock = F_SPSCSpinLock>
	class TF_ConcurrentRingBuffer
	{
	public:
		using F_Item = __F_Item;
		using F_Allocator = __F_Allocator;
		using F_Lock = __F_Lock;

		using F_ItemVector = ContainerTemplates::TF_Vector<F_Item, TF_RebindAllocator<__F_Item, F_Allocator>>;
		using F_IsPoppableVector = ContainerTemplates::TF_Vector<B8, TF_RebindAllocator<B8, F_Allocator>>;

	protected:
		F_ItemVector _ItemVector;
		F_IsPoppableVector _IsPoppableVector;
		
		APDiff _BeginIndex = 0;
		APDiff _EndIndex = 0;

		F_Lock PopLock_;

	private:
		Sz _Capacity = 0;
		
	public:
		/**
		 * @brief Get the current number of items in the buffer
		 * @return The size of the buffer
		 */
		ABYTEK_FORCE_INLINE Sz GetSize() const { return _EndIndex.load(boost::memory_order_acquire) - _BeginIndex.load(boost::memory_order_acquire); }
		
		/**
		 * @brief Get the maximum capacity of the buffer
		 * @return The capacity of the buffer
		 */
		ABYTEK_FORCE_INLINE Sz GetCapacity() const { return _Capacity; }
		
		/**
		 * @brief Check if the buffer is empty
		 * @return True if the buffer is empty, false otherwise
		 */
		ABYTEK_FORCE_INLINE bool IsEmpty() const { return !GetSize(); }
		
		/**
		 * @brief Check if the buffer has been initialized
		 * @return True if the buffer has not been initialized, false otherwise
		 */
		ABYTEK_FORCE_INLINE bool IsNull() const { return !_Capacity; }
        
		/**
		 * @brief Get the current begin index position
		 * @return The begin index
		 */
		ABYTEK_FORCE_INLINE PDiff GetBeginIndex() const { return _BeginIndex.load(boost::memory_order_acquire); }
        
		/**
		 * @brief Get the current end index position
		 * @return The end index
		 */
		ABYTEK_FORCE_INLINE PDiff GetEndIndex() const { return _EndIndex.load(boost::memory_order_acquire); }
        
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
		 * @brief Get the vector indicating which items are poppable
		 * @return Constant reference to the poppable status vector
		 */
		ABYTEK_FORCE_INLINE const F_IsPoppableVector& GetIsPoppableVector() const noexcept { return _IsPoppableVector; }

	public:
		/**
		 * @brief Default constructor
		 */
		ABYTEK_FORCE_INLINE TF_ConcurrentRingBuffer() noexcept = default;
		
		/**
		 * @brief Construct with specified capacity and allocator
		 * @param Capacity The maximum number of items the buffer can hold
		 * @param Allocator The allocator to use
		 */
		TF_ConcurrentRingBuffer(Sz Capacity, const F_Allocator& Allocator) :
			_Capacity(Capacity),
			_ItemVector(Allocator),
			_IsPoppableVector(Capacity, false, Allocator)
		{
			_ItemVector.reserve(Capacity);
		}
		
		/**
		 * @brief Construct with specified capacity using default allocator
		 * @param Capacity The maximum number of items the buffer can hold
		 */
		TF_ConcurrentRingBuffer(Sz Capacity) :
			TF_ConcurrentRingBuffer(Capacity, {})
		{
		}
		
		/**
		 * @brief Copy constructor
		 * @param x The buffer to copy from
		 */
		TF_ConcurrentRingBuffer(const TF_ConcurrentRingBuffer& x) :
			_ItemVector(x._ItemVector),
			_Capacity(x._Capacity),
			_BeginIndex(x.GetBeginIndex()),
			_EndIndex(x.GetEndIndex())
		{
		}
		
		/**
		 * @brief Copy assignment operator
		 * @param x The buffer to copy from
		 * @return Reference to this object
		 */
		TF_ConcurrentRingBuffer& operator = (const TF_ConcurrentRingBuffer& x)
		{
			_ItemVector = x._ItemVector;
			_Capacity = x._Capacity;
			_BeginIndex = x.GetBeginIndex();
			_EndIndex = x.GetEndIndex();
            return *this;
		}
		
		/**
		 * @brief Move constructor
		 * @param x The buffer to move from
		 */
		TF_ConcurrentRingBuffer(TF_ConcurrentRingBuffer&& x) :
			_ItemVector(std::move(x._ItemVector)),
			_Capacity(x._Capacity),
			_BeginIndex(x.GetBeginIndex()),
			_EndIndex(x.GetEndIndex())
		{
			x.Reset();
		}
		
		/**
		 * @brief Move assignment operator
		 * @param x The buffer to move from
		 * @return Reference to this object
		 */
		TF_ConcurrentRingBuffer& operator = (TF_ConcurrentRingBuffer&& x)
		{
			_ItemVector = std::move(x._ItemVector);
			_Capacity = x._Capacity;
			_BeginIndex = x.GetBeginIndex();
			_EndIndex = x.GetEndIndex();
			x.Reset();
            return *this;
		}
		
		/**
		 * @brief Destructor
		 * Cleans up resources by calling Reset()
		 */
		~TF_ConcurrentRingBuffer()
		{
			Reset();
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
			ABYTEK_BASE_CORE_ASSERT(GetSize() < _Capacity) << "out of capacity";

			// obtain a location
			PDiff Location = _EndIndex.fetch_add(1, boost::memory_order_relaxed);
			Location %= _Capacity;

			// store item and mark this location as poppable
			new(_ItemVector.data() + Location) F_Item(
				std::forward<__F_PassedItem>(item)
			);

			boost::atomic_thread_fence(boost::memory_order_release);

			_IsPoppableVector[Location] = true;
			
			return Location;
		}

	public:
		/**
		 * @brief Push a copy of an item into the buffer
		 * @param item The item to push
		 * @return The index where the item was placed
		 */
		ABYTEK_FORCE_INLINE Sz Push(F_Item const& item)
		{
			return _Push(std::forward<F_Item const&>(item));
		}
		
		/**
		 * @brief Push an item into the buffer using move semantics
		 * @param item The item to push
		 * @return The index where the item was placed
		 */
		ABYTEK_FORCE_INLINE Sz Push(F_Item&& item)
		{
			return _Push(std::forward<F_Item>(item));
		}
		
		/**
		 * @brief Try to remove and get an item from the buffer
		 * @param item Reference where the removed item will be stored
		 * @return True if an item was successfully popped, false if the buffer was empty
		 */
		B8 TryPop(F_Item& item)
		{
			PopLock_.ConsumerLock();

			PDiff End = _EndIndex.load(boost::memory_order_relaxed);
			boost::atomic_thread_fence(boost::memory_order_acquire);
			PDiff Begin = _BeginIndex.load(boost::memory_order_relaxed);

			if (End > Begin) {

				PDiff location = Begin % _Capacity;

				// wait for the location to be poppable and then pop it
				while (!((AB8&)_IsPoppableVector[location]).load(boost::memory_order_acquire));

				boost::atomic_thread_fence(boost::memory_order_acquire);

				item = std::move(_ItemVector.data()[location]);

				boost::atomic_thread_fence(boost::memory_order_release);

				// mark this location as non-poppable
				// to be poppable again, it need to be re-pushed
				_IsPoppableVector[location] = false;

				//
				_BeginIndex.fetch_add(1, boost::memory_order_release);

				PopLock_.ConsumerUnlock();
				return true;
			}

			PopLock_.ConsumerUnlock();
			return false;
		}
		
		/**
		 * @brief Reset the buffer to an empty state
		 * Destroys all items and resets indices to 0
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

			_BeginIndex.store(0, boost::memory_order_release);
			_EndIndex.store(0, boost::memory_order_release);
		}
	};
}