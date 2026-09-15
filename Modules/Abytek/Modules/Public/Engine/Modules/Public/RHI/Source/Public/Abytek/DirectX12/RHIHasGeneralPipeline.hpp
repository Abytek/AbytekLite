#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIHasGeneralPipeline.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API A_DirectX12RHIHasGeneralPipeline
    {
    private:
        TW<A_RHIPass> _Pass;
        TW<A_DirectX12RHIPassExtension> _PassExtension;
        TW<A_RHIHasGeneralPipeline> _Base;
        
        E_RHIPassClass _PassClass = E_RHIPassClass::NONE;

    public:

    public:
        A_DirectX12RHIHasGeneralPipeline() = default;
        virtual ~A_DirectX12RHIHasGeneralPipeline() = default;
        void Build(const F_RHIHasGeneralPipelineBuildParams& BuildParams);
        void Release();
        
    protected:
        void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet);
    };
}
#endif