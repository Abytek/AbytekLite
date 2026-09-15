#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/PlatformPipe.hpp"
#include "Abytek/PipeBase.hpp"


#ifdef ABYTEK_BASE_PLATFORM_ENABLE_IPC_PIPE
namespace Abytek
{
    /**
     * @brief RAII wrapper for pipe resources
     * 
     * Provides automatic cleanup of pipe resources when the object goes out of scope
     */
    struct F_UniquePipe
    {
        /** @brief The underlying pipe resource */
        F_Pipe Value;
        
        /** @brief Default constructor */
        ABYTEK_FORCE_INLINE F_UniquePipe() noexcept = default;
        
        /**
         * @brief Destructor - automatically cleans up pipe resources
         */
        ABYTEK_FORCE_INLINE ~F_UniquePipe()
        {
            if(Value)
            {
                H_Pipe::Destroy(Value);
                Value = {};
            }
        }

        /**
         * @brief Constructor from pipe
         * @param InValue Pipe to wrap
         */
        ABYTEK_FORCE_INLINE F_UniquePipe(const F_Pipe& InValue) noexcept :
            Value(InValue)
        {}

        /**
         * @brief Move constructor
         * @param X Source unique pipe object
         */
        ABYTEK_FORCE_INLINE F_UniquePipe(F_UniquePipe&& X) noexcept :
            Value(boost::move(X.Value))
        {
            X.Value = {};
        }
        
        /**
         * @brief Move assignment operator
         * @param X Source unique pipe object
         * @return Reference to this object
         */
        ABYTEK_FORCE_INLINE F_UniquePipe& operator = (F_UniquePipe&& X) noexcept
        {
            Value = X.Value;
            
            X.Value = {};

            return *this;
        }
        
        /**
         * @brief Boolean cast operator
         * @return True if the pipe is valid, false otherwise
         */
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept 
        {
            return static_cast<B8>(Value);
        }
        
        /**
         * @brief Dereference operator
         * @return Reference to the pipe
         */
        ABYTEK_FORCE_INLINE F_Pipe& operator * () noexcept 
        {
            return Value;
        }
        
        /**
         * @brief Const dereference operator
         * @return Const reference to the pipe
         */
        ABYTEK_FORCE_INLINE const F_Pipe& operator * () const noexcept 
        {
            return Value;
        }
        
        /**
         * @brief Arrow operator
         * @return Pointer to the pipe
         */
        ABYTEK_FORCE_INLINE F_Pipe* operator -> () noexcept 
        {
            return &Value;
        }
        
        /**
         * @brief Const arrow operator
         * @return Const pointer to the pipe
         */
        ABYTEK_FORCE_INLINE const F_Pipe* operator -> () const noexcept 
        {
            return &Value;
        }
    };
}
#endif