#include "Abytek/RHIDrawPass.hpp"
#include "Abytek/DirectX12/RHIDrawPass.hpp"
#include "Abytek/RHIDrawPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIDrawPass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
    switch (RHISubsystem->GetActiveAPI())
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
    case E_RHIAPI::DIRECTX12:
        return TU<F_DirectX12RHIDrawPass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIDrawPass::Build(const F_RHIDrawPassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        A_RHIHasGeneralPipeline::Build(BuildParams);
        _DrawType = BuildParams.DrawType;
        _NonIndexed = BuildParams.NonIndexed;
        _Indexed = BuildParams.Indexed;
        _DispatchMesh = BuildParams.DispatchMesh;
        _ViewportScissor = BuildParams.ViewportScissor;
        _Indirect = BuildParams.Indirect;
    }   
    void A_RHIDrawPass::Release()
    {
        _Indirect = {};
        _ViewportScissor = {};
        _DispatchMesh = {};
        _Indexed = {};
        _NonIndexed = {};
        _DrawType = E_RHIDrawType::NONE;
        A_RHIHasGeneralPipeline::Release();
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHIDrawPass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIDrawPassProxy>(ABYTEK_WTHIS());
    }
}
