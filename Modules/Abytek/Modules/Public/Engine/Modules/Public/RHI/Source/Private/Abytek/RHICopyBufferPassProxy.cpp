#include "Abytek/RHICopyBufferPassProxy.hpp"
#include "Abytek/RHICopyBufferPass.hpp"
#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHICopyBufferPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHICopyBufferPassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHICopyBufferPassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHICopyBufferPassProxy::Build(const TW_Valid<A_RHICopyBufferPass>& Pass)
    {
        BuildPassProxy(Pass);
        _SrcBufferProxy = Pass->GetSrcBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
        _SrcOffsetInBytes = Pass->GetSrcOffsetInBytes();
        _DstBufferProxy = Pass->GetDstBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
        _DstOffsetInBytes = Pass->GetDstOffsetInBytes();
        _SizeInBytes = Pass->GetSizeInBytes();
    }   
    void A_RHICopyBufferPassProxy::Release()
    {
        _SizeInBytes = 0;
        _DstOffsetInBytes = 0;
        _DstBufferProxy = {};
        _SrcOffsetInBytes = 0;
        _SrcBufferProxy = {};
        A_RHIPassProxy::Release();
    }
}