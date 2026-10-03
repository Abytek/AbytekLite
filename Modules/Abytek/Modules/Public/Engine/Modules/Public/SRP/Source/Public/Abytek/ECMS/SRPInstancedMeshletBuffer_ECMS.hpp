#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/RHIBufferInlineAllocator.hpp"
#include "Abytek/Renderer/GPUData/GPUDataCommon.hpp"


namespace Abytek
{
    class A_RenderView;
    class A_RHIResource;
    
    namespace SRP::ECMS
    {
        struct ABYTEK_ALIGN(16) F_InstancedMeshlet
        {
            F_GPUDataInstanceAddress PrimitiveAddress;
            U32 MeshletIndex = ~U32(0);
        };
        
        struct ABYTEK_ENGINE_SRP_API F_InstancedMeshletBuffer
        {
            static F_Name GetBindGroupSlotName_Buffer(const F_Name& Name)
            {
                return ABYTEK_TEXT("___Abytek_SRP_ECMS_InstancedMeshletBuffer_") + *Name;
            }
            static F_Name GetBindGroupSlotName_WriteOffsetBuffer(const F_Name& Name)
            {
                return ABYTEK_TEXT("___Abytek_SRP_ECMS_InstancedMeshletWriteOffsetBuffer_") + *Name;
            }
            
            U32 Capacity = 0;
            TS<A_RHIResource> Resource;
            TF_RHIBufferInlineAllocation<U32> WriteOffsetAllocation;
            
            ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
            {
                return Resource && WriteOffsetAllocation;
            }
            ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
            {
                return IsValid();
            }
            
            static F_InstancedMeshletBuffer Create(
                const TS<A_RHISubmissionItemContainer>& SubmissionItenContainer, 
                const TW_Valid<A_RenderView>& View,
                U32 InCapacity,
                E_RHIResourceAdditionalFlag InResourceAdditionalFlags = E_RHIResourceAdditionalFlag::DEFAULT
            );
            void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
            
#ifdef ABYTEK_DEBUG_INFO
            void SetDebugName(const F_Name& DebugName);
#endif
            
            void Bind(const TS<A_RHIBindGroup>& BindGroup, const F_Name& Name, B8 EnableWrite = false) const;
            
            static F_FeedbackStatus AddToBindGroup(F_RHIBindGroupTemplateCompileParams& CompileParams, const F_Name& Name, B8 EnableWrite = false)
            {
                if (EnableWrite)
                {
                    CompileParams.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeResourceView(
                            GetBindGroupSlotName_Buffer(Name),
                            F_RHIResourceAccess::MakeUAV()
                        )  
                    );
                    CompileParams.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeResourceView(
                            GetBindGroupSlotName_WriteOffsetBuffer(Name),
                            F_RHIResourceAccess::MakeUAV()
                        )  
                    );
                }
                else
                {
                    CompileParams.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeResourceView(
                            GetBindGroupSlotName_Buffer(Name),
                            F_RHIResourceAccess::MakeSRV()
                        )  
                    );
                }
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
    }
}
