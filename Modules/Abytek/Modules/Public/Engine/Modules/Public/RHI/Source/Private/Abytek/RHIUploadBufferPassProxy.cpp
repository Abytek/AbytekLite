#include "Abytek/RHIUploadBufferPassProxy.hpp"
#include "Abytek/RHIUploadBufferPass.hpp"
#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIUploadBufferPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIUploadBufferPassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIUploadBufferPassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIUploadBufferPassProxy::Build(const TW_Valid<A_RHIUploadBufferPass>& Pass)
    {
        BuildPassProxy(Pass);
        _BufferDataView = Pass->GetBufferDataView();;
        _BufferProxy = Pass->GetBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
        _OffsetInBytes = Pass->GetOffsetInBytes();
    }   
    void A_RHIUploadBufferPassProxy::Release()
    {
        _OffsetInBytes = 0;
        _BufferProxy = {};
        _BufferDataView = {};
        A_RHIPassProxy::Release();
    }
}