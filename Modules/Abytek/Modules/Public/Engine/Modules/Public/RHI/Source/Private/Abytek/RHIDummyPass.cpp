#include "Abytek/RHIDummyPass.hpp"
#include "Abytek/DirectX12/RHIDummyPass.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIDummyPass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
    switch (RHISubsystem->GetActiveAPI())
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
    case E_RHIAPI::DIRECTX12:
        return TU<F_DirectX12RHIDummyPass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIDummyPass::Build(const F_RHIDummyPassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        _ResourceViews = BuildParams.ResourceViews;
        _VertexBufferBindings = BuildParams.VertexBufferBindings;
        _IndexBufferBinding = BuildParams.IndexBufferBinding;
    }   
    void A_RHIDummyPass::Release()
    {
        _IndexBufferBinding = {};
        _VertexBufferBindings = {};
        _ResourceViews = {};
        A_RHIPass::Release();
    }
}