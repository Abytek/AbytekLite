/**
 * @file ObjectUniqueSmartPointerTemplate.hpp
 * @brief Implements unique ownership smart pointer functionality.
 * 
 * This file contains the implementation of TU (Template Unique), a smart
 * pointer type similar to std::unique_ptr that provides exclusive ownership
 * of dynamically allocated objects with automatic cleanup.
 */
#pragma once

#include "StandardObjectManagement.hpp"
#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/ObjectBase.hpp"
#include "Abytek/ObjectHeader.hpp"
#include "Abytek/ObjectMemory.hpp"
#include "Abytek/ObjectWeakSmartPointerTemplate.hpp"
#include "Abytek/ObjectTracking.hpp"
#include "Abytek/DefaultObjectReleaser.hpp"


namespace Abytek::ObjectSmartPointerTemplates
{    
    /**
     * @brief Unique ownership smart pointer template.
     * 
     * TU provides exclusive ownership of dynamically allocated objects,
     * automatically handling their destruction when the pointer is destroyed
     * or reset. It cannot be copied, only moved.
     * 
     * Features:
     * - Exclusive ownership semantics
     * - Automatic memory management
     * - Object creation helpers
     * - Type-safe casting (static and dynamic)
     * - Configuration switching (validation modes)
     * 
     * Object Creation:
     * The recommended way to create objects with TU is using the function call operator:
     * 
     * TU<Type>()(args...)
     * 
     * Examples:
     * - TU<F_Foo>()()           // Create with default constructor
     * - TU<F_Foo>()(42)         // Create with parameters
     * - TU<F_Foo>()(100, "Bar") // Create with multiple parameters
     * 
     * This syntax creates a temporary default-constructed smart pointer and
     * immediately calls its function call operator to create and return a
     * new smart pointer that owns the newly created object.
     * 
     * Note: Do NOT use the F_ObjectQuickCreateFlag constructor directly.
     * Always prefer the function call operator syntax for object creation.
     * 
     * @tparam __F_PassedObject The object type this pointer owns
     * @tparam __F_Allocator The allocator type used for memory management
     * @tparam __F_Config Pointer configuration settings
     */
    template<typename __F_PassedObject, class __F_Allocator, class __F_Config>
    class TU final
    {
    public:
        template<typename __F_PassedObject2, class __F_Allocator2, class __F_Config2>
        friend class TU;
        template<typename __F_PassedObject2, class __F_Allocator2, class __F_Config2>
        friend class TS;
        template<typename __F_PassedObject2, class __F_Allocator2, class __F_Config2>
        friend class TRA;
        
    public:
        using F_PassedObject = __F_PassedObject;
        using F_Allocator = __F_Allocator;
        using F_Config = __F_Config;

    public:
        using F_Object = std::remove_const_t<std::remove_reference_t<F_PassedObject>>;

    public:
        template<typename __F_PassedObject2>
        using TF_RebindObject = TU< __F_PassedObject2, F_Allocator, F_Config>;
        template<typename __F_Allocator2>
        using TF_RebindAllocator = TU<__F_PassedObject, __F_Allocator2, F_Config>;
        template<class __F_Config2>
        using TF_RebindConfig = TU<F_PassedObject, F_Allocator, __F_Config2>;

    public:
        using F_WithValidation = TF_RebindConfig<typename F_Config::F_WithValidation>;
        using F_NoValidation = TF_RebindConfig<typename F_Config::F_NoValidation>;

    public:
        using W = TW<F_PassedObject, F_Config>;
        
    private:
        using F_ObjectKey = typename F_Config::F_ObjectManagement::F_ObjectKey;
        using F_ObjectKeyPool = typename F_Config::F_ObjectManagement::F_ObjectKeyPool;
        using E_ObjectTrackingFlag = typename F_Config::F_ObjectManagement::E_ObjectTrackingFlag;

    private:
        using F_This = TU<F_PassedObject, F_Allocator, F_Config>;
        
    private:
        F_PassedObject* _ObjectRawP;
        F_ObjectKey _ObjectKey;

    public:
        /**
         * @brief Gets the raw pointer to the owned object.
         * @return Raw pointer to the object
         */
        ABYTEK_FORCE_INLINE F_PassedObject* GetObjectRawP() const noexcept
        {
            return _ObjectRawP;
        }
        
