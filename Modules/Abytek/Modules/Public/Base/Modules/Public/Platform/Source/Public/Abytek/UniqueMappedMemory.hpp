#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/PlatformMemory.hpp"
#include "Abytek/MemoryBase.hpp"
#include "Abytek/Windows/Memory.hpp"


#ifdef ABYTEK_BASE_PLATFORM_ENABLE_LOW_LEVEL_MEMORY
namespace Abytek
{
    /**
     * @brief RAII wrapper for mapped memory
     * 
     * Provides automatic cleanup of mapped memory resources when the object goes out of scope
     */
    struct F_UniqueMappedMemory
    {
        /** @brief The underlying mapped memory resource */
        F_MappedMemory Value;
        
        /** @brief Default constructor */
        ABYTEK_FORCE_INLINE F_UniqueMappedMemory() noexcept = default;
        
        /**
         * @brief Destructor - automatically cleans up mapped memory resources
         */
        ABYTEK_FORCE_INLINE ~F_UniqueMappedMemory()
        {
            if(Value)
            {
                H_Memory::DestroyOrCloseMapped(Value);
                Value = {};
            }
        }

        /**
         * @brief Constructor from mapped memory
         * @param InValue Mapped memory to wrap
         */
        ABYTEK_FORCE_INLINE F_UniqueMappedMemory(const F_MappedMemory& InValue) noexcept :
            Value(InValue)
        {}

        /**
         * @brief Move constructor
         * @param X Source unique mapped memory object
         */
        ABYTEK_FORCE_INLINE F_UniqueMappedMemory(F_UniqueMappedMemory&& X) noexcept :
            Value(boost::move(X.Value))
        {
            X.Value = {};
        }
        
        /**
         * @brief Move assignment operator
         * @param X Source unique mapped memory object
         * @return Reference to this object
         */
        ABYTEK_FORCE_INLINE F_UniqueMappedMemory& operator = (F_UniqueMappedMemory&& X) noexcept
        {
            Value = X.Value;
            
            X.Value = {};

            return *this;
        }
        
        /**
         * @brief Boolean cast operator
         * @return True if the mapped memory is valid, false otherwise
         */
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept 
        {
            return static_cast<B8>(Value);
        }
        
        /**
         * @brief Dereference operator
         * @return Reference to the mapped memory
         */
        ABYTEK_FORCE_INLINE F_MappedMemory& operator * () noexcept 
        {
            return Value;
        }
        
        /**
         * @brief Const dereference operator
         * @return Const reference to the mapped memory
         */
        ABYTEK_FORCE_INLINE const F_MappedMemory& operator * () const noexcept 
        {
            return Value;
        }
        
        /**
         * @brief Arrow operator
         * @return Pointer to the mapped memory
         */
        ABYTEK_FORCE_INLINE F_MappedMemory* operator -> () noexcept 
        {
            return &Value;
        }
        
        /**
         * @brief Const arrow operator
         * @return Const pointer to the mapped memory
         */
        ABYTEK_FORCE_INLINE const F_MappedMemory* operator -> () const noexcept 
        {
            return &Value;
        }
    };
}
#endif