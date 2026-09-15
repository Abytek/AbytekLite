/**
 * @file ObjectBase.hpp
 * @brief Provides foundational types and utilities for Abytek's object management system.
 * 
 * This file defines the base object class and core functionality for Abytek's smart pointer system,
 * including object validation, casting, and reference counting mechanisms.
 */
#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/Assert.hpp"
#include "Abytek/Allocator.hpp"


namespace Abytek
{
    /**
     * @brief Extension point for object pointer configuration customization.
     * 
     * Provides hooks for custom compatibility checking and validation logic
     * that can be specialized for specific pointer types.
     */
    struct F_ObjectPointerConfigExtension
    {
        template<typename __F_ObjectPointerTo, typename __F_ObjectPointerFrom>
        static constexpr B8 CustomIsCompatible()
        {
            return true;
        }
        template<typename __F_ObjectPointer>
        static constexpr B8 CustomCheck(const __F_ObjectPointer& ObjectP)
        {
            return true;
        }
    };
    
    /**
     * @brief Configuration template for object pointers.
     * 
     * Controls validation behavior, default value handling, and extension mechanisms
     * for different types of object pointers.
     * 
     * @tparam __F_ObjectManagement The object management strategy to use
     * @tparam __EnableValidation Whether to enable validation checks
     * @tparam __HasDefaultValue Whether the pointer type supports a default/null value
     * @tparam __F_Extension Extension type for custom behaviors
     */
    template<
        class __F_ObjectManagement,
        B8 __EnableValidation = false,
        B8 __HasDefaultValue = true,
        class __F_Extension = F_ObjectPointerConfigExtension
    >
    struct TF_ObjectPointerConfig
    {
        using F_ObjectManagement = __F_ObjectManagement;
        static constexpr B8 EnableValidation = __EnableValidation;
        static constexpr B8 HasDefaultValue = __HasDefaultValue;
        using F_Extension = __F_Extension;

        using F_WithValidation = TF_ObjectPointerConfig<
            F_ObjectManagement,
            true,
            HasDefaultValue,
            F_Extension
        >;
        using F_NoValidation = TF_ObjectPointerConfig<
            F_ObjectManagement,
            false,
            HasDefaultValue,
            F_Extension
        >;

        template<class __F_Extension2>
        using TF_RebindExtension = TF_ObjectPointerConfig<
            F_ObjectManagement,
            EnableValidation,
            HasDefaultValue,
            __F_Extension2
        >;
    };

    /**
     * @brief Base class for all managed objects in the Abytek system.
     * 
     * Provides the foundation for polymorphic objects that can be managed
     * by the smart pointer system. All objects that need to be used with
     * Abytek smart pointers must inherit from this class.
     */
    struct ABYTEK_BASE_OBJECT_API ABYTEK_ALIGN(sizeof(U64)) A_Object
{
    private:
#ifdef ABYTEK_DEBUG_INFO
        F_DebugName _DebugName;
#endif

    public:
#ifdef ABYTEK_DEBUG_INFO
        ABYTEK_FORCE_INLINE F_DebugName GetDebugName() const noexcept
        {
            return _DebugName;
        }
        virtual void SetDebugName(const F_DebugName& Value) noexcept
        {
            _DebugName = Value;
        }
#else
        // For compatibilities
        ABYTEK_FORCE_INLINE F_DebugName GetDebugName() const noexcept
        {
            return F_DebugName();
        }
        virtual void SetDebugName(const F_DebugName& Value) noexcept
        {
        }
#endif
    
    protected:
        A_Object() noexcept = default;

    public:
        virtual ~A_Object() noexcept = default;
        
        ABYTEK_DISABLE_COPY(A_Object);
        
    private:
        virtual void ___DEFINE_POLYMORPHISM___();
    };

