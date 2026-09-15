#include "Abytek/RHIReadbackBufferPassProxy.hpp"
#include "Abytek/RHIReadbackBufferPass.hpp"
#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIReadbackBufferPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIReadbackBufferPassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIReadbackBufferPassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIReadbackBufferPassProxy::Build(const TW_Valid<A_RHIReadbackBufferPass>& Pass)
    {
        A_RHIPassProxy::Build(Pass);
        _BufferProxy = Pass->GetBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
        _SizeInBytes = Pass->GetSizeInBytes();
        _OffsetInBytes = Pass->GetOffsetInBytes();
        _Callback = Pass->GetCallback();
    }   
    void A_RHIReadbackBufferPassProxy::Release()
    {
        _Callback = {};
        _OffsetInBytes = 0;
        _SizeInBytes = 0;
        _BufferProxy = {};
        A_RHIPassProxy::Release();
    }
}