        /**
         * @brief Gets the object key for the owned object.
         * @return Object key value
         */
        ABYTEK_FORCE_INLINE F_ObjectKey GetObjectKey() const noexcept
        {
            return _ObjectKey;
        }

    public:
        /**
         * @brief Default constructor, creates a null pointer.
         */
        ABYTEK_FORCE_INLINE constexpr TU() noexcept
        {
            SetDefault();
        }
        
        /**
         * @brief Constructor with in-place object creation.
         * @tparam __F_Args Argument types for object constructor
         * @param Flag Quick create flag marker
         * @param Args Constructor arguments for the object
         * 
         * @note This constructor should not be used directly. 
         * Prefer using the function call operator syntax: TU<Type>()(args...)
         */
        template<typename... __F_Args>
        ABYTEK_FORCE_INLINE TU(F_ObjectQuickCreateFlag, __F_Args&&... Args) noexcept
        {
            CreateObjectUnsafe(ABYTEK_FORWARD(Args)...);
        }
        
        /**
         * @brief Constructor from raw pointer and object key.
         * @param ObjectRawP Raw pointer to the object
         * @param ObjectKey Object key for validation
         */
        ABYTEK_FORCE_INLINE explicit TU(F_PassedObject* ObjectRawP, F_ObjectKey ObjectKey) noexcept :
            _ObjectRawP(ObjectRawP),
            _ObjectKey(ObjectKey)
        {
            _EnsureValidationIfRequired();
        }
        
        /**
         * @brief Constructor from raw pointer.
         * @param ObjectRawP Raw pointer to the object
         */
        ABYTEK_FORCE_INLINE explicit TU(F_PassedObject* ObjectRawP) noexcept :
            _ObjectRawP(ObjectRawP),
            _ObjectKey(Abytek::GetObjectKey<typename F_Config::F_ObjectManagement>((void*)ObjectRawP))
        {
            _EnsureValidationIfRequired();
        }
        
        /**
         * @brief Destructor, automatically destroys the owned object.
         */
        ABYTEK_FORCE_INLINE ~TU() noexcept
        {
            ResetUnsafe();
        }
        
        /**
         * @brief Move constructor from compatible unique pointer.
         * @tparam __F_PassedObject2 Source object type (must be downcastable to this pointer's type)
         * @tparam __F_Allocator2 Source allocator type
         * @tparam __F_Config2 Source configuration type
         * @param X Source unique pointer to move from
         */
        ABYTEK_FORCE_INLINE TU(TU&& X) noexcept :
            _ObjectRawP(X._ObjectRawP),
            _ObjectKey(X._ObjectKey)
        {
            _EnsureValidationIfRequired();
            X.SetDefault();
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TU<__F_PassedObject, __F_Allocator, __F_Config>>()) << "incompatible value";
        }
        
