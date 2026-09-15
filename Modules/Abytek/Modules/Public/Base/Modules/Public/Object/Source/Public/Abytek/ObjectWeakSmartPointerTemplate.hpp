/**
 * @file ObjectWeakSmartPointerTemplate.hpp
 * @brief Implements weak (non-owning) smart pointer functionality.
 * 
 * This file contains the implementation of TW (Template Weak), a non-owning smart
 * pointer type similar to std::weak_ptr. Weak pointers provide access to objects
 * owned by other pointer types without affecting their lifetime.
 */
#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/ObjectBase.hpp"
#include "Abytek/ObjectHeader.hpp"
#include "Abytek/ObjectMemory.hpp"
#include "Abytek/UnmanagedObjectWeakSmartPointerTemplate.hpp"


namespace Abytek::ObjectSmartPointerTemplates
{    
    /**
     * @brief Weak (non-owning) smart pointer template.
     * 
     * TW provides a non-owning reference to an object that may be managed by
     * unique or shared pointers. It doesn't affect the object's lifetime but
     * allows checking if the object is still valid and accessing it when it is.
     * 
     * Features:
     * - Object validity checking
     * - Type-safe casting (static and dynamic)
     * - Configuration switching (validation modes)
     * 
     * @tparam __F_PassedObject The object type this pointer references
     * @tparam __F_Config Pointer configuration settings
     */
    template<typename __F_PassedObject, class __F_Config>
    class TW final
    {
    public:
        template<typename __F_PassedObject2, class __F_Config2>
        friend class TW;
        
    public:
        using F_PassedObject = __F_PassedObject;
        using F_Config = __F_Config;

    public:
        using F_Object = std::remove_const_t<std::remove_reference_t<F_PassedObject>>;

    public:
        template<typename __F_PassedObject2>
        using TF_RebindObject = TW<__F_PassedObject2, F_Config>;
        template<class __F_Config2>
        using TF_RebindConfig = TW<F_PassedObject, __F_Config2>;

    public:
        using F_WithValidation = TF_RebindConfig<typename F_Config::F_WithValidation>;
        using F_NoValidation = TF_RebindConfig<typename F_Config::F_NoValidation>;

    public:
        using W_Unmanaged = TW_Unmanaged<F_PassedObject, F_Config>;
        
    private:
        using F_ObjectKey = typename F_Config::F_ObjectManagement::F_ObjectKey;
        using F_ObjectKeyPool = typename F_Config::F_ObjectManagement::F_ObjectKeyPool;
        using E_ObjectTrackingFlag = typename F_Config::F_ObjectManagement::E_ObjectTrackingFlag;

    private:
        using F_This = TW<F_PassedObject, F_Config>;
        
    private:
        F_PassedObject* _ObjectRawP;
        F_ObjectKey _ObjectKey;

    public:
        /**
         * @brief Gets the raw pointer to the referenced object.
         * @return Raw pointer to the object
         */
        ABYTEK_FORCE_INLINE F_PassedObject* GetObjectRawP() const noexcept
        {
            return _ObjectRawP;
        }
        
        /**
         * @brief Gets the object key for the referenced object.
         * @return Object key value
         */
        ABYTEK_FORCE_INLINE F_ObjectKey GetObjectKey() const noexcept
        {
            return _ObjectKey;
        }

    public:
        /**
         * @brief Default constructor, creates a null reference.
         */
        ABYTEK_FORCE_INLINE constexpr TW() noexcept
        {
            SetDefault();
        }
        
        /**
         * @brief Constructor from raw pointer and object key.
         * @param ObjectRawP Raw pointer to the object
         * @param ObjectKey Object key for validation
         */
        ABYTEK_FORCE_INLINE explicit TW(F_PassedObject* ObjectRawP, F_ObjectKey ObjectKey) noexcept :
            _ObjectRawP(ObjectRawP),
            _ObjectKey(ObjectKey)
        {
            _EnsureValidationIfRequired();
        }
        
        /**
         * @brief Constructor from raw pointer.
         * @param ObjectRawP Raw pointer to the object
         */
        ABYTEK_FORCE_INLINE explicit TW(F_PassedObject* ObjectRawP) noexcept :
            _ObjectRawP(ObjectRawP)
        {
            static constexpr auto IsInterfaceOnly = !std::is_convertible_v<F_PassedObject*, const A_Object*>;
            if constexpr (IsInterfaceOnly)
            {
                _ObjectKey = Abytek::GetObjectKey<typename F_Config::F_ObjectManagement>(
                    (void*)dynamic_cast<const A_Object*>(ObjectRawP)
                );
            }
            else
            {
                _ObjectKey = Abytek::GetObjectKey<typename F_Config::F_ObjectManagement>((void*)ObjectRawP);
            }
            _EnsureValidationIfRequired();
        }
        
