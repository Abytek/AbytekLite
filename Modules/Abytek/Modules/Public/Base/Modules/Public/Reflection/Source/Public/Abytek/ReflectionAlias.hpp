#pragma once

#include "Abytek/ReflectionItem.hpp"


namespace Abytek
{
    struct F_ReflectionAliasBuildParams : F_ReflectionItemBuildParams
    {
        F_Name Key;
        F_Name Value;
        
        static F_ReflectionAliasBuildParams Make(
            const TW_Valid<F_ReflectionContext>& InContext, 
            const F_Name& InKey, 
            const F_Name& InValue
        )
        {
            F_ReflectionAliasBuildParams Result;
            Result.Context = InContext;
            Result.Key = InKey;
            Result.Value = InValue;
            return Result;
        }
    };
    class ABYTEK_BASE_REFLECTION_API F_ReflectionAlias : public A_ReflectionItem
    {
    private:
        F_Name _Key;
        F_Name _Value;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetKey() const noexcept
        {
            return _Key;
        }
        ABYTEK_FORCE_INLINE const auto& GetValue() const noexcept
        {
            return _Value;
        }
        
    public:
        F_ReflectionAlias(const F_GBMObjectConfig& GBMConfig, const F_ReflectionAliasBuildParams& BuildParams);
        virtual ~F_ReflectionAlias() override;
    };
}