        /**
         * @brief Move assignment operator from compatible unique pointer.
         * @tparam __F_PassedObject2 Source object type (must be downcastable to this pointer's type)
         * @tparam __F_Allocator2 Source allocator type
         * @tparam __F_Config2 Source configuration type
         * @param X Source unique pointer to move from
         * @return Reference to this pointer
         */
        ABYTEK_FORCE_INLINE TU& operator = (TU&& X) noexcept
        {
            if (((void*)this) == ((void*)&X))
            {
                return *this;
            }
            ResetUnsafe();
            _ObjectRawP = X._ObjectRawP;
            _ObjectKey = X._ObjectKey;
            _EnsureValidationIfRequired();
            X.SetDefault();
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TU<__F_PassedObject, __F_Allocator, __F_Config>>()) << "incompatible value";
            return *this;
        }
        
        /**
         * @brief Move constructor from compatible unique pointer.
         * @tparam __F_PassedObject2 Source object type (must be downcastable to this pointer's type)
         * @tparam __F_Allocator2 Source allocator type
         * @tparam __F_Config2 Source configuration type
         * @param X Source unique pointer to move from
        */
        template<
            typename __F_PassedObject2, class __F_Allocator2, class __F_Config2
            ABYTEK_REQUIRES(IsObjectDownCastable<__F_PassedObject2, __F_PassedObject>)
        >
        ABYTEK_FORCE_INLINE TU(TU<__F_PassedObject2, __F_Allocator2, __F_Config2>&& X) noexcept :
            _ObjectRawP(X._ObjectRawP),
            _ObjectKey(X._ObjectKey)
        {
            _EnsureValidationIfRequired();
            X.SetDefault();
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TU<__F_PassedObject2, __F_Allocator2, __F_Config2>>()) << "incompatible value";
        }
        
        /**
         * @brief Move assignment operator from compatible unique pointer.
         * @tparam __F_PassedObject2 Source object type (must be downcastable to this pointer's type)
         * @tparam __F_Allocator2 Source allocator type
         * @tparam __F_Config2 Source configuration type
         * @param X Source unique pointer to move from
         * @return Reference to this pointer
        */
        template<
            typename __F_PassedObject2, class __F_Allocator2, class __F_Config2
            ABYTEK_REQUIRES(IsObjectDownCastable<__F_PassedObject2, __F_PassedObject>)
        >
        ABYTEK_FORCE_INLINE TU& operator = (TU<__F_PassedObject2, __F_Allocator2, __F_Config2>&& X) noexcept
        {
            if (((void*)this) == ((void*)&X))
            {
                return *this;
            }
            ResetUnsafe();
            _ObjectRawP = X._ObjectRawP;
            _ObjectKey = X._ObjectKey;
            _EnsureValidationIfRequired();
            X.SetDefault();
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TU<__F_PassedObject2, __F_Allocator2, __F_Config2>>()) << "incompatible value";
            return *this;
        }

        TU(const TU&)
        {
            ABYTEK_BASE_OBJECT_ASSERT(false) << "Copy unique pointers is not allowed";
        }
        TU& operator = (const TU&)
        {
            ABYTEK_BASE_OBJECT_ASSERT(false) << "Copy unique pointers is not allowed";
            return *this;
        }

    public:
        /**
         * @brief Less than comparison operator.
         * @param X Pointer to compare against
         * @return True if this pointer's key is less than X's key
         */
        ABYTEK_FORCE_INLINE B8 operator < (const TU& X) const noexcept
        {
            return _ObjectKey.Value < X._ObjectKey.Value;
        }
        
        /**
         * @brief Less than or equal comparison operator.
         * @param X Pointer to compare against
         * @return True if this pointer's key is less than or equal to X's key
         */
        ABYTEK_FORCE_INLINE B8 operator <= (const TU& X) const noexcept
        {
            return _ObjectKey.Value <= X._ObjectKey.Value;
        }
        
        /**
         * @brief Greater than comparison operator.
         * @param X Pointer to compare against
         * @return True if this pointer's key is greater than X's key
         */
        ABYTEK_FORCE_INLINE B8 operator > (const TU& X) const noexcept
        {
            return _ObjectKey.Value > X._ObjectKey.Value;
        }
        
        /**
         * @brief Greater than or equal comparison operator.
         * @param X Pointer to compare against
         * @return True if this pointer's key is greater than or equal to X's key
         */
        ABYTEK_FORCE_INLINE B8 operator >= (const TU& X) const noexcept
        {
            return _ObjectKey.Value >= X._ObjectKey.Value;
        }

    private:
        /**
         * @brief Internal method to perform validation if required by configuration.
         */
        ABYTEK_FORCE_INLINE void _EnsureValidationIfRequired() const noexcept
        {
            if constexpr (F_Config::EnableValidation)
            {
                EnsureValidation();
            }
        }
        
    public:
        /**
         * @brief Explicitly validates the pointer, asserting if invalid.
         */
        ABYTEK_FORCE_INLINE void EnsureValidation() const noexcept
        {
#ifdef ABYTEK_OBJECT_VALIDATION_ENABLE_FAST_MODE
            ABYTEK_BASE_OBJECT_ASSERT(Q_IsValid())
                << "invalid object, object type: \""
                << TypeFullName<__F_PassedObject>()
                << "\"";
            return;
#endif
            
#ifdef ABYTEK_BASE_OBJECT_ENABLE_ASSERTIONS
            auto Flags = F_ObjectKeyPool::GetInstance().GetTrackingFlag(_ObjectKey);
#endif
            ABYTEK_BASE_OBJECT_ASSERT(FlagHas(Flags, E_ObjectTrackingFlag::NONE))
                << "invalid object, object type: \""
                << TypeFullName<__F_PassedObject>()
                << "\", missing required flags: "
                << static_cast<U64>(E_ObjectTrackingFlag::NONE)
                << ", current flags: "
                << static_cast<U64>(Flags);
            ABYTEK_BASE_OBJECT_ASSERT(!FlagHas(Flags, E_ObjectTrackingFlag::INVALID))
                << "invalid object, object type: \""
                << TypeFullName<__F_PassedObject>()
                << "\", having forbidden flags: "
                << static_cast<U64>(E_ObjectTrackingFlag::INVALID)
                << ", current flags: "
                << static_cast<U64>(Flags);
        }

    public:
        /**
         * @brief Sets the pointer to default/null state without validation.
         */
        ABYTEK_FORCE_INLINE void SetDefaultUnsafe()
        {
            if constexpr (F_Config::HasDefaultValue)
            {
                _ObjectRawP = 0;
                _ObjectKey = F_ObjectKey::Null();
            }
        }
        
        /**
         * @brief Resets the pointer, destroying the owned object if any, without validation.
         */
        ABYTEK_FORCE_INLINE void ResetUnsafe()
        {
            if(_ObjectRawP)
            {
                DestroyObject();
            }            
            SetDefaultUnsafe();
        }

    public:
        /**
         * @brief Sets the pointer to default/null state with validation if required.
         */
        ABYTEK_FORCE_INLINE void SetDefault()
        {
            SetDefaultUnsafe();
            _EnsureValidationIfRequired();
        }
        
        /**
         * @brief Resets the pointer, destroying the owned object if any, with validation if required.
         */
        ABYTEK_FORCE_INLINE void Reset()
        {
            ResetUnsafe();
            _EnsureValidationIfRequired();
        }
        
    public:
        template<typename... __F_Args>
        static TU CreateObjectAdvanced(F_ObjectReleaser ObjectReleaser, __F_Args&&... Args)
        {
            TU Object;
            Object.CreateObjectAdvancedUnsafe(
                ObjectReleaser,
                ABYTEK_FORWARD(Args)...    
            );
            return ABYTEK_MOVE(Object);
        }
        
    public:
        void SetObjectReleaser(F_ObjectReleaser ObjectReleaser)
        {
            ABYTEK_BASE_OBJECT_ASSERT(*this) << "Cannot set releaser for invalid objects";
            GetObjectReleaser<typename __F_Config::F_ObjectManagement>(_ObjectRawP) = ObjectReleaser;
        }

    public:
        template<typename... __F_Args>
        void CreateObjectUnsafe(__F_Args&&... Args)
        {
            using F_ObjectHeader = TF_ManagedObjectHeader<F_Object, typename F_Config::F_ObjectManagement>;
            using F_ObjectMemory = TF_ManagedObjectMemory<F_Object, typename F_Config::F_ObjectManagement>;
            
            using F_ObjectMemoryAllocator = Abytek::TF_RebindAllocator<F_ObjectMemory, F_Allocator>;
            
            CreateObjectAdvancedUnsafe(
                GetDefaultManagedObjectReleaser<F_Object, F_Allocator, typename F_Config::F_ObjectManagement>(),
                ABYTEK_FORWARD(Args)...
            );
        }
        template<typename... __F_Args>
        void CreateObjectAdvancedUnsafe(F_ObjectReleaser ObjectReleaser, __F_Args&&... Args)
        {
            using F_ObjectHeader = TF_ManagedObjectHeader<F_Object, typename F_Config::F_ObjectManagement>;
            using F_ObjectMemory = TF_ManagedObjectMemory<F_Object, typename F_Config::F_ObjectManagement>;
            
            using F_ObjectMemoryAllocator = Abytek::TF_RebindAllocator<F_ObjectMemory, F_Allocator>;
            
            F_ObjectMemoryAllocator Allocator;
            F_ObjectMemory* ObjectMemory = (F_ObjectMemory*)Allocator.allocate(1);

            ObjectMemory->ObjectHeader.GetReleaser() = ObjectReleaser;
            ObjectMemory->ObjectHeader.GetCounter().store(1, boost::memory_order_release);
            _ObjectRawP = (F_PassedObject*)(ObjectMemory->GetObjectRawP());
            
            _ObjectKey = F_ObjectKeyPool::GetInstance().Register();
            ObjectMemory->ObjectHeader.GetKey() = _ObjectKey;
            
            new ((F_Object*)_ObjectRawP) F_Object { ABYTEK_FORWARD(Args)... };
            
#ifdef ABYTEK_DEBUG_INFO
            TrackObject_Active(_ObjectRawP);
#endif
        }
        void DestroyObjectUnsafe() noexcept
        {
            GetObjectReleaser<typename F_Config::F_ObjectManagement>(_ObjectRawP)(_ObjectRawP);
        }

    public:
        /**
         * @brief Creates a new object with validation checking.
         * 
         * Verifies the pointer is null before creating a new object.
         * 
         * @tparam __F_Args Argument types for object constructor
         * @param Args Constructor arguments for the object
         */
        template<typename... __F_Args>
        void CreateObject(__F_Args&&... Args)
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsNull()) << "cannot create object on a valid object smart pointer";

            CreateObjectUnsafe(ABYTEK_FORWARD(Args)...);
        }
        
        /**
         * @brief Destroys the owned object with validation checking.
         * 
         * Verifies the pointer is valid before destroying the object.
         */
        void DestroyObject() noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot destroy an invalid object";

            DestroyObjectUnsafe();
        }

    public:
        /**
         * @brief Function call operator for creating objects.
         * 
         * Creates a new unique pointer owning an object constructed with the given arguments.
         * This is the recommended way to create objects with TU smart pointers.
         * 
         * Usage example:
         * TU<Type>()(args...)
         * 
         * @tparam __F_Args Argument types for object constructor
         * @param Args Constructor arguments for the object
         * @return New unique pointer owning the created object
         */
        template<typename... __F_Args>
        F_This operator () (__F_Args&&... Args) const
        {
            return F_This(F_ObjectQuickCreateFlag {}, ABYTEK_FORWARD(Args)...);
        }
        
    public:
        /**
         * @brief Checks if the pointer is valid with validation enabled.
         * @return True if the pointer is valid or if validation is forced
         */
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            if constexpr (F_Config::EnableValidation)
            {
                return true;
            }

            return _ObjectRawP;
        }
        
        /**
         * @brief Checks if the pointer is null with validation enabled.
         * @return True if the pointer is null and validation is not forced
         */
        ABYTEK_FORCE_INLINE B8 IsNull() const noexcept
        {
            if constexpr (F_Config::EnableValidation)
            {
                return false;
            }

            return !_ObjectRawP;
        }
        
        /**
         * @brief Slow check if the pointer is valid.
         * @return True if the pointer is valid, regardless of validation settings
         */
        ABYTEK_FORCE_INLINE B8 NQ_IsValid() const noexcept
        {
            if(_ObjectRawP)
            {
                return F_ObjectKeyPool::GetInstance().Check(_ObjectKey);
            }
            return false;
        }
        
        /**
         * @brief Slow check if the pointer is null.
         * @return True if the pointer is null, regardless of validation settings
         */
        ABYTEK_FORCE_INLINE B8 NQ_IsNull() const noexcept
        {
            if(_ObjectRawP)
            {
                return !F_ObjectKeyPool::GetInstance().Check(_ObjectKey);
            }
            return true;
        }

        /**
         * @brief Slow check with callback if valid.
         * @tparam __F_CallbackIfValid Callback type to execute if the pointer is valid
         * @param CallbackIfValid Callback to execute if the pointer is valid
         * @return True if the pointer is valid and callback was executed
         */
        template<typename __F_CallbackIfValid>
		ABYTEK_FORCE_INLINE B8 NQ_IsValid(__F_CallbackIfValid&& CallbackIfValid) const noexcept
        {
            if(_ObjectRawP)
            {
                return F_ObjectKeyPool::GetInstance().CheckAndCallbackIfValid(_ObjectKey, ABYTEK_FORWARD(CallbackIfValid));
            }
            return false;
        }
        
        ABYTEK_FORCE_INLINE B8 IsValid_Exclusive() const noexcept
        {
            if(_ObjectRawP)
            {
                return F_ObjectKeyPool::GetInstance().CheckExclusive(_ObjectKey);
            }
            return false;
        }
        template<typename __F_CallbackIfValid>
        ABYTEK_FORCE_INLINE B8 IsValid_Exclusive(__F_CallbackIfValid&& CallbackIfValid) const noexcept
        {
            if(_ObjectRawP)
            {
                return F_ObjectKeyPool::GetInstance().CheckExclusiveAndCallbackIfValid(_ObjectKey, ABYTEK_FORWARD(CallbackIfValid));
            }
            return false;
        }

        /**
         * @brief Quick check if the pointer is not null.
         * @return True if the raw pointer is not null
         */
        ABYTEK_FORCE_INLINE B8 Q_IsValid() const noexcept
        {
            return (_ObjectRawP != 0);
        }
        
        /**
         * @brief Quick check if the pointer is null.
         * @return True if the raw pointer is null
         */
        ABYTEK_FORCE_INLINE B8 Q_IsNull() const noexcept
        {
            return (_ObjectRawP == 0);
        }

    public:
        /**
         * @brief Boolean conversion operator for validity checking.
         * @return True if the pointer is valid
         */
        ABYTEK_FORCE_INLINE explicit operator bool() const noexcept
        {
            return IsValid();
        }
        
        /**
         * @brief Arrow operator for member access.
         * @return Pointer to the owned object
         */
        ABYTEK_FORCE_INLINE F_PassedObject* operator -> () const noexcept
        {       
            _EnsureValidationIfRequired();
            ABYTEK_BASE_OBJECT_ASSERT(*this) << "cannot access invalid objects";

            return _ObjectRawP;
        }
        
        /**
         * @brief Dereference operator for object access.
         * @return Reference to the owned object
         */
        ABYTEK_FORCE_INLINE F_PassedObject& operator * () const noexcept
        {      
            _EnsureValidationIfRequired();
            ABYTEK_BASE_OBJECT_ASSERT(*this) << "cannot access invalid objects";

            return *_ObjectRawP;
        }

    public:
        /**
         * @brief Checks if the object can be dynamically cast to another type.
         * @tparam __F_Object2 Target type to check casting to
         * @return True if the cast is possible
         */
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE B8 CanDynamicCast() const noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot try to dynamic cast on an invalid object smart pointer";
            
            return CheckObjectInterface<__F_Object2>(_ObjectRawP);
        }
        
        /**
         * @brief Checks if the object can be statically cast to another type.
         * @tparam __F_Object2 Target type to check casting to
         * @return True if the static cast is possible
         */
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE B8 CanStaticCast() const noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot try to dynamic cast on an invalid object smart pointer";
            
            return CheckObjectInterfaceForStaticCasting_UnchangedPointer<__F_Object2>(_ObjectRawP);
        }
        
        /**
         * @brief Attempts to dynamically cast the object and store the result in a weak pointer.
         * @tparam __F_Object2 Target type to cast to
         * @param OutP Output weak pointer to store the result if cast succeeds
         * @return True if the cast succeeded
         */
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE B8 TryDynamicCast(typename TF_RebindObject<__F_Object2>::template TF_RebindConfig<typename F_Config::F_NoValidation>::W& OutP) const noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot try to dynamic cast on an invalid object smart pointer";
            
            __F_Object2* CastedObjectRawP = TryCastObject<__F_Object2>(_ObjectRawP);
            if(CastedObjectRawP)
            {
                OutP = typename TF_RebindObject<__F_Object2>::template TF_RebindConfig<typename F_Config::F_NoValidation>::W(CastedObjectRawP, _ObjectKey);
                return true;
            }
            return false;
        }
        
        /**
         * @brief Checks if the object has the specified polymorphic type.
         * @tparam __F_Object2 Type to check for
         * @return True if the object is or inherits from the specified type
         */
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE B8 CheckPolymorphism() const noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot try to dynamic cast on an invalid object smart pointer";
            
            return (TryCastObject<__F_Object2>(_ObjectRawP) != 0);
        }
        /**
         * @brief Performs a dynamic cast to a weak pointer of another type.
         * @tparam __F_Object2 Target type to cast to
         * @return Weak pointer to the object as the target type
         */
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE typename TF_RebindObject<__F_Object2>::W DynamicCast() const noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot dynamic cast on an invalid object smart pointer";
            ABYTEK_BASE_OBJECT_ASSERT(CheckObjectInterface<__F_Object2>(_ObjectRawP)) << "invalid object polymorphism";

            if constexpr (std::is_same_v<std::remove_const_t<__F_Object2>, F_Object>)
            {
                return TF_RebindObject<__F_Object2>::W(
                    (__F_Object2*)_ObjectRawP,
                    _ObjectKey
                );
            }
            else
            {
                return TF_RebindObject<__F_Object2>::W(
                    TryCastObject<__F_Object2>(_ObjectRawP),
                    _ObjectKey
                );
            }
        }
        /**
         * @brief Performs a static cast to another type (rvalue).
         * @tparam __F_Object2 Target type to cast to
         * @return Rvalue reference to unique pointer of the target type
         */
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE TF_RebindObject<__F_Object2>&& StaticCast() && noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot static cast on an invalid object smart pointer of type: " << TypeFullName<F_Object>();
            ABYTEK_BASE_OBJECT_ASSERT(CheckObjectInterfaceForStaticCasting_UnchangedPointer<__F_Object2>(_ObjectRawP)) << "invalid object polymorphism for static casting";

            using F_Target = TF_RebindObject<__F_Object2>;
            return (F_Target&&)*((F_Target*)this);
        }
        /**
         * @brief Performs a static cast to another type (const rvalue).
         * @tparam __F_Object2 Target type to cast to
         * @return Const rvalue reference to unique pointer of the target type
         */
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE TF_RebindObject<__F_Object2> const && StaticCast() const && noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot static cast on an invalid object smart pointer of type: " << TypeFullName<F_Object>();
            ABYTEK_BASE_OBJECT_ASSERT(CheckObjectInterfaceForStaticCasting_UnchangedPointer<__F_Object2>(_ObjectRawP)) << "invalid object polymorphism for static casting";

            using F_Target = TF_RebindObject<__F_Object2> const;
            return (F_Target&&)*((F_Target*)this);
        }
        /**
         * @brief Performs a static cast to another type (lvalue).
         * @tparam __F_Object2 Target type to cast to
         * @return Reference to unique pointer of the target type
         */
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE TF_RebindObject<__F_Object2>& StaticCast() & noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot static cast on an invalid object smart pointer of type: " << TypeFullName<F_Object>();
            ABYTEK_BASE_OBJECT_ASSERT(CheckObjectInterfaceForStaticCasting_UnchangedPointer<__F_Object2>(_ObjectRawP)) << "invalid object polymorphism for static casting";

            using F_Target = TF_RebindObject<__F_Object2>;
            return (F_Target&)*((F_Target*)this);
        }
        /**
         * @brief Performs a static cast to another type (const lvalue).
         * @tparam __F_Object2 Target type to cast to
         * @return Const reference to unique pointer of the target type
         */
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE TF_RebindObject<__F_Object2> const & StaticCast() const & noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot static cast on an invalid object smart pointer of type: " << TypeFullName<F_Object>();
            ABYTEK_BASE_OBJECT_ASSERT(CheckObjectInterfaceForStaticCasting_UnchangedPointer<__F_Object2>(_ObjectRawP)) << "invalid object polymorphism for static casting";

            using F_Target = TF_RebindObject<__F_Object2> const;
            return (F_Target&)*((F_Target*)this);
        }
        
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE TF_RebindObject<__F_Object2>&& FastCast() && noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot static cast on an invalid object smart pointer of type: " << TypeFullName<F_Object>();
            ABYTEK_BASE_OBJECT_ASSERT(CheckObjectInterfaceForStaticCasting_UnchangedPointer<__F_Object2>(_ObjectRawP)) << "invalid object polymorphism for static casting";

            using F_Target = TF_RebindObject<__F_Object2>;
            return (F_Target&&)*((F_Target*)this);
        }
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE TF_RebindObject<__F_Object2> const && FastCast() const && noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot static cast on an invalid object smart pointer of type: " << TypeFullName<F_Object>();
            ABYTEK_BASE_OBJECT_ASSERT(CheckObjectInterfaceForStaticCasting_UnchangedPointer<__F_Object2>(_ObjectRawP)) << "invalid object polymorphism for static casting";

            using F_Target = TF_RebindObject<__F_Object2> const;
            return (F_Target&&)*((F_Target*)this);
        }
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE TF_RebindObject<__F_Object2>& FastCast() & noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot static cast on an invalid object smart pointer of type: " << TypeFullName<F_Object>();
            ABYTEK_BASE_OBJECT_ASSERT(CheckObjectInterfaceForStaticCasting_UnchangedPointer<__F_Object2>(_ObjectRawP)) << "invalid object polymorphism for static casting";

            using F_Target = TF_RebindObject<__F_Object2>;
            return (F_Target&)*((F_Target*)this);
        }
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE TF_RebindObject<__F_Object2> const & FastCast() const & noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot static cast on an invalid object smart pointer of type: " << TypeFullName<F_Object>();
            ABYTEK_BASE_OBJECT_ASSERT(CheckObjectInterfaceForStaticCasting_UnchangedPointer<__F_Object2>(_ObjectRawP)) << "invalid object polymorphism for static casting";

            using F_Target = TF_RebindObject<__F_Object2> const;
            return (F_Target&)*((F_Target*)this);
        }

    public:
        ABYTEK_FORCE_INLINE W Weak() const noexcept
        {
            return W(_ObjectRawP, _ObjectKey);
        }
    };

    /**
     * @brief Type alias for a unique pointer with validation enabled.
     * 
     * @tparam __F_PassedObject The object type this pointer owns
     * @tparam __F_Allocator The allocator type used for memory management
     * @tparam __F_Config Pointer configuration settings
     */
    template<typename __F_PassedObject, class __F_Allocator, class __F_Config>
    using TU_Valid = TU<__F_PassedObject, __F_Allocator, typename __F_Config::F_WithValidation>;
    
    /**
     * @brief Equality comparison operator for unique pointers.
     * 
     * @tparam __F_PassedObject1 First pointer's object type
     * @tparam __F_Allocator1 First pointer's allocator type
     * @tparam __F_Config1 First pointer's configuration
     * @tparam __F_PassedObject2 Second pointer's object type
     * @tparam __F_Allocator2 Second pointer's allocator type
     * @tparam __F_Config2 Second pointer's configuration
     * @param A First unique pointer
     * @param B Second unique pointer
     * @return True if both pointers reference the same object
     */
    template<typename __F_PassedObject1, class __F_Allocator1, class __F_Config1, typename __F_PassedObject2, class __F_Allocator2, class __F_Config2>
    ABYTEK_FORCE_INLINE bool operator == (
        const Abytek::ObjectSmartPointerTemplates::TU<__F_PassedObject1, __F_Allocator1, __F_Config1>& A,
        const Abytek::ObjectSmartPointerTemplates::TU<__F_PassedObject2, __F_Allocator2, __F_Config2>& B
    ) noexcept
    {
        return (
            (A.GetObjectRawP() == B.GetObjectRawP())
            && (A.GetObjectKey() == B.GetObjectKey())
        );
    }
    
    /**
     * @brief Inequality comparison operator for unique pointers.
     * 
     * @tparam __F_PassedObject1 First pointer's object type
     * @tparam __F_Allocator1 First pointer's allocator type
     * @tparam __F_Config1 First pointer's configuration
     * @tparam __F_PassedObject2 Second pointer's object type
     * @tparam __F_Allocator2 Second pointer's allocator type
     * @tparam __F_Config2 Second pointer's configuration
     * @param A First unique pointer
     * @param B Second unique pointer
     * @return True if the pointers reference different objects
     */
    template<typename __F_PassedObject1, class __F_Allocator1, class __F_Config1, typename __F_PassedObject2, class __F_Allocator2, class __F_Config2>
    ABYTEK_FORCE_INLINE bool operator != (
        const Abytek::ObjectSmartPointerTemplates::TU<__F_PassedObject1, __F_Allocator1, __F_Config1>& A,
        const Abytek::ObjectSmartPointerTemplates::TU<__F_PassedObject2, __F_Allocator2, __F_Config2>& B
    ) noexcept
    {
        return (
            (A.GetObjectRawP() != B.GetObjectRawP())
            || (A.GetObjectKey() != B.GetObjectKey())
        );
    }
}