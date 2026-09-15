#include "Abytek/DirectX12/RHISubmissionListExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    A_DirectX12RHISubmissionListExtension::A_DirectX12RHISubmissionListExtension()
    {
    }
    A_DirectX12RHISubmissionListExtension::~A_DirectX12RHISubmissionListExtension()
    {
    }
    
    void A_DirectX12RHISubmissionListExtension::Build()
    {
        A_DirectX12RHISubmissionItemExtension::Build();
    }
    void A_DirectX12RHISubmissionListExtension::Release()
    {
        A_DirectX12RHISubmissionItemExtension::Release();
    }
}
#endif