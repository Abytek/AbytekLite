/**
 * @file ObjectUniqueSmartPointerTemplate.hpp
 * @brief Implements unique ownership smart pointer functionality.
 * 
 * This file contains the implementation of TU_Unmanaged (Template Unique), a smart
 * pointer type similar to std::unique_ptr that provides exclusive ownership
 * of dynamically allocated objects with automatic cleanup.
 */
#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/ObjectBase.hpp"
#include "Abytek/ObjectHeader.hpp"
#include "Abytek/ObjectMemory.hpp"
#include "Abytek/UnmanagedObjectWeakSmartPointerTemplate.hpp"


namespace Abytek::ObjectSmartPointerTemplates
{    
    template<typename __F_PassedObject, class __F_Allocator, class __F_Config>
    class TU_Unmanaged final
    {
    public:
        template<typename __F_PassedObject2, class __F_Allocator2, class __F_Config2>
        friend class TU_Unmanaged;
        template<typename __F_PassedObject2, class __F_Allocator2, class __F_Config2>
        friend class TS_Unmanaged;
        
    public:
        using F_PassedObject = __F_PassedObject;
        using F_Allocator = __F_Allocator;
        using F_Config = __F_Config;

    public:
        using F_Object = std::remove_const_t<std::remove_reference_t<F_PassedObject>>;

    public:
        template<typename __F_PassedObject2>
        using TF_RebindObject = TU_Unmanaged< __F_PassedObject2, F_Allocator, F_Config>;
        template<typename __F_Allocator2>
        using TF_RebindAllocator = TU_Unmanaged<__F_PassedObject, __F_Allocator2, F_Config>;
        template<class __F_Config2>
        using TF_RebindConfig = TU_Unmanaged<F_PassedObject, F_Allocator, __F_Config2>;

    public:
        using F_WithValidation = TF_RebindConfig<typename F_Config::F_WithValidation>;
        using F_NoValidation = TF_RebindConfig<typename F_Config::F_NoValidation>;

    public:
        using W = TW_Unmanaged<F_PassedObject, F_Config>;

    private:
        using F_This = TU_Unmanaged<F_PassedObject, F_Allocator, F_Config>;
        
    private:
        F_PassedObject* _ObjectRawP;

    public:
        /**
         * @brief Gets the raw pointer to the owned object.
         * @return Raw pointer to the object
         */
        ABYTEK_FORCE_INLINE F_PassedObject* GetObjectRawP() const noexcept
        {
            return _ObjectRawP;
        }

    public:
        /**
         * @brief Default constructor, creates a null pointer.
         */
        ABYTEK_FORCE_INLINE constexpr TU_Unmanaged() noexcept
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
         * Prefer using the function call operator syntax: TU_Unmanaged<Type>()(args...)
         */
        template<typename... __F_Args>
        ABYTEK_FORCE_INLINE TU_Unmanaged(F_ObjectQuickCreateFlag, __F_Args&&... Args) noexcept
        {
            CreateObjectUnsafe(ABYTEK_FORWARD(Args)...);
        }
        
        /**
         * @brief Constructor from raw pointer and object key.
         * @param ObjectRawP Raw pointer to the object
         */
        ABYTEK_FORCE_INLINE explicit TU_Unmanaged(F_PassedObject* ObjectRawP) noexcept :
            _ObjectRawP(ObjectRawP)
        {
            _EnsureValidationIfRequired();
        }
        
        /**
         * @brief Destructor, automatically destroys the owned object.
         */
        ABYTEK_FORCE_INLINE ~TU_Unmanaged() noexcept
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
        ABYTEK_FORCE_INLINE TU_Unmanaged(TU_Unmanaged&& X) noexcept :
            _ObjectRawP(X._ObjectRawP)
        {
            _EnsureValidationIfRequired();
            X.SetDefault();
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TU_Unmanaged<__F_PassedObject, __F_Allocator, __F_Config>>()) << "incompatible value";
        }
        
