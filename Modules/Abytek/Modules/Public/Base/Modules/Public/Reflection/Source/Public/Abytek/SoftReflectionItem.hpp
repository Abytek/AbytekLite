#pragma once

#include "Abytek/ReflectionCommon.hpp"


namespace Abytek
{
    struct F_SoftReflectionItem
    {
    private:
        F_Name _Key;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetKey() const noexcept
        {
            return _Key;
        }
    
    public:
        F_SoftReflectionItem(const F_Name& Key) :
            _Key(Key)
        {
        }
        F_SoftReflectionItem() = default;
        F_SoftReflectionItem(const F_SoftReflectionItem&) = default;
        F_SoftReflectionItem& operator = (const F_SoftReflectionItem&) = default;
        F_SoftReflectionItem(F_SoftReflectionItem&&) = default;
        F_SoftReflectionItem& operator = (F_SoftReflectionItem&&) = default;
        
    public:
        B8 IsValid() const noexcept
        {
            if (!_Key)
            {
                return false;
            }
            return static_cast<B8>(GlobalSearchReflectionItem(_Key));
        }
        ABYTEK_FORCE_INLINE explicit operator B8() const noexcept
        {
            return IsValid();
        }
        ABYTEK_FORCE_INLINE B8 IsNone() const noexcept
        {
            return static_cast<B8>(_Key);
        }
        static F_SoftReflectionItem MakeNone() noexcept
        {
            return {};
        }
        
    public:
        A_ReflectionItem& operator * () const noexcept
        {
            return *GlobalSearchReflectionItem(_Key);
        }
        A_ReflectionItem* operator -> () const noexcept
        {
            return GlobalSearchReflectionItem(_Key).GetObjectRawP();
        }
        
    public:
        ABYTEK_FORCE_INLINE B8 operator == (const F_SoftReflectionItem& X) const noexcept
        {
            return (_Key == X._Key);
        }
        ABYTEK_FORCE_INLINE B8 operator != (const F_SoftReflectionItem& X) const noexcept
        {
            return (_Key != X._Key);
        }
    };
}