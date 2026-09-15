#pragma once

#include "Abytek/RHIProcessDataHandle.hpp"
#include "Abytek/RHIPass.hpp"
#include "Abytek/DirectX12/RHISubmissionItemExtension.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"
#include "Abytek/DirectX12/RHIContext.hpp"
#include "Abytek/DirectX12/RHIProfilerEventState.hpp"
#include "Abytek/DirectX12/RHIPassBatch.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHICommandQueue;
    class F_DirectX12RHICommandQueueProxy;
    class A_RHIProcess;
    class A_RHIResource;
    class A_RHIPass;
    class A_DirectX12RHIPassExtension;
    
    struct F_DirectX12RHIPassExtensionProcessData
    {
        U32 GlobalIndex = ~U32(0);
        U32 IndexInSection = ~U32(0);
        U32 SectionIndex = 0;
        U32 SortedIndexInSection = ~U32(0);
        U32 BatchIndex = ~U32(0);
        
        TW<F_DirectX12RHICommandQueue> CommandQueue;
        
        F_DirectX12RHISubresourceBindingSet SubresourceBindingSet;
        F_DirectX12RHIResourceUseSet ResourceUseSet;
        
        TF_SmallVector<TW<A_DirectX12RHIPassExtension>, 16> Dependencies;
        U32 DependencyLevel = 0;
        U64 DependencyScore = 0;
        
        F_DirectX12RHIResourceBarrierProxySet D3D12ResourceBarrierProxies_Before;
        F_DirectX12RHIResourceBarrierProxySet D3D12ResourceBarrierProxies_After;
        
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
        TF_SmallVector<F_DirectX12RHIProfilerEventState, 1> ProfilerEventStatesToBegin;
        TF_SmallVector<F_DirectX12RHIProfilerEventState, 1> ProfilerEventStatesToEnd;
#endif
    };
    
    class ABYTEK_ENGINE_RHI_API A_DirectX12RHIPassExtension : public A_DirectX12RHISubmissionItemExtension
    {
    private:
        
    public:
        ABYTEK_FORCE_INLINE auto GetPass() const noexcept
        {
            return GetSubmissionItem().FastCast<A_RHIPass>();
        }
        
    public:
        TF_RHIProcessDataHandle<F_DirectX12RHIPassExtensionProcessData> ProcessDataHandle_PassExtension;
        ABYTEK_FORCE_INLINE auto GetProcessData_PassExtension() const noexcept
        {
            return ProcessDataHandle_PassExtension.Get(GetSubmissionItem()->GetProcess());
        }
        
    public:
        A_DirectX12RHIPassExtension();
        virtual ~A_DirectX12RHIPassExtension() override;

    public:
        virtual void Build() override;
        virtual void Release() override;
        
    protected:
        virtual void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet);
        
    public:
        void GatherSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet);
        
    public:
        virtual E_DirectX12RHIPassBatchType GetPassBatchType();
        virtual E_DirectX12RHIExecutionRangeType GetExecutionRangeType();
    };
}
#endif