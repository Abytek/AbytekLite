#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIBindGroupProxy.hpp"
#include "Abytek/RHIPipelineStateProxy.hpp"


namespace Abytek
{
    class A_RHIPass;
    class A_RHIBindGroupProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHIHasGeneralPipelineProxy
    {
    private:
        TS<A_RHIPipelineStateProxy> _PipelineStateProxy;
        F_RHIBindGroupProxySet _BindGroupProxies;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetPipelineStateProxy() const noexcept
        {
            return _PipelineStateProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectPipelineStateProxy() noexcept
        {
            return _PipelineStateProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetBindGroupProxies() const noexcept
        {
            return _BindGroupProxies;
        }
        ABYTEK_FORCE_INLINE auto& InjectBindGroupProxies() noexcept
        {
            return _BindGroupProxies;
        }
        
    public:
        A_RHIHasGeneralPipelineProxy() = default;
        virtual ~A_RHIHasGeneralPipelineProxy() = default;
        void Build(const TW_Valid<A_RHIPass>& Pass);
        void Release();
    };
}
