#include "Abytek/DirectX12/RHIFence.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHICommandQueueProxy.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHIFence);
    void F_DirectX12RHIFence::Build(const F_DirectX12RHIFenceBuildParams& BuildParams)
    {
        A_RHIDeviceChild::Build(BuildParams.Device);

        HRESULT HR = BuildParams.Device.FastCast<F_DirectX12RHIDevice>()->GetD3D12Device()->CreateFence(
            BuildParams.InitialValue,
            D3D12_FENCE_FLAG_SHARED,
            IID_PPV_ARGS(&_D3D12Fence)
        );
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot create d3d12 fence";
    }
    void F_DirectX12RHIFence::Release()
    {
        _InitialValue = 0;
        _D3D12Fence = nullptr;
        A_RHIDeviceChild::Release();
    }

    void F_DirectX12RHIFence::SignalFromCPUSide(U64 NewValue)
    {
        HRESULT HR =_D3D12Fence->Signal(NewValue);
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot signal d3d12 fence from CPU-side";
    }
    void F_DirectX12RHIFence::SignalFromGPUSide(U64 NewValue, const TW_Valid<F_DirectX12RHICommandQueueProxy>& CommandQueueProxy)
    {
        CommandQueueProxy->GetD3D12CommandQueue()->Signal(
            _D3D12Fence.Get(),
            NewValue
        );
    }
    void F_DirectX12RHIFence::WaitOnGPUSide(U64 Value, const TW_Valid<F_DirectX12RHICommandQueueProxy>& CommandQueueProxy)
    {
        CommandQueueProxy->GetD3D12CommandQueue()->Wait(
            _D3D12Fence.Get(),
            Value
        );
    }
    U64 F_DirectX12RHIFence::GetCompletedValue() const
    {
        return _D3D12Fence->GetCompletedValue();
    }
    B8 F_DirectX12RHIFence::IsPassed(U64 MinValue) const
    {
        U64 CompletedValue = GetCompletedValue();
        return CompletedValue >= MinValue;
    }
    void F_DirectX12RHIFence::WaitOnCPUSide(U64 MinValue) const
    {
        while (!IsPassed(MinValue))
        {
            H_TaskUtilities::Switch();
        }
    }
}
#endif