        /**
         * @brief Move assignment operator from compatible unique pointer.
         * @tparam __F_PassedObject2 Source object type (must be downcastable to this pointer's type)
         * @tparam __F_Allocator2 Source allocator type
         * @tparam __F_Config2 Source configuration type
         * @param X Source unique pointer to move from
         * @return Reference to this pointer
        */
        ABYTEK_FORCE_INLINE TU_Unmanaged& operator = (TU_Unmanaged&& X) noexcept
        {
            if (((void*)this) == ((void*)&X))
            {
                return *this;
            }
            ResetUnsafe();
            _ObjectRawP = X._ObjectRawP;
            _EnsureValidationIfRequired();
            X.SetDefault();
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TU_Unmanaged<__F_PassedObject, __F_Allocator, __F_Config>>()) << "incompatible value";
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
        ABYTEK_FORCE_INLINE TU_Unmanaged(TU_Unmanaged<__F_PassedObject2, __F_Allocator2, __F_Config2>&& X) noexcept :
            _ObjectRawP(X._ObjectRawP)
        {
            _EnsureValidationIfRequired();
            X.SetDefault();
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TU_Unmanaged<__F_PassedObject2, __F_Allocator2, __F_Config2>>()) << "incompatible value";
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
        ABYTEK_FORCE_INLINE TU_Unmanaged& operator = (TU_Unmanaged<__F_PassedObject2, __F_Allocator2, __F_Config2>&& X) noexcept
        {
            if (((void*)this) == ((void*)&X))
            {
                return *this;
            }
            ResetUnsafe();
            _ObjectRawP = X._ObjectRawP;
            _EnsureValidationIfRequired();
            X.SetDefault();
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TU_Unmanaged<__F_PassedObject2, __F_Allocator2, __F_Config2>>()) << "incompatible value";
            return *this;
        }

        TU_Unmanaged(const TU_Unmanaged&)
        {
            ABYTEK_BASE_OBJECT_ASSERT(false) << "Copy unique pointers is not allowed";
        }
        TU_Unmanaged& operator = (const TU_Unmanaged&)
        {
            ABYTEK_BASE_OBJECT_ASSERT(false) << "Copy unique pointers is not allowed";
            return *this;
        }

    public:
        ABYTEK_FORCE_INLINE B8 operator < (const TU_Unmanaged& X) const noexcept
        {
            return _ObjectRawP < X._ObjectRawP;
        }
        ABYTEK_FORCE_INLINE B8 operator <= (const TU_Unmanaged& X) const noexcept
        {
            return _ObjectRawP <= X._ObjectRawP;
        }
        ABYTEK_FORCE_INLINE B8 operator > (const TU_Unmanaged& X) const noexcept
        {
            return _ObjectRawP > X._ObjectRawP;
        }
        ABYTEK_FORCE_INLINE B8 operator >= (const TU_Unmanaged& X) const noexcept
        {
            return _ObjectRawP >= X._ObjectRawP;
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
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "invalid object, object type: \"" << TypeFullName<__F_PassedObject>() << "\"";
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
        /**
         * @brief Creates a new object without validation checking.
         * 
         * Allocates memory for the object and its header, initializes the key and releaser,
         * and constructs the object with the provided arguments.
         * 
         * @tparam __F_Args Argument types for object constructor
         * @param Args Constructor arguments for the object
         */
        template<typename... __F_Args>
        void CreateObjectUnsafe(__F_Args&&... Args)
        {
            using F_ObjectHeader = TF_UniqueUnmanagedObjectHeader<F_Object>;
            using F_ObjectMemory = TF_UniqueUnmanagedObjectMemory<F_Object>;
            
            using F_ObjectMemoryAllocator = Abytek::TF_RebindAllocator<F_ObjectMemory, F_Allocator>;
            
            F_ObjectMemoryAllocator Allocator;
            F_ObjectMemory* ObjectMemory = (F_ObjectMemory*)Allocator.allocate(1);

            ObjectMemory->ObjectHeader.GetReleaser() = [](void* BaseObjectRawP)
            {
                ((F_Object*)BaseObjectRawP)->~F_Object();
                
                F_ObjectMemoryAllocator Allocator;
                Allocator.deallocate(
                    (F_ObjectMemory*)(
                        ((F_ObjectHeader*)BaseObjectRawP)
                        - 1
                    ),
                    1
                );
            };
            _ObjectRawP = (F_PassedObject*)(ObjectMemory->GetObjectRawP());

            new ((F_Object*)_ObjectRawP) F_Object { ABYTEK_FORWARD(Args)... };
        }
        
        /**
         * @brief Destroys the owned object without validation checking.
         * 
         * Deregisters the object key and calls the releaser function.
         */
        void DestroyObjectUnsafe() noexcept
        {
            GetUnmanagedObjectReleaser(_ObjectRawP)(_ObjectRawP);
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
         * This is the recommended way to create objects with TU_Unmanaged smart pointers.
         * 
         * Usage example:
         * TU_Unmanaged<Type>()(args...)
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
            return _ObjectRawP != nullptr;
        }
        
        /**
         * @brief Slow check if the pointer is null.
         * @return True if the pointer is null, regardless of validation settings
         */
        ABYTEK_FORCE_INLINE B8 NQ_IsNull() const noexcept
        {
            return _ObjectRawP == nullptr;
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
                CallbackIfValid();
                return true;
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
                OutP = typename TF_RebindObject<__F_Object2>::template TF_RebindConfig<typename F_Config::F_NoValidation>::W(CastedObjectRawP);
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
                    (__F_Object2*)_ObjectRawP
                );
            }
            else
            {
                return TF_RebindObject<__F_Object2>::W(
                    TryCastObject<__F_Object2>(_ObjectRawP)
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
            return W(_ObjectRawP);
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
    using TU_ValidUnmanaged = TU_Unmanaged<__F_PassedObject, __F_Allocator, typename __F_Config::F_WithValidation>;

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
        const Abytek::ObjectSmartPointerTemplates::TU_Unmanaged<__F_PassedObject1, __F_Allocator1, __F_Config1>& A,
        const Abytek::ObjectSmartPointerTemplates::TU_Unmanaged<__F_PassedObject2, __F_Allocator2, __F_Config2>& B
    ) noexcept
    {
        return (
            (A.GetObjectRawP() == B.GetObjectRawP())
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
        const Abytek::ObjectSmartPointerTemplates::TU_Unmanaged<__F_PassedObject1, __F_Allocator1, __F_Config1>& A,
        const Abytek::ObjectSmartPointerTemplates::TU_Unmanaged<__F_PassedObject2, __F_Allocator2, __F_Config2>& B
    ) noexcept
    {
        return (
            (A.GetObjectRawP() != B.GetObjectRawP())
        );
    }
}