#include "Abytek/ECMS/SRPInstancedMeshletBuffer_ECMS.hpp"

#include "Abytek/Renderer/RenderView.hpp"


namespace Abytek
{
    namespace SRP::ECMS
    {
        F_InstancedMeshletBuffer F_InstancedMeshletBuffer::Create(
            const TS<A_RHISubmissionItemContainer>& SubmissionItenContainer, 
            const TW_Valid<A_RenderView>& View,
            U32 InCapacity,
            E_RHIResourceAdditionalFlag InResourceAdditionalFlags
        )
        {
            auto Context = H_RHI::GetMainContext();
            
            F_InstancedMeshletBuffer Result;
            Result.Capacity = InCapacity;
            
            F_RHIBufferBuildParams BufferBuildParams;
            BufferBuildParams.Context = Context.Weak();
            BufferBuildParams.BufferAspect.SizeInBytes = InCapacity * sizeof(F_InstancedMeshlet);
            BufferBuildParams.BufferAspect.StrideInBytes = sizeof(F_InstancedMeshlet);
            BufferBuildParams.AccessCapabilities = (
                F_RHIResourceAccess::MakeUAVCapabilities() 
                | F_RHIResourceAccess::MakeSRVCapabilities()
            );
            BufferBuildParams.AdditionalFlags = InResourceAdditionalFlags;
            Result.Resource = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
            
            Result.WriteOffsetAllocation = View->GetUAVDataAllocator()->Allocate<U32>(2);
            U32 WriteOffsetValues[] { 0, InCapacity };
            Result.WriteOffsetAllocation.Upload(SubmissionItenContainer, WriteOffsetValues);
            return Result;
        }
        void F_InstancedMeshletBuffer::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
        {
            Resource = {};
        }

#ifdef ABYTEK_DEBUG_INFO
        void F_InstancedMeshletBuffer::SetDebugName(const F_Name& DebugName)
        {
            Resource->SetDebugName(DebugName);
        }
#endif

        void F_InstancedMeshletBuffer::Bind(const TS<A_RHIBindGroup>& BindGroup, const F_Name& Name, B8 EnableWrite) const
        {
            if (EnableWrite)
            {
                BindGroup->BindResourceView(GetBindGroupSlotName_Buffer(Name), Resource);
                WriteOffsetAllocation.Bind(BindGroup, GetBindGroupSlotName_WriteOffsetBuffer(Name), F_RHIResourceAccess::MakeUAV());
            }
            else
            {
                BindGroup->BindResourceView(GetBindGroupSlotName_Buffer(Name), Resource);
            }
        }
    }
}
