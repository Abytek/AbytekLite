#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/PlatformProcess.hpp"
#include "Abytek/ProcessBase.hpp"
#include "Abytek/Windows/Process.hpp"


#ifdef ABYTEK_BASE_PLATFORM_ENABLE_IPC
namespace Abytek
{
    /**
     * @brief RAII wrapper for process resources
     * 
     * Provides automatic cleanup of process resources when the object goes out of scope.
     * Will wait for the process to complete and then destroy the process handle.
     */
    struct F_UniqueProcess
    {
        /** @brief The underlying process resource */
        F_ProcessLegacy Value;
        
        /** @brief Default constructor */
        ABYTEK_FORCE_INLINE F_UniqueProcess() noexcept = default;
        
        /**
         * @brief Destructor - destroy process
         */
        ABYTEK_FORCE_INLINE ~F_UniqueProcess() 
        {
            if(Value) 
            { 
                H_ProcessLegacy::Destroy(Value);
                Value = {};
            }
        }

        /**
         * @brief Constructor from process
         * @param InValue Process to wrap
         */
        ABYTEK_FORCE_INLINE F_UniqueProcess(const F_ProcessLegacy& InValue) noexcept :
            Value(InValue)
        {}

        /**
         * @brief Move constructor
         * @param X Source unique process object
         */
        ABYTEK_FORCE_INLINE F_UniqueProcess(F_UniqueProcess&& X) noexcept :
            Value(boost::move(X.Value))
        {
            X.Value = {};
        }
        
        /**
         * @brief Move assignment operator
         * @param X Source unique process object
         * @return Reference to this object
         */
        ABYTEK_FORCE_INLINE F_UniqueProcess& operator = (F_UniqueProcess&& X) noexcept
        {
            Value = X.Value;
            
            X.Value = {};

            return *this;
        }
        
        /**
         * @brief Boolean cast operator
         * @return True if the process is valid, false otherwise
         */
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept 
        {
            return static_cast<B8>(Value);
        }
        
        /**
         * @brief Dereference operator
         * @return Reference to the process
         */
        ABYTEK_FORCE_INLINE F_ProcessLegacy& operator * () noexcept 
        {
            return Value;
        }
        
        /**
         * @brief Const dereference operator
         * @return Const reference to the process
         */
        ABYTEK_FORCE_INLINE const F_ProcessLegacy& operator * () const noexcept 
        {
            return Value;
        }
        
        /**
         * @brief Arrow operator
         * @return Pointer to the process
         */
        ABYTEK_FORCE_INLINE F_ProcessLegacy* operator -> () noexcept 
        {
            return &Value;
        }
        
        /**
         * @brief Const arrow operator
         * @return Const pointer to the process
         */
        ABYTEK_FORCE_INLINE const F_ProcessLegacy* operator -> () const noexcept 
        {
            return &Value;
        }
    };
}
#endif