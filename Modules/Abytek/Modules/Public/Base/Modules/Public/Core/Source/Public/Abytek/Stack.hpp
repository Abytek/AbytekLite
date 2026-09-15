#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/BoostContainerTemplates.hpp"


namespace Abytek::ContainerTemplates
{
    /**
     * @brief Stack container that provides LIFO (Last-In-First-Out) functionality
     * 
     * A templated stack implementation that can be configured with different
     * underlying container types. Defaults to using a vector container.
     * 
     * @tparam __F_Item The type of items stored in the stack
     * @tparam __F_Container The underlying container type, defaults to TF_Vector<__F_Item>
     */
    template<typename __F_Item, class __F_Container = TF_Vector<__F_Item>>
    class TF_Stack
    {
    public:
        using F_Item = __F_Item;
        using F_Container = __F_Container;
        
    private:
        F_Container _Container;

    public:
        /**
         * @brief Gets the underlying container
         * @return Reference to the container
         */
        ABYTEK_FORCE_INLINE F_Container& GetContainer() noexcept
        {
            return _Container;
        }
        
        /**
         * @brief Gets the underlying container (const version)
         * @return Const reference to the container
         */
        ABYTEK_FORCE_INLINE const F_Container& GetContainer() const noexcept
        {
            return _Container;
        }

    public:
        /** @brief Default constructor */
        ABYTEK_FORCE_INLINE TF_Stack() = default;
        
        /**
         * @brief Constructs a stack from an existing container
         * @param Container The container to use
         */
        ABYTEK_FORCE_INLINE TF_Stack(const F_Container& Container) :
            _Container(Container)
        {}
        
        /**
         * @brief Constructs a stack by moving from an existing container
         * @param Container The container to move from
         */
        ABYTEK_FORCE_INLINE TF_Stack(F_Container&& Container) :
            _Container(boost::move(Container))
        {}
        
        /** @brief Copy constructor */
        ABYTEK_FORCE_INLINE TF_Stack(const TF_Stack& X) = default;
        
        /** @brief Copy assignment operator */
        ABYTEK_FORCE_INLINE TF_Stack& operator = (const TF_Stack& X) = default;
        
        /** @brief Move constructor */
        ABYTEK_FORCE_INLINE TF_Stack(TF_Stack&& X) = default;
        
        /** @brief Move assignment operator */
        ABYTEK_FORCE_INLINE TF_Stack& operator = (TF_Stack&& X) = default;

    public:
        /**
         * @brief Gets the number of elements in the stack
         * @return The stack size
         */
        ABYTEK_FORCE_INLINE Sz GetSize() const noexcept
        {
            return _Container.size();
        }
        
        /**
         * @brief Gets reference to the first element (front)
         * @return Reference to the first element
         */
        ABYTEK_FORCE_INLINE F_Item& GetFront() noexcept
        {
            return _Container.front();
        }
        
        /**
         * @brief Gets const reference to the first element (front)
         * @return Const reference to the first element
         */
        ABYTEK_FORCE_INLINE const F_Item& GetFront() const noexcept
        {
            return _Container.front();
        }
        
        /**
         * @brief Gets reference to the last element (back)
         * @return Reference to the last element
         */
        ABYTEK_FORCE_INLINE F_Item& GetBack() noexcept
        {
            return _Container.back();
        }
        
        /**
         * @brief Gets const reference to the last element (back)
         * @return Const reference to the last element
         */
        ABYTEK_FORCE_INLINE const F_Item& GetBack() const noexcept
        {
            return _Container.back();
        }
        
        /**
         * @brief Gets reference to the bottom element (alias for GetFront)
         * @return Reference to the bottom element
         */
        ABYTEK_FORCE_INLINE F_Item& GetBottom() noexcept
        {
            return GetFront();
        }
        
        /**
         * @brief Gets const reference to the bottom element (alias for GetFront)
         * @return Const reference to the bottom element
         */
        ABYTEK_FORCE_INLINE const F_Item& GetBottom() const noexcept
        {
            return GetFront();
        }
        
        /**
         * @brief Gets reference to the top element (alias for GetBack)
         * @return Reference to the top element
         */
        ABYTEK_FORCE_INLINE F_Item& GetTop() noexcept
        {
            return GetBack();
        }
        
        /**
         * @brief Gets const reference to the top element (alias for GetBack)
         * @return Const reference to the top element
         */
        ABYTEK_FORCE_INLINE const F_Item& GetTop() const noexcept
        {
            return GetBack();
        }
        
        /**
         * @brief Adds an element to the top of the stack
         * @param Item The item to add
         */
        ABYTEK_FORCE_INLINE void Push(const F_Item& Item)
        {
            _Container.push_back(Item);
        }
        
        /**
         * @brief Adds an element to the top of the stack by moving
         * @param Item The item to move and add
         */
        ABYTEK_FORCE_INLINE void Push(F_Item&& Item)
        {
            _Container.push_back(boost::move(Item));
        }
        
        /**
         * @brief Removes the top element from the stack
         */
        ABYTEK_FORCE_INLINE void Pop()
        {
            _Container.pop_back();
        }
        
        /**
         * @brief Tries to remove the top element and store it
         * @param OutValue Reference to store the popped value
         * @return True if an element was removed, false if the stack was empty
         */
        ABYTEK_FORCE_INLINE B8 TryPop(F_Item& OutValue)
        {
            if(GetSize() == 0)
            {
                return false;
            }
            OutValue = boost::move(GetBack());
            Pop();
            return true;
        }
    };
}