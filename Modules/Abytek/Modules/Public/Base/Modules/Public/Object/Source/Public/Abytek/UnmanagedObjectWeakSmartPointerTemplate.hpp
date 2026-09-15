#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/ObjectBase.hpp"
#include "Abytek/ObjectHeader.hpp"
#include "Abytek/ObjectMemory.hpp"


namespace Abytek::ObjectSmartPointerTemplates
{    
    template<typename __F_PassedObject, class __F_Config>
    class TW_Unmanaged final
    {
    public:
        template<typename __F_PassedObject2, class __F_Config2>
        friend class TW_Unmanaged;
        
    public:
        using F_PassedObject = __F_PassedObject;
        using F_Config = __F_Config;

    public:
        using F_Object = std::remove_const_t<std::remove_reference_t<F_PassedObject>>;

    public:
        template<typename __F_PassedObject2>
        using TF_RebindObject = TW_Unmanaged<__F_PassedObject2, F_Config>;
        template<class __F_Config2>
        using TF_RebindConfig = TW_Unmanaged<F_PassedObject, __F_Config2>;

    public:
        using F_WithValidation = TF_RebindConfig<typename F_Config::F_WithValidation>;
        using F_NoValidation = TF_RebindConfig<typename F_Config::F_NoValidation>;

    private:
        using F_This = TW_Unmanaged<F_PassedObject, F_Config>;
        
    private:
        F_PassedObject* _ObjectRawP;

    public:
        /**
         * @brief Gets the raw pointer to the referenced object.
         * @return Raw pointer to the object
         */
        ABYTEK_FORCE_INLINE F_PassedObject* GetObjectRawP() const noexcept
        {
            return _ObjectRawP;
        }

    public:
        /**
         * @brief Default constructor, creates a null reference.
         */
        ABYTEK_FORCE_INLINE constexpr TW_Unmanaged() noexcept
        {
            SetDefault();
        }
        
        /**
         * @brief Constructor from raw pointer and object key.
         * @param ObjectRawP Raw pointer to the object
         */
        ABYTEK_FORCE_INLINE explicit TW_Unmanaged(F_PassedObject* ObjectRawP) noexcept :
            _ObjectRawP(ObjectRawP)
        {
            _EnsureValidationIfRequired();
        }
        
        /**
         * @brief Destructor.
         */
        ABYTEK_FORCE_INLINE ~TW_Unmanaged() noexcept
        {
            ResetUnsafe();
        }

        template<
            typename __F_PassedObject2, class __F_Config2
            ABYTEK_REQUIRES(IsObjectDownCastable<__F_PassedObject2, __F_PassedObject>)
        >
        /**
         * @brief Copy constructor from compatible weak pointer.
         * @tparam __F_PassedObject2 Source object type (must be downcastable to this pointer's type)
         * @tparam __F_Config2 Source configuration type
         * @param X Source weak pointer to copy from
         */
        ABYTEK_FORCE_INLINE TW_Unmanaged(const TW_Unmanaged<__F_PassedObject2, __F_Config2>& X) noexcept :
            _ObjectRawP(X._ObjectRawP)
        {
            _EnsureValidationIfRequired();
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TW_Unmanaged<__F_PassedObject2, __F_Config2>>()) << "incompatible value";
        }
        
