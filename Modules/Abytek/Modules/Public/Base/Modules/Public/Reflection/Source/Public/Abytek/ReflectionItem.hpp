#pragma once

#include "Abytek/ReflectionCommon.hpp"


namespace Abytek
{
    class F_ReflectionContext;
    
    struct F_ReflectionItemBuildParams
    {
        TW<F_ReflectionContext> Context;
        F_Name Canonical;
    };
    class ABYTEK_BASE_REFLECTION_API A_ReflectionItem : public A_GBMObject
    {
    public:
        friend class F_ReflectionContext;
        
    private:
        TW<F_ReflectionContext> _Context;
        F_Name _Canonical;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetContext() const noexcept
        {
            return _Context;
        }
        ABYTEK_FORCE_INLINE const auto& GetCanonical() const noexcept
        {
            return _Canonical;
        }
        
    public:
        A_ReflectionItem(const F_GBMObjectConfig& GBMConfig, const F_ReflectionItemBuildParams& BuildParams);
        virtual ~A_ReflectionItem() override;
        
    public:
        void SetCanonical(const F_Name& Canonical);
    };
}