        /**
         * @brief Destructor.
         */
        ABYTEK_FORCE_INLINE ~TW() noexcept
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
        ABYTEK_FORCE_INLINE TW(const TW<__F_PassedObject2, __F_Config2>& X) noexcept :
            _ObjectRawP(X._ObjectRawP),
            _ObjectKey(X._ObjectKey)
        {
            _EnsureValidationIfRequired();
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TW<__F_PassedObject2, __F_Config2>>()) << "incompatible value";
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
        ABYTEK_FORCE_INLINE TW& operator = (const TW<__F_PassedObject2, __F_Config2>& X) noexcept
        {
            _ObjectRawP = X._ObjectRawP;
            _ObjectKey = X._ObjectKey;
            _EnsureValidationIfRequired();
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TW<__F_PassedObject2, __F_Config2>>()) << "incompatible value";
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
        ABYTEK_FORCE_INLINE TW(TW<__F_PassedObject2, __F_Config2>&& X) noexcept :
            _ObjectRawP(X._ObjectRawP),
            _ObjectKey(X._ObjectKey)
        {
            _EnsureValidationIfRequired();
            if constexpr (!__F_Config2::EnableValidation)
            {
                X.SetDefault();
            }
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TW<__F_PassedObject2, __F_Config2>>()) << "incompatible value";
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
        ABYTEK_FORCE_INLINE TW& operator = (TW<__F_PassedObject2, __F_Config2>&& X) noexcept
        {
            _ObjectRawP = X._ObjectRawP;
            _ObjectKey = X._ObjectKey;
            _EnsureValidationIfRequired();
            if constexpr (!__F_Config2::EnableValidation)
            {
                X.SetDefault();
            }
            ABYTEK_BASE_CORE_ASSERT(F_Config::F_Extension::template CustomIsCompatible<F_This, TW<__F_PassedObject2, __F_Config2>>()) << "incompatible value";
            return *this;
        }

    public:
        /**
         * @brief Less than comparison operator.
         * @param X Pointer to compare against
         * @return True if this pointer's key is less than X's key
         */
        ABYTEK_FORCE_INLINE B8 operator < (const TW& X) const noexcept
        {
            return _ObjectKey.Value < X._ObjectKey.Value;
        }
        
        /**
         * @brief Less than or equal comparison operator.
         * @param X Pointer to compare against
         * @return True if this pointer's key is less than or equal to X's key
         */
        ABYTEK_FORCE_INLINE B8 operator <= (const TW& X) const noexcept
        {
            return _ObjectKey.Value <= X._ObjectKey.Value;
        }
        
        /**
         * @brief Greater than comparison operator.
         * @param X Pointer to compare against
         * @return True if this pointer's key is greater than X's key
         */
        ABYTEK_FORCE_INLINE B8 operator > (const TW& X) const noexcept
        {
            return _ObjectKey.Value > X._ObjectKey.Value;
        }
        
        /**
         * @brief Greater than or equal comparison operator.
         * @param X Pointer to compare against
         * @return True if this pointer's key is greater than or equal to X's key
         */
        ABYTEK_FORCE_INLINE B8 operator >= (const TW& X) const noexcept
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
                << "\", required flags: "
                << static_cast<U64>(E_ObjectTrackingFlag::NONE)
                << ", current flags: "
                << static_cast<U64>(Flags);
            ABYTEK_BASE_OBJECT_ASSERT(!FlagHasAny(Flags, E_ObjectTrackingFlag::INVALID))
                << "invalid object, object type: \""
                << TypeFullName<__F_PassedObject>()
                << "\", forbidden flags: "
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

			if(_ObjectRawP)
			{
				return F_ObjectKeyPool::GetInstance().Check(_ObjectKey);
			}
			return false;
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

