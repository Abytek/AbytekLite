#include "Abytek/RHIDrawPassProxy.hpp"
#include "Abytek/RHIDrawPass.hpp"
#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIDrawPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIDrawPassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIDrawPassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIDrawPassProxy::Build(const TW_Valid<A_RHIDrawPass>& Pass)
    {
        A_RHIPassProxy::Build(Pass);
        A_RHIHasGeneralPipelineProxy::Build(Pass);
        _DrawType = Pass->GetDrawType();
        _NonIndexed = Pass->GetNonIndexed();
        _Indexed = Pass->GetIndexed();
        _DispatchMesh = Pass->GetDispatchMesh();
        _ViewportScissor = Pass->GetViewportScissor();
        if (auto Indirect = Pass->GetIndirect())
        {
            _IndirectProxy = F_RHIIndirectConfigProxy::From(*Indirect);
        }
    }   
    void A_RHIDrawPassProxy::Release()
    {
        _IndirectProxy = {};
        _ViewportScissor = {};
        _DispatchMesh = {};
        _Indexed = {};
        _NonIndexed = {};
        _DrawType = E_RHIDrawType::NONE;
        A_RHIHasGeneralPipelineProxy::Release();
        A_RHIPassProxy::Release();
    }
}