#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/Assert.hpp"
#include "Abytek/Allocator.hpp"
#include "Abytek/BoostContainerTemplates.hpp"


namespace Abytek::ContainerTemplates
{
    /**
     * @brief Forward declaration of the Event class
     */
    template<typename __F_RawFunction, typename __F_Allocator>
    class TF_Event;

    /**
     * @brief Event handler class for callback management
     * 
     * This container manages a list of callback functions that can be called
     * when an event is broadcast. It provides mechanisms to add, remove,
     * and invoke listeners.
     * 
     * @tparam __F_Allocator The allocator type used for memory management
     * @tparam __F_Args The argument types for the event callback functions
     */
    template<typename __F_Allocator, typename __F_Return, typename... __F_Args>
    class TF_Event<__F_Return(__F_Args...), __F_Allocator>
    {
    public:
        using F_RawFunction = __F_Return(__F_Args...);
        using F_Allocator = __F_Allocator;

    public:
        using F_Function = TF_Function<F_RawFunction>;
        using F_FunctionList = TF_List<F_Function, TF_RebindAllocator<F_Function, F_Allocator>>;

    public:
        using F_ListenerHandle = typename F_FunctionList::iterator;

    private:
        F_FunctionList _Functions;

    public:
        /**
         * @brief Get the list of registered functions
         * @return Constant reference to the function list
         */
        ABYTEK_FORCE_INLINE const auto& GetFunctions() const noexcept
        {
            return _Functions;
        }

    public:
        /**
         * @brief Default constructor
         */
        ABYTEK_FORCE_INLINE TF_Event() noexcept = default;

        /**
         * @brief Copy constructor
         * @param X The event to copy from
         */
        TF_Event(const TF_Event& X) noexcept = default;
        
        /**
         * @brief Copy assignment operator
         * @param X The event to copy from
         * @return Reference to this object
         */
        TF_Event& operator = (const TF_Event& X) noexcept = default;

        /**
         * @brief Move constructor
         * @param X The event to move from
         */
        TF_Event(TF_Event&& X) noexcept = default;
        
        /**
         * @brief Move assignment operator
         * @param X The event to move from
         * @return Reference to this object
         */
        TF_Event& operator = (TF_Event&& X) noexcept = default;

    public:
        /**
         * @brief Add a listener to the back of the event list
         * @tparam __F_PassedListener Type of the listener function
         * @param Listener The listener function to add
         * @return Handle to the added listener for later removal
         */
        template<typename __F_PassedListener>
        F_ListenerHandle AddBackListener(__F_PassedListener&& Listener)
        {
            _Functions.push_back(ABYTEK_FORWARD(Listener));
            return --(_Functions.end());
        }
        
        /**
         * @brief Add a listener to the front of the event list
         * @tparam __F_PassedListener Type of the listener function
         * @param Listener The listener function to add
         * @return Handle to the added listener for later removal
         */
        template<typename __F_PassedListener>
        F_ListenerHandle AddFrontListener(__F_PassedListener&& Listener)
        {
            _Functions.push_front(ABYTEK_FORWARD(Listener));
            return _Functions.begin();
        }
        
        /**
         * @brief Add a listener to the event (alias for AddBackListener)
         * @tparam __F_PassedListener Type of the listener function
         * @param Listener The listener function to add
         * @return Handle to the added listener for later removal
         */
        template<typename __F_PassedListener>
        F_ListenerHandle AddListener(__F_PassedListener&& Listener)
        {
            return AddBackListener(ABYTEK_FORWARD(Listener));
        }
        
        /**
         * @brief Remove a previously added listener
         * @param ListenerHandle The handle of the listener to remove
         */
        void RemoveListener(const F_ListenerHandle& ListenerHandle)
        {
            _Functions.erase(ListenerHandle);
        }

    public:
        /**
         * @brief Remove all listeners from the event
         */
        void RemoveListeners()
        {
            _Functions.clear();
        }

    public:
        /**
         * @brief Trigger the event, calling all registered listeners
         * @param Args The arguments to pass to each listener
         */
        void Broadcast(__F_Args... Args)
        {
            for(auto It = _Functions.begin(); It != _Functions.end();)
            {
                auto NextIt = It;
                ++NextIt;
                
                auto& Function = *It;
                Function(ABYTEK_FORWARD(Args)...);

                It = NextIt;
            }
        }
        
        /**
         * @brief Trigger the event with custom caller logic
         * 
         * This version allows for custom processing of each listener,
         * including the ability to stop broadcasting early.
         * 
         * @tparam __F_PassedCaller Type of the caller function
         * @param Caller A callable that processes each listener
         *        Should return true to continue broadcasting, false to stop
         */
        template<typename __F_PassedCaller>
        void BroadcastCustom(__F_PassedCaller&& Caller)
        {
            for(auto It = _Functions.begin(); It != _Functions.end();)
            {
                auto NextIt = It;
                ++NextIt;
                
                auto& Function = *It;
                if(!Caller(Function))
                {
                    break;
                }

                It = NextIt;
            }
        }

    public:
        /**
         * @brief Trigger the event, calling all registered listeners
         * @param Args The arguments to pass to each listener
         */
        void BroadcastReversed(__F_Args... Args)
        {
            for(auto It = _Functions.rbegin(); It != _Functions.rend();)
            {
                auto NextIt = It;
                ++NextIt;
                
                auto& Function = *It;
                Function(ABYTEK_FORWARD(Args)...);

                It = NextIt;
            }
        }
        
        /**
         * @brief Trigger the event with custom caller logic
         * 
         * This version allows for custom processing of each listener,
         * including the ability to stop broadcasting early.
         * 
         * @tparam __F_PassedCaller Type of the caller function
         * @param Caller A callable that processes each listener
         *        Should return true to continue broadcasting, false to stop
         */
        template<typename __F_PassedCaller>
        void BroadcastCustomReversed(__F_PassedCaller&& Caller)
        {
            for(auto It = _Functions.rbegin(); It != _Functions.rend();)
            {
                auto NextIt = It;
                ++NextIt;
                
                auto& Function = *It;
                if(!Caller(Function))
                {
                    break;
                }

                It = NextIt;
            }
        }
    };
}