			if(_ObjectRawP)
			{
				return !F_ObjectKeyPool::GetInstance().Check(_ObjectKey);
			}
			return true;
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
                OutP = typename TF_RebindObject<__F_Object2>::template TF_RebindConfig<typename F_Config::F_NoValidation>(CastedObjectRawP, _ObjectKey);
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
                    (__F_Object2*)_ObjectRawP,
                    _ObjectKey
                );
            }
            else
            {
                return TF_RebindObject<__F_Object2>(
                    TryCastObject<__F_Object2>(_ObjectRawP),
                    _ObjectKey
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
        ABYTEK_FORCE_INLINE TW Weak() const noexcept
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

    public:
        ABYTEK_FORCE_INLINE W_Unmanaged ToUnmanaged() const noexcept
        {
            return W_Unmanaged(_ObjectRawP);
        }
    };

    /**
     * @brief Type alias for a weak pointer with validation enabled.
     * 
     * @tparam __F_PassedObject The object type this pointer references
     * @tparam __F_Config Pointer configuration settings
     */
    template<typename __F_PassedObject, class __F_Config>
    using TW_Valid = TW<__F_PassedObject, typename __F_Config::F_WithValidation>;
    
    /**
     * @brief Creates a weak pointer to the current object.
     * 
     * @tparam __F_PassedObject The object type
     * @tparam __F_Config Pointer configuration settings
     * @param ObjectRawP Raw pointer to the object (typically 'this')
     * @return Weak pointer with validation to the object
     */
    template<typename __F_PassedObject, class __F_Config>
    typename TW<__F_PassedObject, __F_Config>::F_WithValidation GetWThis(__F_PassedObject* ObjectRawP) noexcept 
    {
        return typename TW<__F_PassedObject, __F_Config>::F_WithValidation((__F_PassedObject*)ObjectRawP);
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
    typename TW<std::remove_const_t<__F_PassedObject>, __F_Config>::F_WithValidation GetWThisMutable(__F_PassedObject* ObjectRawP) noexcept 
    {
        return typename TW<std::remove_const_t<__F_PassedObject>, __F_Config>::F_WithValidation((std::remove_const_t<__F_PassedObject>*)ObjectRawP);
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
    typename TW<__F_PassedObject, __F_Config>::F_NoValidation GetWThis_NoValidation(__F_PassedObject* ObjectRawP) noexcept 
    {
        return typename TW<__F_PassedObject, __F_Config>::F_NoValidation((__F_PassedObject*)ObjectRawP);
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
    typename TW<std::remove_const_t<__F_PassedObject>, __F_Config>::F_NoValidation GetWThisMutable_NoValidation(__F_PassedObject* ObjectRawP) noexcept 
    {
        return typename TW<std::remove_const_t<__F_PassedObject>, __F_Config>::F_NoValidation((std::remove_const_t<__F_PassedObject>*)ObjectRawP);
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
        const Abytek::ObjectSmartPointerTemplates::TW<__F_PassedObject1, __F_Config1>& A,
        const Abytek::ObjectSmartPointerTemplates::TW<__F_PassedObject2, __F_Config2>& B
    ) noexcept
    {
        return (
            (A.GetObjectRawP() == B.GetObjectRawP())
            && (A.GetObjectKey() == B.GetObjectKey())
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
        const Abytek::ObjectSmartPointerTemplates::TW<__F_PassedObject1, __F_Config1>& A,
        const Abytek::ObjectSmartPointerTemplates::TW<__F_PassedObject2, __F_Config2>& B
    ) noexcept
    {
        return (
            (A.GetObjectRawP() != B.GetObjectRawP())
            || (A.GetObjectKey() != B.GetObjectKey())
        );
    }
}


/**
 * @brief Macro to get a weak pointer to the current object with validation.
 * @param ... Optional template parameters
 */
#define ABYTEK_WTHIS(...) __VA_ARGS__ GetWThis(this) 

/**
 * @brief Macro to get a mutable weak pointer to the current object with validation.
 * @param ... Optional template parameters
 */
#define ABYTEK_WTHIS_MUTABLE(...) __VA_ARGS__ GetWThisMutable(this) 

/**
 * @brief Macro to get a weak pointer to the current object without validation.
 * @param ... Optional template parameters
 */
#define ABYTEK_WTHIS_NO_VALIDATION(...) __VA_ARGS__ GetWThis_NoValidation(this) 

/**
 * @brief Macro to get a mutable weak pointer to the current object without validation.
 * @param ... Optional template parameters
 */
#define ABYTEK_WTHIS_MUTABLE_NO_VALIDATION(...) __VA_ARGS__ GetWThisMutable_NoValidation(this)