#include "Abytek/RenderCoreExtensions.hpp"


namespace Abytek
{
    void H_RenderCoreExtensions::SafeCopyTexture2D(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime, 
        const TS<A_RHIResource>& DstTexture,
        const TS<A_RHIResource>& SrcTexture,
        const F_DebugName& DebugName
    )
    {
        const auto& DstTextureAccessCapabilities = DstTexture->GetAccessCapabilities();
        const auto& SrcTextureAccessCapabilities = SrcTexture->GetAccessCapabilities();
        const auto& DstTextureAspect = DstTexture->GetTextureAspect();
        const auto& SrcTextureAspect = SrcTexture->GetTextureAspect();
        
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(
            (DstTextureAspect.Width == SrcTextureAspect.Width)  
            && (DstTextureAspect.Height == SrcTextureAspect.Height)  
        ) << "texture size mismatch";
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(
            (DstTextureAspect.DimensionCount == SrcTextureAspect.DimensionCount)
        ) << "texture dimensions mismatch";
        
        if (
            !FlagHas(DstTextureAccessCapabilities.GPU, E_RHIResourceGPUAccess::DSV)
            && !FlagHas(SrcTextureAccessCapabilities.GPU, E_RHIResourceGPUAccess::DSV)
        )
        {
            H_RHISubmissionUtilities::CopyTexture(
                SubmissionItemContainer,
                DstTexture,
                SrcTexture,
                { F_RHITextureElement {} },
                { F_RHITextureElement {} },
                DebugName
            );
            return;
        }
        
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(FlagHas(SrcTextureAccessCapabilities.GPU, E_RHIResourceGPUAccess::SRV)) << "Requires SRV access for src texture";
        
        if (FlagHas(DstTextureAccessCapabilities.GPU, E_RHIResourceGPUAccess::DSV))
        {
            F_RHITextureViewBuildParams DSVBuildParams;
            DSVBuildParams.Context = DstTexture->GetContext().Weak();
            DSVBuildParams.Format = E_RHIFormat::D32_FLOAT;
            DSVBuildParams.Access = F_RHIResourceAccess::MakeDSV();
            auto DSV = RACreateAndBuildShared<A_RHIResourceView>(DSVBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            DSV->SetDebugName(
                *DebugName
                + ABYTEK_TEXT(".DSV")
            );
#endif
            H_RHISubmissionUtilities::ClearDSV(
                SubmissionItemContainer,
                DSV,
                E_RHIClearDSVFlag::DEPTH,
                0.0f,
                0
#ifdef ABYTEK_DEBUG_INFO
                , *DebugName + ABYTEK_TEXT(".ClearDSV")
#endif
            );
            MergeDepthTexture(
                SubmissionItemContainer,
                RenderRegistryRuntime,
                DstTexture,
                SrcTexture,
                DebugName
            );
            return;
        }
        
        if (FlagHas(SrcTextureAccessCapabilities.GPU, E_RHIResourceGPUAccess::DSV))
        {
            auto BindGroup = RenderCoreExtensions::CopyTexture2D_R32_SRVToUAV::F_Binding::Instantiate(
                RenderRegistryRuntime    
            ).CreateBindGroup();
            RenderCoreExtensions::CopyTexture2D_R32_SRVToUAV::F_UniformData UniformData;
            UniformData.Resolution = F_Vector2_U32(DstTextureAspect.Width, DstTextureAspect.Height);
            BindGroup->BindUniformData(ABYTEK_NAME("UniformData"), UniformData);
            BindGroup->BindResourceView(
                ABYTEK_NAME("SRV"), 
                SrcTexture, 
                F_RHIResourceAccess::MakeSRV(), 
                E_RHIFormat::R32_FLOAT
            );
            BindGroup->BindResourceView(
                ABYTEK_NAME("UAV"), 
                DstTexture
            );
            BindGroup->Commit();
            
            H_RHISubmissionUtilities::DispatchCompute(
                SubmissionItemContainer,
                RenderCoreExtensions::CopyTexture2D_R32_SRVToUAV::F_Pipeline::Instantiate(
                    RenderRegistryRuntime
                ).AcquirePipelineState(),
                {
                    BindGroup
                },
                F_Vector3_U32(
                    RoundUpDivide(DstTextureAspect.Width, 8U),    
                    RoundUpDivide(DstTextureAspect.Height, 8U),
                    1
                ),
                E_RHIGPUWorkClass::DEFAULT,
                DebugName
            );
            return;
        }
        
        ABYTEK_LOG_FATAL() << "Unable to copy texture 2D";
    }
    void H_RenderCoreExtensions::MergeDepthTexture(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime, 
        const TS<A_RHIResource>& DstTexture,
        const TS<A_RHIResource>& SrcTexture,
        const F_DebugName& DebugName
    )
    {
        const auto& DstTextureAccessCapabilities = DstTexture->GetAccessCapabilities();
        const auto& SrcTextureAccessCapabilities = SrcTexture->GetAccessCapabilities();
        const auto& DstTextureAspect = DstTexture->GetTextureAspect();
        const auto& SrcTextureAspect = SrcTexture->GetTextureAspect();
        
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(
            (DstTextureAspect.Width == SrcTextureAspect.Width)  
            && (DstTextureAspect.Height == SrcTextureAspect.Height)  
        ) << "texture size mismatch";
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(
            (DstTextureAspect.DimensionCount == SrcTextureAspect.DimensionCount)
        ) << "texture dimensions mismatch";
        
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(FlagHas(DstTextureAccessCapabilities.GPU, E_RHIResourceGPUAccess::DSV)) << "Requires DSV access for dst texture";
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(FlagHas(SrcTextureAccessCapabilities.GPU, E_RHIResourceGPUAccess::SRV)) << "Requires SRV access for src texture";
        
        auto BindGroup = RenderCoreExtensions::CopyTexture2D_R32_SRVToDSV::F_Binding::Instantiate(
            RenderRegistryRuntime    
        ).CreateBindGroup();
        RenderCoreExtensions::CopyTexture2D_R32_SRVToDSV::F_UniformData UniformData;
        UniformData.Resolution = F_Vector2_U32(DstTextureAspect.Width, DstTextureAspect.Height);
        BindGroup->BindUniformData(ABYTEK_NAME("UniformData"), UniformData);
        BindGroup->BindResourceView(
            ABYTEK_NAME("SRV"), 
            SrcTexture, 
            F_RHIResourceAccess::MakeSRV(), 
            E_RHIFormat::R32_FLOAT
        );
        BindGroup->BindDSV(
            ABYTEK_NAME("DSV"), 
            DstTexture
        );
        BindGroup->Commit();
            
        H_RHISubmissionUtilities::DrawNonIndexed(
            SubmissionItemContainer,
            RenderCoreExtensions::CopyTexture2D_R32_SRVToDSV::F_Pipeline::Instantiate(
                RenderRegistryRuntime
            ).AcquirePipelineState(),
            {
                BindGroup
            },
            F_RHIViewportScissorConfig::Make(F_Vector2_F32(UniformData.Resolution)),
            F_RHIDrawNonIndexedConfig::Make(6),
            E_RHIGPUWorkClass::DEFAULT,
            DebugName
        );
    }
}
