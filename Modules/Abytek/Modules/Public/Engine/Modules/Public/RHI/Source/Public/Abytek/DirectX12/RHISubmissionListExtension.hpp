#pragma once

#include "Abytek/RHIProcessDataHandle.hpp"
#include "Abytek/DirectX12/RHISubmissionItemExtension.hpp"
#include "Abytek/RHISubmissionList.hpp"
 

#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    struct F_DirectX12RHISubmissionListExtensionProcessData
    {
    }; 
    
    class ABYTEK_ENGINE_RHI_API A_DirectX12RHISubmissionListExtension : public A_DirectX12RHISubmissionItemExtension
    {
    private:
        
    public:
        ABYTEK_FORCE_INLINE auto GetSubmissionList() const noexcept
        {
            return GetSubmissionItem().FastCast<A_RHISubmissionList>();
        }
        
    public:
        TF_RHIProcessDataHandle<F_DirectX12RHISubmissionListExtensionProcessData> ProcessDataHandle_SubmissionListExtension;
        ABYTEK_FORCE_INLINE auto GetProcessData_SubmissionListExtension() const noexcept
        {
            return ProcessDataHandle_SubmissionListExtension.Get(GetSubmissionItem()->GetProcess());
        }
        
    public:
        A_DirectX12RHISubmissionListExtension();
        virtual ~A_DirectX12RHISubmissionListExtension() override;

    public:
        virtual void Build() override;
        virtual void Release() override;
    };
}
#endif