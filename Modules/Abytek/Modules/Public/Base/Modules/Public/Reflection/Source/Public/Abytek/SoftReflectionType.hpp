#pragma once

#include "Abytek/ReflectionCommon.hpp"
#include "Abytek/ReflectionType.hpp"


namespace Abytek
{
    template<typename __F_Type>
    struct TF_SoftReflectionTypeHandle
    {
    public:
        using F_Type = __F_Type;
        using F_Hard = TF_ReflectionTypeHandle<F_Type>;
        
    private:
        F_Name _Key;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetKey() const noexcept
        {
            return _Key;
        }
    
    public:
        TF_SoftReflectionTypeHandle(const F_Name& Key) :
            _Key(Key)
        {
        }
        TF_SoftReflectionTypeHandle() = default;
        TF_SoftReflectionTypeHandle(const TF_SoftReflectionTypeHandle&) = default;
        TF_SoftReflectionTypeHandle& operator = (const TF_SoftReflectionTypeHandle&) = default;
        TF_SoftReflectionTypeHandle(TF_SoftReflectionTypeHandle&&) = default;
        TF_SoftReflectionTypeHandle& operator = (TF_SoftReflectionTypeHandle&&) = default;
        
    public:
        B8 IsValid() const noexcept
        {
            if (!_Key)
            {
                return false;
            }
            
            auto Item = GlobalSearchReflectionItem(_Key);
            if (!Item)
            {
                return false;
            }
            
            TW<F_ReflectionType> Type;
            if (!Item.TryDynamicCast<F_ReflectionType>(Type))
            {
                return false;
            }
            
            return F_Hard::CheckType(Type);
        }
        ABYTEK_FORCE_INLINE explicit operator B8() const noexcept
        {
            return IsValid();
        }
        ABYTEK_FORCE_INLINE B8 IsNone() const noexcept
        {
            return static_cast<B8>(_Key);
        }
        static TF_SoftReflectionTypeHandle MakeNone() noexcept
        {
            return {};
        }
        
    public:
        F_ReflectionType& operator * () const noexcept
        {
            ABYTEK_BASE_REFLECTION_ASSERT(IsValid()) << "Cannot access invalid soft reflection type handle";
            return *(GlobalSearchReflectionItem(_Key).FastCast<F_ReflectionType>());
        }
        F_ReflectionType* operator -> () const noexcept
        {
            ABYTEK_BASE_REFLECTION_ASSERT(IsValid()) << "Cannot access invalid soft reflection type handle";
            return GlobalSearchReflectionItem(_Key).FastCast<F_ReflectionType>().GetObjectRawP();
        }
        
    public:
        template<
            typename __F_Type2 
            ABYTEK_REQUIRES(
                std::is_convertible_v<
                    std::remove_reference_t<__F_Type>*, 
                    std::remove_reference_t<__F_Type2>*
                >
            )
        >
        operator TF_ReflectionTypeHandle<__F_Type2>() const noexcept
        {
            return GlobalSearchReflectionItem(_Key).FastCast<F_ReflectionType>();
        }
        
    public:
        ABYTEK_FORCE_INLINE B8 operator == (const TF_SoftReflectionTypeHandle& X) const noexcept
        {
            return (_Key == X._Key);
        }
        ABYTEK_FORCE_INLINE B8 operator != (const TF_SoftReflectionTypeHandle& X) const noexcept
        {
            return (_Key != X._Key);
        }
    };
}