#pragma once

#include "Abytek/RHIResourceSyncPass.hpp"


namespace Abytek
{
    class A_RHIResourceProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHIResourceSyncPassProxy : public A_RHIPassProxy
    {
    private:
        TS<A_RHIResourceProxy> _ResourceProxy;
        F_RHIResourceAccess _Access;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetResourceProxy() const noexcept
        {
            return _ResourceProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetAccess() const noexcept
        {
            return _Access;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIResourceSyncPassProxy);
        virtual void Build(const TW_Valid<A_RHIResourceSyncPass>& Pass);
        virtual void Release() override;
    };
}