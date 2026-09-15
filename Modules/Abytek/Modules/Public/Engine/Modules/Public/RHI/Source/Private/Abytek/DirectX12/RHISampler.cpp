#include "Abytek/DirectX12/RHISampler.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/DirectX12/RHIContext.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIFormat.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"
#include "Abytek/DirectX12/RHISamplerProxy.hpp"
#include "Abytek/DirectX12/RHIProcess.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHISampler::Build(const F_RHISamplerBuildParams& BuildParams)
    {
        A_RHISampler::Build(BuildParams);
        D3D12InitDescriptor();
    }
    void F_DirectX12RHISampler::Release()
    {
        D3D12DeallocateDescriptor();
        A_RHISampler::Release();
    }
    
    void F_DirectX12RHISampler::D3D12InitDescriptor()
    {
        const auto& Access = GetAccess();
        
        DirectX12SharedAPIWrapper::E_DescriptorHeapType DescriptorHeapType = DirectX12SharedAPIWrapper::E_DescriptorHeapType::SAMPLER;
        
        auto DescriptorManager = GetContext().FastCast<F_DirectX12RHIContext>()->QueryDescriptorManager(
            DescriptorHeapType,
            GetAllowBindless()
        );
        _DescriptorAllocation = DescriptorManager->Allocate();
        
        DirectX12RHIProcessQueries::Execution::F_InitSamplerDescriptor Query;
        Query.SamplerProxy = GetProxy().FastCast<A_RHISamplerProxy>();
        Query.DescriptorAllocation = _DescriptorAllocation;
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()->Queues.Execution.InitSamplerDescriptor.Push(Query);
        
        InjectBindlessId() = _DescriptorAllocation.BeginOffset;
    }
    void F_DirectX12RHISampler::D3D12DeallocateDescriptor()
    {
        if (_DescriptorAllocation)
        {
            DirectX12RHIProcessQueries::Compile::F_DeallocateDescriptors Query;
            Query.DescriptorAllocation = _DescriptorAllocation;
            GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()->Queues.Compile.DeallocateDescriptors.Push(Query);
        }
        _DescriptorAllocation = {};
    }
}
#endif