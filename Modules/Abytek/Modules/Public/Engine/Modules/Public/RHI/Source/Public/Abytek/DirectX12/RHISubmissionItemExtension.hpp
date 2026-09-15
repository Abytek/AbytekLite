#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIProcessDataHandle.hpp"
#include "Abytek/RHISubmissionItem.hpp"
#include "Abytek/RHIResourceAccess.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class A_DirectX12RHISubmissionItemExtension;
    class A_DirectX12RHISubmissionListExtension;
    class A_DirectX12RHIPassExtension;
    class A_RHIResource;
    
    struct F_DirectX12RHISubmissionItemGraphData
    {
        U32 Offset = 0;
        TF_SmallVector<TW<A_DirectX12RHISubmissionListExtension>, 4> ListExtensions;
    };
    
    struct F_DirectX12RHISubmissionItemExtensionProcessData
    {
        F_DirectX12RHISubmissionItemGraphData GraphData;
        
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DRIVER_DEBUGGER
        struct F_SubresourceAccessValidationData
        {
            F_DirectX12RHISubresourceBindingReference BindingReference;
        };
        TF_MultiMap<
            F_DirectX12RHISubresourceReference, 
            F_SubresourceAccessValidationData, 
            TF_HashBasedLess<F_DirectX12RHISubresourceReference>
        > SubresourceAccessValidationData; 
        
        TF_Map<
            F_DirectX12RHISubresourceReference, 
            E_RHIResourceAccessCategory, 
            TF_HashBasedLess<F_DirectX12RHISubresourceReference>
        > SubresourceAccessCategories;
#endif
    };

    class ABYTEK_ENGINE_RHI_API A_DirectX12RHISubmissionItemExtension
    {
    private:
        TW<A_RHISubmissionItem> _SubmissionItem;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetSubmissionItem() const noexcept
        {
            return _SubmissionItem;
        }
        
    public:
        TF_RHIProcessDataHandle<F_DirectX12RHISubmissionItemExtensionProcessData> ProcessDataHandle_SubmissionItemExtension;
        ABYTEK_FORCE_INLINE auto GetProcessData_SubmissionItemExtension() const noexcept
        {
            return ProcessDataHandle_SubmissionItemExtension.Get(GetSubmissionItem()->GetProcess());
        }
        
    public:
        A_DirectX12RHISubmissionItemExtension();
        virtual ~A_DirectX12RHISubmissionItemExtension();

    public:
        virtual void Build();
        virtual void Release();
        
    public:
        B8 IsBeforeIfHasLowerOffset(const TW_Valid<A_DirectX12RHISubmissionItemExtension>& X);
    };
}
#endif