#pragma once

#include "Abytek/RHIDispatchComputePass.hpp"
#include "Abytek/RHIHasGeneralPipelineProxy.hpp"


namespace Abytek
{
    class A_RHIResourceProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHIDispatchComputePassProxy : public A_RHIPassProxy, public A_RHIHasGeneralPipelineProxy
    {
    private:
        F_Vector3_U32 _NumThreadGroups = F_Vector3_U32::Zero();
        
        TF_Optional<F_RHIIndirectConfigProxy> _IndirectProxy;

    public:
        ABYTEK_FORCE_INLINE const auto& GetNumThreadGroups() const noexcept
        {
            return _NumThreadGroups;
        }
        ABYTEK_FORCE_INLINE auto& InjectNumThreadGroups() noexcept
        {
            return _NumThreadGroups;
        }

        ABYTEK_FORCE_INLINE const auto& GetIndirectProxy() const noexcept
        {
            return _IndirectProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectIndirectProxy() noexcept
        {
            return _IndirectProxy;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIDispatchComputePassProxy);
        virtual void Build(const TW_Valid<A_RHIDispatchComputePass>& Pass);
        virtual void Release() override;
    };
}