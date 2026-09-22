#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"


namespace Abytek
{
    class F_GPUDataPage;
    
    struct F_GPUDataInstanceAllocation
    {
        TS<F_GPUDataPage> Page;
        U32 BeginLocalIndex = 0;
        U32 EndLocalIndex = 0;
    
        ABYTEK_FORCE_INLINE Sz GetSize() const noexcept
        {
            return EndLocalIndex - BeginLocalIndex;
        }
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return Page && GetSize();
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
    };
    
    struct F_GPUDataInstanceAddress
    {
        U32 PageIndex = 0;
        U32 LocalIndex = 0;
    };
    
    namespace GPUData
    {
        inline B8 SupportMultiplePages(const F_RHIFeatureSupports& FeatureSupports)
        {
            return ABYTEK_RHI_FEATURE_CHECK(
                Binding,
                F_RHIFeatureSet::E_Binding::L2, // dynamic descriptor indexing
                FeatureSupports
            );
        }
        inline F_FeedbackStatus SetupCompileParams(F_RHIPipelineStateTemplateCompileParams& CompileParams)
        {
            if (SupportMultiplePages(CompileParams.Database->GetFeatureSupports()))
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    CompileParams.AddShaderDefinition(
                        ABYTEK_NAME("ABYTEK_GPU_DATA_ENABLE_MULTIPLE_PAGES")
                    )
                );
            }
            return F_FeedbackStatus::MakeSucceeded();
        }
    }
}