#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/SRPCommon.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"
#include "Abytek/RHIBufferInlineAllocator.hpp"
#include "Abytek/RHIClearUAVUIntPass.hpp"
#include "Abytek/RHIHelper.hpp"
#include "Abytek/RHISubmissionUtilities.hpp"


namespace Abytek
{
    namespace SRP::VisibilityBuffer
    {
        inline B8 SupportSingleChannelFormat(const F_RHIFeatureSupports& FeatureSupports)
        {
            return false;
        };
        inline E_RHIFormat GetFormat(const F_RHIFeatureSupports& FeatureSupports)
        {
            return E_RHIFormat::R32G32_UINT;
        };
        inline F_FeedbackStatus SetupCompileParams(F_RHIPipelineStateTemplateCompileParams& CompileParams)
        {
            if (SupportSingleChannelFormat(CompileParams.Database->GetFeatureSupports()))
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    CompileParams.AddShaderDefinition(
                        ABYTEK_NAME("ABYTEK_SRP_VISIBILITY_BUFFER_SINGLE_CHANNEL")
                    )
                );
            }
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        inline F_Name GetBindGroupSlotName(const F_Name& Name)
        {
            return ABYTEK_TEXT("___Abytek_VisibilityBuffer_Opaque_") + *Name;
        }
        
        struct F_OpaqueInstance
        {
            TS<A_RHIResource> Resource;
            TS<A_RHIResourceView> SRV;
            TS<A_RHIResourceView> UAV;
            
            ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
            {
                return (
                    Resource
                    && SRV
                    && UAV
                );
            }
            ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
            {
                return IsValid();
            }
            
            static F_OpaqueInstance Create(
                const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
                const F_Vector2_U32& Size, 
                const F_RHIFeatureSupports& FeatureSupports, 
                E_RHIResourceAdditionalFlag ResourceAdditionalFlags = E_RHIResourceAdditionalFlag::DEFAULT
            );
            void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
            void Clear(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const F_Name& DebugName = {}) const;
            void Bind(const TS<A_RHIBindGroup>& BindGroup, const F_Name& Name, B8 EnableWrite = false) const;
            
#ifdef ABYTEK_DEBUG_INFO
            void SetDebugName(const F_Name& DebugName);
#endif
        };
        
        inline F_FeedbackStatus AddToBindGroup_Opaque(
            F_RHIBindGroupTemplateCompileParams& CompileParams,
            const F_Name& Name,
            B8 EnableWrite = false
        )
        {
            CompileParams.Slots.push_back(
                F_RHIBindGroupTemplateSlot::MakeResourceView(
                      GetBindGroupSlotName(Name),
                      EnableWrite ? F_RHIResourceAccess::MakeUAV() : F_RHIResourceAccess::MakeSRV()
                )
            );
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        struct F_DemoBinding : F_GlobalRenderBinding
        {
            ABYTEK_GLOBAL_RENDER_BINDING(F_DemoBinding, ABYTEK_NAME("Abytek::SRP::VisibilityBuffer::F_DemoBinding"));
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    AddToBindGroup_Opaque(
                        Config, 
                        ABYTEK_NAME("OpaqueVisibilityBuffer"),
                        true
                    )    
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct F_DemoPipeline : F_GlobalRenderPipeline
        {
            ABYTEK_GLOBAL_RENDER_PIPELINE(F_DemoPipeline, ABYTEK_NAME("Abytek::SRP::VisibilityBuffer::F_DemoPipeline"));
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Type = E_RHIPipelineStateType::COMPUTE;
                Config.ComputeShader = ABYTEK_GLOBAL_SHADER("MainCS", "Abytek/SRPDemoOpaqueVisibilityBufferCS", E_RHIShaderFrequency::COMPUTE);
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        F_DemoBinding::GetTemplateHashCode()
                    )
                );
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    SetupCompileParams(Config)
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
    }
}
