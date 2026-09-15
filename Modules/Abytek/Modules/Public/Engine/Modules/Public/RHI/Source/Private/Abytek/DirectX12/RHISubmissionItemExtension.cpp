#include "Abytek/DirectX12/RHISubmissionItemExtension.hpp"
#include "Abytek/RHISubmissionList.hpp"
#include "Abytek/DirectX12/RHISubmissionListExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    A_DirectX12RHISubmissionItemExtension::A_DirectX12RHISubmissionItemExtension()
    {
    }
    A_DirectX12RHISubmissionItemExtension::~A_DirectX12RHISubmissionItemExtension()
    {
    }
    
    void A_DirectX12RHISubmissionItemExtension::Build()
    {
        _SubmissionItem = ABYTEK_WTHIS().DynamicCast<A_RHISubmissionItem>();
    }
    void A_DirectX12RHISubmissionItemExtension::Release()
    {
        _SubmissionItem = {};
    }

    B8 A_DirectX12RHISubmissionItemExtension::IsBeforeIfHasLowerOffset(const TW_Valid<A_DirectX12RHISubmissionItemExtension>& X)
    {
        const auto& ThisParentProxies = GetProcessData_SubmissionItemExtension()->GraphData.ListExtensions;
        const auto& XParentProxies = X->GetProcessData_SubmissionItemExtension()->GraphData.ListExtensions;
        U32 ThisNumParentProxies = ThisParentProxies.size();
        U32 XNumParentProxies = XParentProxies.size();

        E_RHISubmissionListOrder LastOrder = E_RHISubmissionListOrder::SEQUENTIAL;
        for (U32 Idx = 0; Idx < ThisNumParentProxies; ++Idx)
        {
            if (Idx >= XNumParentProxies)
            {
                break;
            }
            
            const auto& ThisParentProxy = ThisParentProxies[Idx];
            const auto& XParentProxy = XParentProxies[Idx];
            if (ThisParentProxy != XParentProxy)
            {
                break;
            }
            
            LastOrder = ThisParentProxy->GetSubmissionList()->GetOrder();
        }
        return (LastOrder == E_RHISubmissionListOrder::SEQUENTIAL);
    }
}
#endif