    /**
     * @brief Checks if an object implements a specific interface.
     * 
     * Uses dynamic_cast to verify if the object can be cast to the target type.
     * 
     * @tparam __F_Target The target interface type
     * @tparam __F_PassedObject The actual object type
     * @param ObjectRawP Pointer to the object to check
     * @return True if the object implements the target interface
     */
    template<typename __F_Target, typename __F_PassedObject>
    ABYTEK_FORCE_INLINE B8 CheckObjectInterface(__F_PassedObject* ObjectRawP)
    {
        return (
            dynamic_cast<__F_Target*>(
                (std::remove_const_t<__F_PassedObject>*)ObjectRawP
            )
            != 0
        );
    }
    
    /**
     * @brief Attempts to cast an object to a target type.
     * 
     * @tparam __F_Target The target type to cast to
     * @tparam __F_PassedObject The source object type
     * @param ObjectRawP Pointer to the object to cast
     * @return Pointer to the cast object or nullptr if cast fails
     */
    template<typename __F_Target, typename __F_PassedObject>
    ABYTEK_FORCE_INLINE __F_Target* TryCastObject(__F_PassedObject* ObjectRawP)
    {
        return dynamic_cast<__F_Target*>(
            (std::remove_const_t<__F_PassedObject>*)ObjectRawP
        );
    }
    
    /**
     * @brief Checks if an object can be statically cast to a target type.
     * 
     * @tparam __F_Target The target type
     * @tparam __F_PassedObject The source object type
     * @param ObjectRawP Pointer to the object to check
     * @return True if the static cast is valid
     */
    template<typename __F_Target, typename __F_PassedObject>
    ABYTEK_FORCE_INLINE B8 CheckObjectInterfaceForStaticCasting(__F_PassedObject* ObjectRawP)
    {
        return (
            TryCastObject<__F_Target>(ObjectRawP)
            || !ObjectRawP
        );
    }

    template<typename __F_Target, typename __F_PassedObject>
    ABYTEK_FORCE_INLINE B8 CheckObjectInterfaceForStaticCasting_UnchangedPointer(__F_PassedObject* ObjectRawP)
    {
        return (
            (TryCastObject<__F_Target>(ObjectRawP) == ((__F_Target*)ObjectRawP))
            && (ObjectRawP != 0)
        );
    }
    
    /**
     * @brief Type trait to check if one type can be downcast to another.
     * 
     * @tparam __F_From Source type
     * @tparam __F_To Target type
     */
    template<typename __F_From, typename __F_To>
    static constexpr B8 IsObjectDownCastable = std::is_convertible_v<__F_From*, __F_To*>;
    
    /**
     * @brief Type trait to check if one type can be upcast to another.
     * 
     * @tparam __F_From Source type
     * @tparam __F_To Target type
     */
    template<typename __F_From, typename __F_To>
    static constexpr B8 IsObjectUpCastable = (
        IsObjectDownCastable<__F_To, __F_From>
        && !IsObjectDownCastable<__F_From, __F_To>
    );
    
    /**
     * @brief Function pointer type for object release operations.
     */
    using F_ObjectReleaser = void (*)(void*);

    /**
     * @brief Flag type for object quick creation.
     */
    struct F_ObjectQuickCreateFlag {};
    
    /**
     * @brief Flag type for object quick obtaining.
     */
    struct F_ObjectQuickOptainRawFlag {};

    /**
     * @brief Retrieves the object key from an object pointer.
     * 
     * @tparam __F_ObjectManagement Object management strategy
     * @param ObjectRawP Raw pointer to the object
     * @return Reference to the object key
     */
    template<class __F_ObjectManagement>
    ABYTEK_FORCE_INLINE typename __F_ObjectManagement::F_ObjectKey& GetObjectKey(void* ObjectRawP) noexcept
    {
        return ((typename __F_ObjectManagement::F_ObjectKey*)((U8*)ObjectRawP))[-1];
    }
    
