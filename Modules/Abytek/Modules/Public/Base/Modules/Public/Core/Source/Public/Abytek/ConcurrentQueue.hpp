#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/BoostContainerTemplates.hpp"
#include "Abytek/SpinLock.hpp"
#include "Abytek/ScopeLock.hpp"


namespace Abytek::ContainerTemplates
{
    /**
     * @brief A thread-safe queue data structure
     * 
     * This container provides synchronized access to a queue data structure,
     * allowing operations from multiple threads.
     * 
     * @tparam __F_Item The type of items stored in the queue
     * @tparam __F_Container The underlying container type for storage (defaults to deque)
     */
    template<typename __F_Item, class __F_Container = TF_Deque<__F_Item>>
    class TF_ConcurrentQueue
    {
    public:
        using F_Item = __F_Item;
        using F_Container = __F_Container;
        
    private:
        F_Container _Container;
        F_SpinLock _Lock;

    public:
        /**
         * @brief Get the underlying container
         * @return Reference to the container
         */
        ABYTEK_FORCE_INLINE F_Container& GetContainer() noexcept
        {
            return _Container;
        }
        
        /**
         * @brief Get the underlying container (const version)
         * @return Constant reference to the container
         */
        ABYTEK_FORCE_INLINE const F_Container& GetContainer() const noexcept
        {
            return _Container;
        }
        
        /**
         * @brief Get the lock used for synchronization
         * @return Reference to the spin lock
         */
        ABYTEK_FORCE_INLINE F_SpinLock& GetLock() noexcept
        {
            return _Lock;
        }

    public:
        /**
         * @brief Default constructor
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentQueue() = default;
        
        /**
         * @brief Construct with an existing container
         * @param Container The container to use
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentQueue(const F_Container& Container) :
            _Container(Container)
        {}
        
        /**
         * @brief Construct with a moved container
         * @param Container The container to move from
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentQueue(F_Container&& Container) :
            _Container(boost::move(Container))
        {}
        
        /**
         * @brief Copy constructor
         * @param X The queue to copy from
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentQueue(const TF_ConcurrentQueue& X) :
            _Container(X._Container)
        {}
        
        /**
         * @brief Copy assignment operator
         * @param X The queue to copy from
         * @return Reference to this object
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentQueue& operator = (const TF_ConcurrentQueue& X)
        {
            _Container = X._Container;
            return *this;
        }
        
        /**
         * @brief Move constructor
         * @param X The queue to move from
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentQueue(TF_ConcurrentQueue&& X) = default;
        
        /**
         * @brief Move assignment operator
         * @param X The queue to move from
         * @return Reference to this object
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentQueue& operator = (TF_ConcurrentQueue&& X) = default;

    public:
        /**
         * @brief Get the number of items in the queue
         * @return The size of the queue
         */
        ABYTEK_FORCE_INLINE Sz GetSize() const noexcept
        {
            return _Container.size();
        }
        
        /**
         * @brief Get the item at the front of the queue
         * @return Reference to the front item
         */
        ABYTEK_FORCE_INLINE F_Item& GetFront() noexcept
        {
            return _Container.front();
        }
        
        /**
         * @brief Get the item at the front of the queue (const version)
         * @return Constant reference to the front item
         */
        ABYTEK_FORCE_INLINE const F_Item& GetFront() const noexcept
        {
            return _Container.front();
        }
        
        /**
         * @brief Get the item at the back of the queue
         * @return Reference to the back item
         */
        ABYTEK_FORCE_INLINE F_Item& GetBack() noexcept
        {
            return _Container.back();
        }
        
        /**
         * @brief Get the item at the back of the queue (const version)
         * @return Constant reference to the back item
         */
        ABYTEK_FORCE_INLINE const F_Item& GetBack() const noexcept
        {
            return _Container.back();
        }
        
        /**
         * @brief Get the first item in the queue (alias for GetFront)
         * @return Reference to the first item
         */
        ABYTEK_FORCE_INLINE F_Item& GetFirst() noexcept
        {
            return GetFront();
        }
        
        /**
         * @brief Get the first item in the queue (const version, alias for GetFront)
         * @return Constant reference to the first item
         */
        ABYTEK_FORCE_INLINE const F_Item& GetFirst() const noexcept
        {
            return GetFront();
        }
        
        /**
         * @brief Get the last item in the queue (alias for GetBack)
         * @return Reference to the last item
         */
        ABYTEK_FORCE_INLINE F_Item& GetLast() noexcept
        {
            return GetBack();
        }
        
        /**
         * @brief Get the last item in the queue (const version, alias for GetBack)
         * @return Constant reference to the last item
         */
        ABYTEK_FORCE_INLINE const F_Item& GetLast() const noexcept
        {
            return GetBack();
        }
        
        /**
         * @brief Add an item to the back of the queue
         * @param Item The item to add (by copy)
         */
        ABYTEK_FORCE_INLINE void PushFront(const F_Item& Item)
        {
            TF_ScopeLock<F_SpinLock> _(_Lock);
            _Container.push_front(Item);
        }
        
        /**
         * @brief Add an item to the back of the queue using move semantics
         * @param Item The item to add (by move)
         */
        ABYTEK_FORCE_INLINE void PushFront(F_Item&& Item)
        {
            TF_ScopeLock<F_SpinLock> _(_Lock);
            _Container.push_front(boost::move(Item));
        }
        
        /**
         * @brief Add an item to the back of the queue
         * @param Item The item to add (by copy)
         */
        ABYTEK_FORCE_INLINE void Push(const F_Item& Item)
        {
            TF_ScopeLock<F_SpinLock> _(_Lock);
            _Container.push_back(Item);
        }
        
        /**
         * @brief Add an item to the back of the queue using move semantics
         * @param Item The item to add (by move)
         */
        ABYTEK_FORCE_INLINE void Push(F_Item&& Item)
        {
            TF_ScopeLock<F_SpinLock> _(_Lock);
            _Container.push_back(boost::move(Item));
        }
        
        /**
         * @brief Remove the item from the front of the queue
         */
        ABYTEK_FORCE_INLINE void Pop()
        {
            TF_ScopeLock<F_SpinLock> _(_Lock);
            _Container.pop_front();
        }
        
        /**
         * @brief Try to remove and get the front item from the queue
         * @param OutValue Reference where the removed item will be stored
         * @return True if an item was successfully popped, false if the queue was empty
         */
        ABYTEK_FORCE_INLINE B8 TryPop(F_Item& OutValue)
        {
            TF_ScopeLock<F_SpinLock> _(_Lock);
            if(GetSize() == 0)
            {
                return false;
            }
            OutValue = boost::move(GetFront());
            _Container.pop_front();
            return true;
        }
        
        ContainerTemplates::TF_Vector<F_Item, typename F_Container::allocator_type> PopAll()
        {
            TF_ScopeLock<F_SpinLock> _(_Lock);
            ContainerTemplates::TF_Vector<F_Item, typename F_Container::allocator_type> Result;
            while (GetSize())
            {
                Result.push_back(boost::move(GetFront()));
                _Container.pop_front();
            }
            return ABYTEK_MOVE(Result);
        }
    };
}