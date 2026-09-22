#pragma once

#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIResource.hpp"


namespace Abytek
{
    struct F_RHIResourceSyncPassBuildParams : public F_RHIPassBuildParams
    {
        TS<A_RHIResource> Resource;
        F_RHIResourceAccess Access;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIResourceSyncPass : public A_RHIPass
    {
    private:
        TS<A_RHIResource> _Resource;
        F_RHIResourceAccess _Access;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetResource() const noexcept
        {
            return _Resource;
        }
        ABYTEK_FORCE_INLINE const auto& GetAccess() const noexcept
        {
            return _Access;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIResourceSyncPass);
        virtual void Build(const F_RHIResourceSyncPassBuildParams& BuildParams);
        void Release() override;
        
    public:
        E_RHIPassClass GetPassClass() const override
        {
            if (_Access.GPU != E_RHIResourceGPUAccess::NONE)
            {
                if (
                    FlagHas(_Access.GPU, E_RHIResourceGPUAccess::RTV)
                    || FlagHas(_Access.GPU, E_RHIResourceGPUAccess::DSV)
                )
                {
                    return E_RHIPassClass::GRAPHICS;
                }
                return E_RHIPassClass::COMPUTE;
            }
            if (_Access.CPU != E_RHIResourceCPUAccess::NONE)
            {
                return E_RHIPassClass::BLIT;
            }
            return E_RHIPassClass::NONE;
        }
        
    public:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy() override;
    };
}