    /**
     * @brief Retrieves the object releaser from an object pointer.
     * 
     * @tparam __F_ObjectManagement Object management strategy
     * @param ObjectRawP Raw pointer to the object
     * @return Reference to the object releaser
     */
    template<class __F_ObjectManagement>
    ABYTEK_FORCE_INLINE F_ObjectReleaser& GetObjectReleaser(void* ObjectRawP) noexcept
    {
        return ((F_ObjectReleaser*)(((U8*)ObjectRawP) - sizeof(typename __F_ObjectManagement::F_ObjectKey)))[-1];
    }
    
    /**
     * @brief Retrieves the reference counter from an object pointer.
     * 
     * @tparam __F_ObjectManagement Object management strategy
     * @param ObjectRawP Raw pointer to the object
     * @return Reference to the atomic reference counter
     */
    template<class __F_ObjectManagement>
    ABYTEK_FORCE_INLINE AU32& GetObjectCounter(void* ObjectRawP) noexcept
    {
        return ((AU32*)(((U8*)ObjectRawP) - sizeof(typename __F_ObjectManagement::F_ObjectKey) - sizeof(F_ObjectReleaser)))[-1];
    }

    /**
     * @brief Increases the reference count of a shared object.
     * 
     * @tparam __F_ObjectManagement Object management strategy
     * @param ObjectRawP Raw pointer to the object
     * @return Previous reference count
     */
    template<class __F_ObjectManagement>
    ABYTEK_FORCE_INLINE U32 IncreaseSharedObjectCounter(void* ObjectRawP)
    {
        return GetObjectCounter<__F_ObjectManagement>(ObjectRawP).fetch_add(1, boost::memory_order_release);
    }
    
    /**
     * @brief Decreases the reference count of a shared object.
     * 
     * @tparam __F_ObjectManagement Object management strategy
     * @param ObjectRawP Raw pointer to the object
     * @return New reference count
     */
    template<class __F_ObjectManagement>
    ABYTEK_FORCE_INLINE U32 DecreaseSharedObjectCounter(void* ObjectRawP)
    {
        return GetObjectCounter<__F_ObjectManagement>(ObjectRawP).fetch_sub(1, boost::memory_order_release);
    }
    
    /**
     * @brief Sets the reference count of a shared object to one.
     * 
     * @tparam __F_ObjectManagement Object management strategy
     * @param ObjectRawP Raw pointer to the object
     */
    template<class __F_ObjectManagement>
    ABYTEK_FORCE_INLINE void SetSharedObjectCounterToOne(void* ObjectRawP)
    {
        GetObjectCounter<__F_ObjectManagement>(ObjectRawP).store(1, boost::memory_order_release);
    }
    
    ABYTEK_FORCE_INLINE F_ObjectReleaser& GetUnmanagedObjectReleaser(void* ObjectRawP) noexcept
    {
        return ((F_ObjectReleaser*)((U8*)ObjectRawP))[-1];
    }
    
    ABYTEK_FORCE_INLINE AU32& GetUnmanagedObjectCounter(void* ObjectRawP) noexcept
    {
        return ((AU32*)(((U8*)ObjectRawP) - sizeof(F_ObjectReleaser)))[-1];
    }

    ABYTEK_FORCE_INLINE U32 IncreaseSharedUnmanagedObjectCounter(void* ObjectRawP)
    {
        return GetUnmanagedObjectCounter(ObjectRawP).fetch_add(1, boost::memory_order_release);
    }
    
    ABYTEK_FORCE_INLINE U32 DecreaseSharedUnmanagedObjectCounter(void* ObjectRawP)
    {
        return GetUnmanagedObjectCounter(ObjectRawP).fetch_sub(1, boost::memory_order_release);
    }
    
    ABYTEK_FORCE_INLINE void SetSharedUnmanagedObjectCounterToOne(void* ObjectRawP)
    {
        GetUnmanagedObjectCounter(ObjectRawP).store(1, boost::memory_order_release);
    }
}