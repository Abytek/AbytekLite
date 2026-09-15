#include "Abytek/DirectX12/RHIResourcePlacement.hpp"
#include "Abytek/DirectX12/RHIPlacedResourceManager.hpp"
#include "Abytek/DirectX12/RHIPlacedResourceManagerProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    F_DirectX12RHIResourcePlacementProxy F_DirectX12RHIResourcePlacementProxy::From(const F_DirectX12RHIResourcePlacement& X)
    {
        F_DirectX12RHIResourcePlacementProxy Result;
        Result.PageProxy = X.Page->GetProxy().FastCast<F_DirectX12RHIPlacedResourcePageProxy>().Weak();
        Result.BeginOffsetInBytes = X.BeginOffsetInBytes;
        Result.EndOffsetInBytes = X.EndOffsetInBytes;
        return Result;
    }
}
#endif