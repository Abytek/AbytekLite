#include "Abytek/SRPVisibilityBuffer.hpp"


namespace Abytek
{
    namespace SRP::VisibilityBuffer
    {
        F_OpaqueInstance F_OpaqueInstance::Create(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
            const F_Vector2_U32& Size, 
            const F_RHIFeatureSupports& FeatureSupports, 
            E_RHIResourceAdditionalFlag ResourceAdditionalFlags
        )
        {
            F_OpaqueInstance Result;
            
            auto Context = H_RHI::GetMainContext();
        
            F_RHITextureBuildParams OpaqueVisibilityBufferBuildParams;
            OpaqueVisibilityBufferBuildParams.Context = Context.Weak();
            OpaqueVisibilityBufferBuildParams.TextureAspect.Width = Size.X;
            OpaqueVisibilityBufferBuildParams.TextureAspect.Height = Size.Y;
            OpaqueVisibilityBufferBuildParams.TextureAspect.DimensionCount = 2;
            OpaqueVisibilityBufferBuildParams.Format = GetFormat(FeatureSupports);
            OpaqueVisibilityBufferBuildParams.AccessCapabilities = (
                F_RHIResourceAccess::MakeSRVCapabilities() 
                | F_RHIResourceAccess::MakeUAVCapabilities()
            );
            OpaqueVisibilityBufferBuildParams.AdditionalFlags = ResourceAdditionalFlags;
            Result.Resource = RACreateAndBuildShared<A_RHIResource>(OpaqueVisibilityBufferBuildParams);
        
            F_RHITextureViewBuildParams OpaqueVisibilitySRVBuildParams;
            OpaqueVisibilitySRVBuildParams.Context = Context.Weak();
            OpaqueVisibilitySRVBuildParams.Resource = Result.Resource;
            OpaqueVisibilitySRVBuildParams.Access = F_RHIResourceAccess::MakeSRV();
            Result.SRV = RACreateAndBuildShared<A_RHIResourceView>(OpaqueVisibilitySRVBuildParams);
        
            F_RHITextureViewBuildParams OpaqueVisibilityUAVBuildParams;
            OpaqueVisibilityUAVBuildParams.Context = Context.Weak();
            OpaqueVisibilityUAVBuildParams.Resource = Result.Resource;
            OpaqueVisibilityUAVBuildParams.Access = F_RHIResourceAccess::MakeUAV();
            OpaqueVisibilityUAVBuildParams.TextureViewAspect.UAVClearable = true;
            Result.UAV = RACreateAndBuildShared<A_RHIResourceView>(OpaqueVisibilityUAVBuildParams);
            return ABYTEK_MOVE(Result);
        }
        void F_OpaqueInstance::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
        {
            UAV = {};
            SRV = {};
            Resource = {};
        }
        void F_OpaqueInstance::Clear(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const F_Name& DebugName) const
        {
            H_RHISubmissionUtilities::ClearUAVUInt(
                SubmissionItemContainer,
                UAV,
                F_Vector4_U64(0),
                DebugName
            );
        }
        void F_OpaqueInstance::Bind(const TS<A_RHIBindGroup>& BindGroup, const F_Name& Name, B8 EnableWrite) const
        {
            if (EnableWrite)
            {
                BindGroup->BindResourceView(GetBindGroupSlotName(Name), UAV);
            }
            else
            {
                BindGroup->BindResourceView(GetBindGroupSlotName(Name), SRV);
            }
        }
        
#ifdef ABYTEK_DEBUG_INFO
        void F_OpaqueInstance::SetDebugName(const F_Name& DebugName)
        {
            ABYTEK_ENGINE_SRP_ASSERT(IsValid()) << "Cannot set debug name on invalid opaque instance";
            Resource->SetDebugName(DebugName);
            SRV->SetDebugName(*DebugName + ABYTEK_TEXT(".SRV"));
            UAV->SetDebugName(*DebugName + ABYTEK_TEXT(".UAV"));
        }
#endif
    }
}