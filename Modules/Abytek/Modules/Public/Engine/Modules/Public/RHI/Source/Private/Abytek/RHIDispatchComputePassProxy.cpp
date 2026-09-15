#include "Abytek/RHIDispatchComputePassProxy.hpp"
#include "Abytek/RHIDispatchComputePass.hpp"
#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIDispatchComputePassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIDispatchComputePassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIDispatchComputePassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIDispatchComputePassProxy::Build(const TW_Valid<A_RHIDispatchComputePass>& Pass)
    {
        A_RHIPassProxy::Build(Pass);
        A_RHIHasGeneralPipelineProxy::Build(Pass);
        _NumThreadGroups = Pass->GetNumThreadGroups();
        if (auto Indirect = Pass->GetIndirect())
        {
            _IndirectProxy = F_RHIIndirectConfigProxy::From(*Indirect);
        }
    }   
    void A_RHIDispatchComputePassProxy::Release()
    {
        _IndirectProxy = {};
        _NumThreadGroups = F_Vector3_U32::Zero();
        A_RHIHasGeneralPipelineProxy::Release();
        A_RHIPassProxy::Release();
    }
}