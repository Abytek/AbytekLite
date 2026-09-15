/**
 * @file ObjectSmartPointerTypedefs.inl
 * @brief Type definitions for smart pointers using the default object management system.
 * 
 * This file provides convenient type aliases and helper functions for the smart pointer templates,
 * configured with the default object management implementation.
 */

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/ObjectSmartPointerTemplates.hpp"
#include "Abytek/StandardObjectManagement.hpp"

#ifndef ABYTEK_OBJECT_SMART_POINTER_TYPEDEFS_NAMESPACE
#define ABYTEK_OBJECT_SMART_POINTER_TYPEDEFS_NAMESPACE Abytek
#endif


namespace ABYTEK_OBJECT_SMART_POINTER_TYPEDEFS_NAMESPACE
{
#ifndef ABYTEK_OBJECT_CUSTOM_OBJECT_POINTER_CONFIG_EXTENSION
    /**
     * @brief Default extension point for object pointer configuration customization.
     */
    using F_DefaultObjectPointerConfigExtension = F_ObjectPointerConfigExtension;
#endif
    
    /**
     * @brief Memory header for uniquely owned objects using default object management.
     * @tparam __F_Object The object type
     */
    template<class __F_Object>
    using TF_ManagedObjectHeader = Abytek::ObjectSmartPointerTemplates::TF_ManagedObjectHeader<__F_Object, F_DefaultObjectManagement>;
    
    /**
     * @brief Memory header for shared objects using default object management.
     * @tparam __F_Object The object type
     */
    template<class __F_Object>
    using TF_ManagedObjectHeader = Abytek::ObjectSmartPointerTemplates::TF_ManagedObjectHeader<__F_Object, F_DefaultObjectManagement>;

    /**
     * @brief Memory layout for uniquely owned objects using default object management.
     * @tparam __F_Object The object type
     */
    template<class __F_Object>
    using TF_ManagedObjectMemory = Abytek::ObjectSmartPointerTemplates::TF_ManagedObjectMemory<__F_Object, F_DefaultObjectManagement>;
    
    /**
     * @brief Default configuration for object pointers using standard object management.
     */
    using F_DefaultObjectConfig = typename Abytek::TF_ObjectPointerConfig<F_DefaultObjectManagement>::template TF_RebindExtension<F_DefaultObjectPointerConfigExtension>;
    
    /**
     * @brief Weak pointer template with default configuration.
     * @tparam __F_PassedObject The object type this pointer references
     * @tparam __F_Config Configuration for pointer behavior and validation (default: F_DefaultObjectConfig)
     */
    template<typename __F_PassedObject, class __F_Config = F_DefaultObjectConfig>
    using TW = Abytek::ObjectSmartPointerTemplates::TW<__F_PassedObject, __F_Config>;
    
    /**
     * @brief Weak pointer template with validation enabled.
     * @tparam __F_PassedObject The object type this pointer references
     * @tparam __F_Config Configuration for pointer behavior and validation (default: F_DefaultObjectConfig)
     */
    template<typename __F_PassedObject, class __F_Config = F_DefaultObjectConfig>
    using TW_Valid = Abytek::ObjectSmartPointerTemplates::TW_Valid<__F_PassedObject, __F_Config>;
    
