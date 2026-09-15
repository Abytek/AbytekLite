#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/BoostContainerTemplates.hpp"
#include "Abytek/SpinLock.hpp"
#include "Abytek/ScopeLock.hpp"


namespace Abytek::ContainerTemplates
{
    /**
     * @brief A thread-safe stack data structure
     * 
     * This container provides synchronized access to a stack data structure,
     * allowing operations from multiple threads.
     * 
     * @tparam __F_Item The type of items stored in the stack
     * @tparam __F_Container The underlying container type for storage (defaults to vector)
     */
    template<typename __F_Item, class __F_Container = TF_Vector<__F_Item>>
    class TF_ConcurrentStack
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
        ABYTEK_FORCE_INLINE TF_ConcurrentStack() = default;
        
        /**
         * @brief Construct with an existing container
         * @param Container The container to use
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentStack(const F_Container& Container) :
            _Container(Container)
        {}
        
        /**
         * @brief Construct with a moved container
         * @param Container The container to move from
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentStack(F_Container&& Container) :
            _Container(boost::move(Container))
        {}
        
        /**
         * @brief Copy constructor
         * @param X The stack to copy from
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentStack(const TF_ConcurrentStack& X) :
            _Container(X._Container)
        {}
        
        /**
         * @brief Copy assignment operator
         * @param X The stack to copy from
         * @return Reference to this object
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentStack& operator = (const TF_ConcurrentStack& X)
        {
            _Container = X._Container;
            return *this;
        }
        
        /**
         * @brief Move constructor
         * @param X The stack to move from
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentStack(TF_ConcurrentStack&& X) = default;
        
        /**
         * @brief Move assignment operator
         * @param X The stack to move from
         * @return Reference to this object
         */
        ABYTEK_FORCE_INLINE TF_ConcurrentStack& operator = (TF_ConcurrentStack&& X) = default;

    public:
        /**
         * @brief Get the number of items in the stack
         * @return The size of the stack
         */
        ABYTEK_FORCE_INLINE Sz GetSize() const noexcept
        {
            return _Container.size();
        }
        
        /**
         * @brief Get the item at the front of the container
         * @return Reference to the front item
         */
        ABYTEK_FORCE_INLINE F_Item& GetFront() noexcept
        {
            return _Container.front();
        }
        
        /**
         * @brief Get the item at the front of the container (const version)
         * @return Constant reference to the front item
         */
        ABYTEK_FORCE_INLINE const F_Item& GetFront() const noexcept
        {
            return _Container.front();
        }
        
        /**
         * @brief Get the item at the back of the container
         * @return Reference to the back item
         */
        ABYTEK_FORCE_INLINE F_Item& GetBack() noexcept
        {
            return _Container.back();
        }
        
        /**
         * @brief Get the item at the back of the container (const version)
         * @return Constant reference to the back item
         */
        ABYTEK_FORCE_INLINE const F_Item& GetBack() const noexcept
        {
            return _Container.back();
        }
        
        /**
         * @brief Get the item at the bottom of the stack (alias for GetFront)
         * @return Reference to the bottom item
         */
        ABYTEK_FORCE_INLINE F_Item& GetBottom() noexcept
        {
            return GetFront();
        }
        
        /**
         * @brief Get the item at the bottom of the stack (const version, alias for GetFront)
         * @return Constant reference to the bottom item
         */
        ABYTEK_FORCE_INLINE const F_Item& GetBottom() const noexcept
        {
            return GetFront();
        }
        
        /**
         * @brief Get the item at the top of the stack (alias for GetBack)
         * @return Reference to the top item
         */
        ABYTEK_FORCE_INLINE F_Item& GetTop() noexcept
        {
            return GetBack();
        }
        
        /**
         * @brief Get the item at the top of the stack (const version, alias for GetBack)
         * @return Constant reference to the top item
         */
        ABYTEK_FORCE_INLINE const F_Item& GetTop() const noexcept
        {
            return GetBack();
        }
        
        /**
         * @brief Add an item to the top of the stack
         * @param Item The item to add (by copy)
         */
        ABYTEK_FORCE_INLINE void Push(const F_Item& Item)
        {
            TF_ScopeLock<F_SpinLock> _(_Lock);
            _Container.push_back(Item);
        }
        
        /**
         * @brief Add an item to the top of the stack using move semantics
         * @param Item The item to add (by move)
         */
        ABYTEK_FORCE_INLINE void Push(F_Item&& Item)
        {
            TF_ScopeLock<F_SpinLock> _(_Lock);
            _Container.push_back(boost::move(Item));
        }
        
        /**
         * @brief Remove the item from the top of the stack
         */
        ABYTEK_FORCE_INLINE void Pop()
        {
            TF_ScopeLock<F_SpinLock> _(_Lock);
            _Container.pop_back();
        }
        
        /**
         * @brief Try to remove and get the top item from the stack
         * @param OutValue Reference where the removed item will be stored
         * @return True if an item was successfully popped, false if the stack was empty
         */
        ABYTEK_FORCE_INLINE B8 TryPop(F_Item& OutValue)
        {
            TF_ScopeLock<F_SpinLock> _(_Lock);
            if(GetSize() == 0)
            {
                return false;
            }
            OutValue = boost::move(GetBack());
            _Container.pop_back();
            return true;
        }
    };
}