        template<
            typename __F_PassedObject2, class __F_Config2
            ABYTEK_REQUIRES(IsObjectDownCastable<__F_PassedObject2, __F_PassedObject>)
        >
        /**
         * @brief Copy assignment operator from compatible weak pointer.
         * @tparam __F_PassedObject2 Source object type (must be downcastable to this pointer's type)
         * @tparam __F_Config2 Source configuration type
         * @param X Source weak pointer to copy from
         * @return Reference to this pointer
         */
        ABYTEK_FORCE_INLINE TW_Unmanaged& operator = (const TW_Unmanaged<__F_PassedObject2, __F_Config2>& X) noexcept
        {
            _ObjectRawP = X._ObjectRawP;
            _EnsureValidationIfRequired();
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TW_Unmanaged<__F_PassedObject2, __F_Config2>>()) << "incompatible value";
            return *this;
        }

        template<
            typename __F_PassedObject2, class __F_Config2
            ABYTEK_REQUIRES(IsObjectDownCastable<__F_PassedObject2, __F_PassedObject>)
        >
        /**
         * @brief Move constructor from compatible weak pointer.
         * @tparam __F_PassedObject2 Source object type (must be downcastable to this pointer's type)
         * @tparam __F_Config2 Source configuration type
         * @param X Source weak pointer to move from
         */
        ABYTEK_FORCE_INLINE TW_Unmanaged(TW_Unmanaged<__F_PassedObject2, __F_Config2>&& X) noexcept :
            _ObjectRawP(X._ObjectRawP)
        {
            _EnsureValidationIfRequired();
            if constexpr (!__F_Config2::EnableValidation)
            {
                X.SetDefault();
            }
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TW_Unmanaged<__F_PassedObject2, __F_Config2>>()) << "incompatible value";
        }
        
        template<
            typename __F_PassedObject2, class __F_Config2
            ABYTEK_REQUIRES(IsObjectDownCastable<__F_PassedObject2, __F_PassedObject>)
        >
        /**
         * @brief Move assignment operator from compatible weak pointer.
         * @tparam __F_PassedObject2 Source object type (must be downcastable to this pointer's type)
         * @tparam __F_Config2 Source configuration type
         * @param X Source weak pointer to move from
         * @return Reference to this pointer
         */
        ABYTEK_FORCE_INLINE TW_Unmanaged& operator = (TW_Unmanaged<__F_PassedObject2, __F_Config2>&& X) noexcept
        {
            _ObjectRawP = X._ObjectRawP;
            _EnsureValidationIfRequired();
            if constexpr (!__F_Config2::EnableValidation)
            {
                X.SetDefault();
            }
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TW_Unmanaged<__F_PassedObject2, __F_Config2>>()) << "incompatible value";
            return *this;
        }

    public:
        ABYTEK_FORCE_INLINE B8 operator < (const TW_Unmanaged& X) const noexcept
        {
            return _ObjectRawP < X._ObjectRawP;
        }
        ABYTEK_FORCE_INLINE B8 operator <= (const TW_Unmanaged& X) const noexcept
        {
            return _ObjectRawP <= X._ObjectRawP;
        }
        ABYTEK_FORCE_INLINE B8 operator > (const TW_Unmanaged& X) const noexcept
        {
            return _ObjectRawP > X._ObjectRawP;
        }
        ABYTEK_FORCE_INLINE B8 operator >= (const TW_Unmanaged& X) const noexcept
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
         * @brief Resets the pointer to default/null state without validation.
         */
        ABYTEK_FORCE_INLINE void ResetUnsafe()
        {
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
         * @brief Resets the pointer to default/null state with validation if required.
         */
        ABYTEK_FORCE_INLINE void Reset()
        {
            ResetUnsafe();
            _EnsureValidationIfRequired();
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

			return (_ObjectRawP != 0);
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

            return (_ObjectRawP == 0);
        }
        
        /**
         * @brief Slow check if the pointer is valid.
         * @return True if the pointer is valid, regardless of validation settings
         */
        ABYTEK_FORCE_INLINE B8 NQ_IsValid() const noexcept
        {
			return (_ObjectRawP != 0);
        }
        
        /**
         * @brief Slow check if the pointer is null.
         * @return True if the pointer is null, regardless of validation settings
         */
        ABYTEK_FORCE_INLINE B8 NQ_IsNull() const noexcept
        {
			return (_ObjectRawP == 0);
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
         * @return Pointer to the referenced object
         */
        ABYTEK_FORCE_INLINE F_PassedObject* operator -> () const noexcept
        {        
            _EnsureValidationIfRequired();
            ABYTEK_BASE_OBJECT_ASSERT(*this) << "cannot access invalid objects";

            return _ObjectRawP;
        }
        
        /**
         * @brief Dereference operator for object access.
         * @return Reference to the referenced object
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
            
            return CheckObjectInterfaceForStaticCasting<__F_Object2>(_ObjectRawP);
        }
        
        /**
         * @brief Attempts to dynamically cast the object and store the result.
         * @tparam __F_Object2 Target type to cast to
         * @param OutP Output pointer to store the result if cast succeeds
         * @return True if the cast succeeded
         */
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE B8 TryDynamicCast(typename TF_RebindObject<__F_Object2>::template TF_RebindConfig<typename F_Config::F_NoValidation>& OutP) const noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot try to dynamic cast on an invalid object smart pointer";
            
            __F_Object2* CastedObjectRawP = TryCastObject<__F_Object2>(_ObjectRawP);
            if(CastedObjectRawP)
            {
                OutP = typename TF_RebindObject<__F_Object2>::template TF_RebindConfig<typename F_Config::F_NoValidation>(CastedObjectRawP);
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
         * @brief Performs a dynamic cast to another type.
         * @tparam __F_Object2 Target type to cast to
         * @return New pointer of the target type
         */
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE TF_RebindObject<__F_Object2> DynamicCast() const noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot dynamic cast on an invalid object smart pointer";
            ABYTEK_BASE_OBJECT_ASSERT(CheckObjectInterface<__F_Object2>(_ObjectRawP)) << "invalid object polymorphism";

            if constexpr (std::is_same_v<std::remove_const_t<__F_Object2>, F_Object>)
            {
                return TF_RebindObject<__F_Object2>(
                    (__F_Object2*)_ObjectRawP
                );
            }
            else
            {
                return TF_RebindObject<__F_Object2>(
                    TryCastObject<__F_Object2>(_ObjectRawP)
                );
            }
        }
        
        template<typename __F_Object2>
        ABYTEK_FORCE_INLINE TF_RebindObject<__F_Object2> StaticCast() const noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "cannot static cast on an invalid object smart pointer of type: " << TypeFullName<F_Object>();
            return *this;
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
        ABYTEK_FORCE_INLINE TW_Unmanaged Weak() const noexcept
        {
            return *this;
        }

    public:    public:
        /**
         * @brief Returns a pointer with validation enabled (rvalue).
         * @return Rvalue reference to pointer with validation
         */
        ABYTEK_FORCE_INLINE auto&& WithValidation() && noexcept
        {        
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "invalid object, object type: \"" << TypeFullName<__F_PassedObject>() << "\"";
            using F_Target = TF_RebindConfig<typename F_Config::F_WithValidation>;
            return (F_Target&&)*((F_Target*)this);
        }
        
        /**
         * @brief Returns a pointer with validation enabled (const rvalue).
         * @return Const rvalue reference to pointer with validation
         */
        ABYTEK_FORCE_INLINE auto&& WithValidation() const && noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "invalid object, object type: \"" << TypeFullName<__F_PassedObject>() << "\"";
            using F_Target = TF_RebindConfig<typename F_Config::F_WithValidation> const;
            return (F_Target&&)*((F_Target*)this);
        }
        
        /**
         * @brief Returns a pointer with validation enabled (lvalue).
         * @return Reference to pointer with validation
         */
        ABYTEK_FORCE_INLINE auto& WithValidation() & noexcept
        {          
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "invalid object, object type: \"" << TypeFullName<__F_PassedObject>() << "\"";
            using F_Target = TF_RebindConfig<typename F_Config::F_WithValidation>;
            return (F_Target&)*((F_Target*)this);
        }
        
        /**
         * @brief Returns a pointer with validation enabled (const lvalue).
         * @return Const reference to pointer with validation
         */
        ABYTEK_FORCE_INLINE auto& WithValidation() const & noexcept
        {        
            ABYTEK_BASE_OBJECT_ASSERT(NQ_IsValid()) << "invalid object, object type: \"" << TypeFullName<__F_PassedObject>() << "\"";
            using F_Target = TF_RebindConfig<typename F_Config::F_WithValidation> const;
            return (F_Target&)*((F_Target*)this);
        }
        
        /**
         * @brief Returns a pointer with validation disabled (rvalue).
         * @return Rvalue reference to pointer without validation
         */
        ABYTEK_FORCE_INLINE auto&& NoValidation() && noexcept
        {
            using F_Target = TF_RebindConfig<typename F_Config::F_NoValidation>;
            return (F_Target&&)*((F_Target*)this);
        }
        
        /**
         * @brief Returns a pointer with validation disabled (const rvalue).
         * @return Const rvalue reference to pointer without validation
         */
        ABYTEK_FORCE_INLINE auto&& NoValidation() const && noexcept
        {
            using F_Target = TF_RebindConfig<typename F_Config::F_NoValidation> const;
            return (F_Target&&)*((F_Target*)this);
        }
        
        /**
         * @brief Returns a pointer with validation disabled (lvalue).
         * @return Reference to pointer without validation
         */
        ABYTEK_FORCE_INLINE auto& NoValidation() & noexcept
        {
            using F_Target = TF_RebindConfig<typename F_Config::F_NoValidation>;
            return (F_Target&)*((F_Target*)this);
        }
        
        /**
         * @brief Returns a pointer with validation disabled (const lvalue).
         * @return Const reference to pointer without validation
         */
        ABYTEK_FORCE_INLINE auto& NoValidation() const & noexcept
        {
            using F_Target = TF_RebindConfig<typename F_Config::F_NoValidation> const;
            return (F_Target&)*((F_Target*)this);
        }
    };

    /**
     * @brief Type alias for a weak pointer with validation enabled.
     * 
     * @tparam __F_PassedObject The object type this pointer references
     * @tparam __F_Config Pointer configuration settings
     */
    template<typename __F_PassedObject, class __F_Config>
    using TW_ValidUnmanaged = TW_Unmanaged<__F_PassedObject, typename __F_Config::F_WithValidation>;
    
    /**
     * @brief Creates a weak pointer to the current object.
     * 
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Pointer configuration settings
     * @param ObjectRawP Raw pointer to the object (typically 'this')
     * @return Weak pointer with validation to the object
     */
    template<typename __F_PassedObject, class __F_Config>
    typename TW_Unmanaged<__F_PassedObject, __F_Config>::F_WithValidation GetWThisUnmanaged(__F_PassedObject* ObjectRawP) noexcept 
    {
        return typename TW_Unmanaged<__F_PassedObject, __F_Config>::F_WithValidation((__F_PassedObject*)ObjectRawP);
    }
    
    /**
     * @brief Creates a mutable weak pointer to the current object.
     * 
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Pointer configuration settings
     * @param ObjectRawP Raw pointer to the object (typically 'this')
     * @return Weak pointer with validation to a mutable version of the object
     */
    template<typename __F_PassedObject, class __F_Config>
    typename TW_Unmanaged<std::remove_const_t<__F_PassedObject>, __F_Config>::F_WithValidation GetWThisMutableUnmanaged(__F_PassedObject* ObjectRawP) noexcept 
    {
        return typename TW_Unmanaged<std::remove_const_t<__F_PassedObject>, __F_Config>::F_WithValidation((std::remove_const_t<__F_PassedObject>*)ObjectRawP);
    }
    
    /**
     * @brief Creates a weak pointer to the current object without validation.
     * 
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Pointer configuration settings
     * @param ObjectRawP Raw pointer to the object (typically 'this')
     * @return Weak pointer without validation to the object
     */
    template<typename __F_PassedObject, class __F_Config>
    typename TW_Unmanaged<__F_PassedObject, __F_Config>::F_NoValidation GetWThisUnmanaged_NoValidation(__F_PassedObject* ObjectRawP) noexcept 
    {
        return typename TW_Unmanaged<__F_PassedObject, __F_Config>::F_NoValidation((__F_PassedObject*)ObjectRawP);
    }
    
    /**
     * @brief Creates a mutable weak pointer to the current object without validation.
     * 
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Pointer configuration settings
     * @param ObjectRawP Raw pointer to the object (typically 'this')
     * @return Weak pointer without validation to a mutable version of the object
     */
    template<typename __F_PassedObject, class __F_Config>
    typename TW_Unmanaged<std::remove_const_t<__F_PassedObject>, __F_Config>::F_NoValidation GetWThisMutableUnmanaged_NoValidation(__F_PassedObject* ObjectRawP) noexcept 
    {
        return typename TW_Unmanaged<std::remove_const_t<__F_PassedObject>, __F_Config>::F_NoValidation((std::remove_const_t<__F_PassedObject>*)ObjectRawP);
    }
    
    /**
     * @brief Equality comparison operator for weak pointers.
     * 
     * @tparam __F_PassedObject1 First pointer's object type
     * @tparam __F_Config1 First pointer's configuration
     * @tparam __F_PassedObject2 Second pointer's object type
     * @tparam __F_Config2 Second pointer's configuration
     * @param A First weak pointer
     * @param B Second weak pointer
     * @return True if both pointers reference the same object
     */
    template<typename __F_PassedObject1, class __F_Config1, typename __F_PassedObject2, class __F_Config2>
    ABYTEK_FORCE_INLINE bool operator == (
        const Abytek::ObjectSmartPointerTemplates::TW_Unmanaged<__F_PassedObject1, __F_Config1>& A,
        const Abytek::ObjectSmartPointerTemplates::TW_Unmanaged<__F_PassedObject2, __F_Config2>& B
    ) noexcept
    {
        return (
            (A.GetObjectRawP() == B.GetObjectRawP())
        );
    }
    
    /**
     * @brief Inequality comparison operator for weak pointers.
     * 
     * @tparam __F_PassedObject1 First pointer's object type
     * @tparam __F_Config1 First pointer's configuration
     * @tparam __F_PassedObject2 Second pointer's object type
     * @tparam __F_Config2 Second pointer's configuration
     * @param A First weak pointer
     * @param B Second weak pointer
     * @return True if the pointers reference different objects
     */
    template<typename __F_PassedObject1, class __F_Config1, typename __F_PassedObject2, class __F_Config2>
    ABYTEK_FORCE_INLINE bool operator != (
        const Abytek::ObjectSmartPointerTemplates::TW_Unmanaged<__F_PassedObject1, __F_Config1>& A,
        const Abytek::ObjectSmartPointerTemplates::TW_Unmanaged<__F_PassedObject2, __F_Config2>& B
    ) noexcept
    {
        return (
            (A.GetObjectRawP() != B.GetObjectRawP())
        );
    }
}


/**
 * @brief Macro to get a weak pointer to the current object with validation.
 * @param ... Optional template parameters
 */
#define ABYTEK_WTHIS_UNMANAGED(...) __VA_ARGS__ GetWThisUnmanaged(this) 

/**
 * @brief Macro to get a mutable weak pointer to the current object with validation.
 * @param ... Optional template parameters
 */
#define ABYTEK_WTHIS_MUTABLE_UNMANAGED(...) __VA_ARGS__ GetWThisMutableUnmanaged(this) 

/**
 * @brief Macro to get a weak pointer to the current object without validation.
 * @param ... Optional template parameters
 */
#define ABYTEK_WTHIS_UNMANAGED_NO_VALIDATION(...) __VA_ARGS__ GetWThisUnmanaged_NoValidation(this) 

/**
 * @brief Macro to get a mutable weak pointer to the current object without validation.
 * @param ... Optional template parameters
 */
#define ABYTEK_WTHIS_MUTABLE_UNMANAGED_NO_VALIDATION(...) __VA_ARGS__ GetWThisMutableUnmanaged_NoValidation(this)