#pragma once

#include "Abytek/RHIBindGroupProxy.hpp"
#include "Abytek/DirectX12/RHISubmissionContext.hpp"
#include "Abytek/DirectX12/RHIBindGroup.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    struct ABYTEK_ENGINE_RHI_API H_DirectX12RHIBindGroupProxy
    {
        static void PopulateCommandList(
            F_DirectX12RHISubmissionContext& SubmissionContext,
            const TF_Span<TW<A_RHIBindGroupProxy>>& BindGroupProxies,
            const TF_Span<F_DirectX12SharedRHIPipelineStateTemplateCompiledData::F_BindGroupMap>& BindGroupMaps,
            E_RHIPassClass PassClass
        );
        template<typename __F_BindGroupProxies, typename __F_BindGroupMaps>
        ABYTEK_FORCE_INLINE static void PopulateCommandList(
            F_DirectX12RHISubmissionContext& SubmissionContext,
            const __F_BindGroupProxies& BindGroupProxies,
            const __F_BindGroupMaps& BindGroupMaps,
            E_RHIPassClass PassClass
        )
        {
            PopulateCommandList(
                SubmissionContext,
                TF_Span<TW<A_RHIBindGroupProxy>>(
                    (TW<A_RHIBindGroupProxy>*)BindGroupProxies.data(),
                    BindGroupProxies.size()
                ),
                TF_Span<F_DirectX12SharedRHIPipelineStateTemplateCompiledData::F_BindGroupMap>(
                    (F_DirectX12SharedRHIPipelineStateTemplateCompiledData::F_BindGroupMap*)BindGroupMaps.data(),
                    BindGroupMaps.size()
                ),
                PassClass
            );
        }
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIBindGroupProxy : public A_RHIBindGroupProxy
    {
    private:
        F_RHIBindGroupSlotDataProxy _SlotDataProxy;
        F_DirectX12RHIBindGroupAllocationProxy _AllocationProxy;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetSlotDataProxy() const noexcept
        {
            return _SlotDataProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetAllocationProxy() const noexcept
        {
            return _AllocationProxy;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIBindGroupProxy);
        void LateBuildBindGroup_DirectX12();
        virtual void Release() override;
    
    public:
        void AssignCommit(
            F_RHIBindGroupSlotDataProxy&& SlotDataProxy,
            F_DirectX12RHIBindGroupAllocationProxy&& AllocationProxy
        );
    };
}
#endif