    /**
     * @brief Creates a weak pointer to an object with validation enabled.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A weak pointer to the object
     */
    template<
        typename __F_PassedObject, 
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetWThis(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetWThis<__F_PassedObject, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Creates a mutable weak pointer to an object with validation enabled.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A mutable weak pointer to the object
     */
    template<
        typename __F_PassedObject, 
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetWThisMutable(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetWThisMutable<__F_PassedObject, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Creates a weak pointer to an object without validation.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A weak pointer to the object without validation
     */
    template<
        typename __F_PassedObject, 
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetWThis_NoValidation(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetWThis_NoValidation<__F_PassedObject, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Creates a mutable weak pointer to an object without validation.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A mutable weak pointer to the object without validation
     */
    template<
        typename __F_PassedObject, 
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetWThisMutable_NoValidation(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetWThisMutable_NoValidation<__F_PassedObject, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Unique pointer template with default configuration.
     * @tparam __F_PassedObject The object type this pointer owns
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    using TU = Abytek::ObjectSmartPointerTemplates::TU<__F_PassedObject, __F_Allocator, __F_Config>;
    
    /**
     * @brief Unique pointer template with validation enabled.
     * @tparam __F_PassedObject The object type this pointer owns
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    using TU_Valid = Abytek::ObjectSmartPointerTemplates::TU_Valid<__F_PassedObject, __F_Allocator, __F_Config>;
    
    /**
     * @brief Shared pointer template with default configuration.
     * @tparam __F_PassedObject The object type this pointer references
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    using TS = Abytek::ObjectSmartPointerTemplates::TS<__F_PassedObject, __F_Allocator, __F_Config>;
    
    /**
     * @brief Shared pointer template with validation enabled.
     * @tparam __F_PassedObject The object type this pointer references
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    using TS_Valid = Abytek::ObjectSmartPointerTemplates::TS_Valid<__F_PassedObject, __F_Allocator, __F_Config>;
    
    /**
     * @brief Creates a shared pointer to an object with validation enabled.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A shared pointer to the object
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetSThis(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetSThis<__F_PassedObject, __F_Allocator, __F_Config>(ObjectRawP);
    }
    
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetUThis(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetUThis<__F_PassedObject, __F_Allocator, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Creates a mutable shared pointer to an object with validation enabled.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A mutable shared pointer to the object
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetSThisMutable(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetSThisMutable<__F_PassedObject, __F_Allocator, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Creates a shared pointer to an object without validation.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A shared pointer to the object without validation
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetSThis_NoValidation(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetSThis_NoValidation<__F_PassedObject, __F_Allocator, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Creates a mutable shared pointer to an object without validation.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A mutable shared pointer to the object without validation
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetSThisMutable_NoValidation(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetSThisMutable_NoValidation<__F_PassedObject, __F_Allocator, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Weak pointer template with default configuration.
     * @tparam __F_PassedObject The object type this pointer references
     * @tparam __F_Config Configuration for pointer behavior and validation (default: F_DefaultObjectConfig)
     */
    template<typename __F_PassedObject, class __F_Config = F_DefaultObjectConfig>
    using TW_Unmanaged = Abytek::ObjectSmartPointerTemplates::TW_Unmanaged<__F_PassedObject, __F_Config>;
    
    /**
     * @brief Weak pointer template with validation enabled.
     * @tparam __F_PassedObject The object type this pointer references
     * @tparam __F_Config Configuration for pointer behavior and validation (default: F_DefaultObjectConfig)
     */
    template<typename __F_PassedObject, class __F_Config = F_DefaultObjectConfig>
    using TW_ValidUnmanaged = Abytek::ObjectSmartPointerTemplates::TW_ValidUnmanaged<__F_PassedObject, __F_Config>;
    
    /**
     * @brief Creates a weak pointer to an object with validation enabled.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A weak pointer to the object
     */
    template<
        typename __F_PassedObject, 
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetWThisUnmanaged(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetWThisUnmanaged<__F_PassedObject, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Creates a mutable weak pointer to an object with validation enabled.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A mutable weak pointer to the object
     */
    template<
        typename __F_PassedObject, 
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetWThisMutableUnmanaged(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetWThisMutableUnmanaged<__F_PassedObject, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Creates a weak pointer to an object without validation.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A weak pointer to the object without validation
     */
    template<
        typename __F_PassedObject, 
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetWThisUnmanaged_NoValidation(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetWThisUnmanaged_NoValidation<__F_PassedObject, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Creates a mutable weak pointer to an object without validation.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A mutable weak pointer to the object without validation
     */
    template<
        typename __F_PassedObject, 
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetWThisMutableUnmanaged_NoValidation(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetWThisMutableUnmanaged_NoValidation<__F_PassedObject, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Unique pointer template with default configuration.
     * @tparam __F_PassedObject The object type this pointer owns
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    using TU_Unmanaged = Abytek::ObjectSmartPointerTemplates::TU_Unmanaged<__F_PassedObject, __F_Allocator, __F_Config>;
    
    /**
     * @brief Unique pointer template with validation enabled.
     * @tparam __F_PassedObject The object type this pointer owns
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    using TU_ValidUnmanaged = Abytek::ObjectSmartPointerTemplates::TU_ValidUnmanaged<__F_PassedObject, __F_Allocator, __F_Config>;
    
    /**
     * @brief Shared pointer template with default configuration.
     * @tparam __F_PassedObject The object type this pointer references
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    using TS_Unmanaged = Abytek::ObjectSmartPointerTemplates::TS_Unmanaged<__F_PassedObject, __F_Allocator, __F_Config>;
    
    /**
     * @brief Shared pointer template with validation enabled.
     * @tparam __F_PassedObject The object type this pointer references
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    using TS_ValidUnmanaged = Abytek::ObjectSmartPointerTemplates::TS_ValidUnmanaged<__F_PassedObject, __F_Allocator, __F_Config>;
    
    /**
     * @brief Creates a shared pointer to an object with validation enabled.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A shared pointer to the object
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetSThisUnmanaged(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetSThisUnmanaged<__F_PassedObject, __F_Allocator, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Creates a mutable shared pointer to an object with validation enabled.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A mutable shared pointer to the object
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetSThisMutableUnmanaged(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetSThisMutableUnmanaged<__F_PassedObject, __F_Allocator, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Creates a shared pointer to an object without validation.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A shared pointer to the object without validation
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetSThisUnmanaged_NoValidation(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetSThisUnmanaged_NoValidation<__F_PassedObject, __F_Allocator, __F_Config>(ObjectRawP);
    }
    
    /**
     * @brief Creates a mutable shared pointer to an object without validation.
     * @tparam __F_PassedObject The object type
     * @tparam __F_Allocator The allocator type (default: TF_DefaultAllocator<A_Object>)
     * @tparam __F_Config Configuration for pointer behavior (default: F_DefaultObjectConfig)
     * @param ObjectRawP Raw pointer to the object
     * @return A mutable shared pointer to the object without validation
     */
    template<
        typename __F_PassedObject, 
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    ABYTEK_FORCE_INLINE auto GetSThisMutableUnmanaged_NoValidation(__F_PassedObject* ObjectRawP) noexcept 
    {
        return Abytek::ObjectSmartPointerTemplates::GetSThisMutableUnmanaged_NoValidation<__F_PassedObject, __F_Allocator, __F_Config>(ObjectRawP);
    }

    template<
        typename __F_PassedObject,
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    auto ShareObject(
        const TW<__F_PassedObject, __F_Config>& WeakP
    ) noexcept
    {
        return ObjectSmartPointerTemplates::ShareObject<__F_PassedObject, __F_Allocator, __F_Config>(WeakP);
    }
    template<
        typename __F_PassedObject,
        class __F_Allocator = TF_DefaultAllocator<A_Object>,
        class __F_Config = F_DefaultObjectConfig
    >
    auto ShareObject(
        const TW_Unmanaged<__F_PassedObject, __F_Config>& WeakP
    ) noexcept
    {
        return ObjectSmartPointerTemplates::ShareObject<__F_PassedObject, __F_Allocator, __F_Config>(WeakP);
    }
}

#ifdef ABYTEK_OBJECT_CUSTOM_OBJECT_POINTER_CONFIG_EXTENSION
#undef ABYTEK_OBJECT_CUSTOM_OBJECT_POINTER_CONFIG_EXTENSION
#endif

#undef ABYTEK_OBJECT_SMART_POINTER_TYPEDEFS_NAMESPACE