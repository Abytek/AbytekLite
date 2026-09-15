#pragma once

#include "Abytek/RHIBindGroupTemplate.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


namespace Abytek
{
    struct F_DirectX12SharedRHIBindGroupTemplateCompiledData
    {
        struct F_SlotMap
        {
            // For root parameter
            U32 RootParameterIndex = ~U32(0);
            U32 DescriptorIndexFromRootParameterStart = ~U32(0);

            // For descriptor range
            U32 DescriptorRangeIndex = ~U32(0);
            U32 DescriptorIndexFromRangeStart = ~U32(0);

            //
            U32 NumDescriptors = 0;

            // For bindings
            U32 DescriptorBindingIndex = ~U32(0);
            U32 VertexBufferBindingIndex = ~U32(0);
            B8 HasIndexBufferBinding = false;
            U32 RTVBindingIndex = ~U32(0);
            B8 HasDSVBinding = false;
            U32 UniformDataBindingIndex = ~U32(0);
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_SlotMap& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RootParameterIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DescriptorIndexFromRootParameterStart);
                
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DescriptorRangeIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DescriptorIndexFromRangeStart);
                
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.NumDescriptors);
                
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DescriptorBindingIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.VertexBufferBindingIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.HasIndexBufferBinding);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RTVBindingIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.HasDSVBinding);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UniformDataBindingIndex);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_SlotMap& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RootParameterIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DescriptorIndexFromRootParameterStart);
                
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DescriptorRangeIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DescriptorIndexFromRangeStart);
                
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.NumDescriptors);
                
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DescriptorBindingIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.VertexBufferBindingIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.HasIndexBufferBinding);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RTVBindingIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.HasDSVBinding);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.UniformDataBindingIndex);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        
        struct F_DescriptorBinding
        {
            U32 RootParameterIndex = ~U32(0);
            U32 NumFixedDescriptors = 0;
            U32 DynamicSizeSlotIndex = ~U32(0);
            DirectX12SharedAPIWrapper::E_DescriptorHeapType HeapType = DirectX12SharedAPIWrapper::E_DescriptorHeapType::NONE;
            
            // For bindless
            U32 EncodedDataOffsetInBytes = ~U32(0);
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_DescriptorBinding& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RootParameterIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.NumFixedDescriptors);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DynamicSizeSlotIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.HeapType);
                
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.EncodedDataOffsetInBytes);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_DescriptorBinding& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RootParameterIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.NumFixedDescriptors);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DynamicSizeSlotIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.HeapType);
                
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.EncodedDataOffsetInBytes);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct F_VertexBufferBinding
        {
            U32 SlotIndex = ~U32(0);
            U32 InputElementIndex = ~U32(0);
            U32 NumInputElements = 0;
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_VertexBufferBinding& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SlotIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.InputElementIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.NumInputElements);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_VertexBufferBinding& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SlotIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.InputElementIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.NumInputElements);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct F_IndexBufferBinding
        {
            U32 SlotIndex = ~U32(0);
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_IndexBufferBinding& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SlotIndex);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_IndexBufferBinding& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SlotIndex);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct F_RTVBinding
        {
            U32 SlotIndex = ~U32(0);
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RTVBinding& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SlotIndex);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RTVBinding& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SlotIndex);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct F_DSVBinding
        {
            U32 SlotIndex = ~U32(0);
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_DSVBinding& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SlotIndex);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_DSVBinding& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SlotIndex);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct F_UniformDataBinding
        {
            U32 SlotIndex = ~U32(0);
            U32 RootParameterIndex = ~U32(0);
            U32 SizeInBytes = 0;
            B8 UseRootConstants = false;
            
            // For bindless
            U32 EncodedDataOffsetInBytes = ~U32(0);
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_UniformDataBinding& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SlotIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RootParameterIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SizeInBytes);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UseRootConstants);
                
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.EncodedDataOffsetInBytes);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_UniformDataBinding& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SlotIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RootParameterIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SizeInBytes);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.UseRootConstants);
                
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.EncodedDataOffsetInBytes);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };

        // For root signature creation
        TF_SmallVector<DirectX12SharedAPIWrapper::F_RootParameter, 4> RootParameters;
        TF_SmallVector<DirectX12SharedAPIWrapper::F_InputElementDesc, 4> InputElementDescs;
        B8 HasIndexBuffer = false;
        TF_SmallVector<E_RHIFormat, 8> RTVFormats;
        TF_Optional<E_RHIFormat> DSVFormat;

        // For slot-related mappings
        TF_SmallVector<F_SlotMap, 6> SlotMaps;

        // For bindings
        TF_SmallVector<F_DescriptorBinding, 4> DescriptorBindings;
        TF_SmallVector<F_VertexBufferBinding, 8> VertexBufferBindings;
        TF_Optional<F_IndexBufferBinding> IndexBufferBinding;
        TF_SmallVector<F_RTVBinding, 8> RTVBindings;
        TF_Optional<F_DSVBinding> DSVBinding;
        TF_SmallVector<F_UniformDataBinding, 2> UniformDataBindings;
        B8 UseConstantBuffer = false;
        
        // For bindless
        U32 EncodedDataSizeInBytes = ~U32(0);
        U32 EncodedDataAlignmentInBytes = ~U32(0);
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_DirectX12SharedRHIBindGroupTemplateCompiledData& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RootParameters);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.InputElementDescs);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.HasIndexBuffer);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RTVFormats);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DSVFormat);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SlotMaps);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DescriptorBindings);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.VertexBufferBindings);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.IndexBufferBinding);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RTVBindings);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DSVBinding);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UniformDataBindings);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UseConstantBuffer);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.EncodedDataSizeInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.EncodedDataAlignmentInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_DirectX12SharedRHIBindGroupTemplateCompiledData& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RootParameters);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.InputElementDescs);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.HasIndexBuffer);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RTVFormats);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DSVFormat);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SlotMaps);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DescriptorBindings);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.VertexBufferBindings);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.IndexBufferBinding);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RTVBindings);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DSVBinding);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.UniformDataBindings);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.UseConstantBuffer);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.EncodedDataSizeInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.EncodedDataAlignmentInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };

    class ABYTEK_ENGINE_RHI_API F_DirectX12SharedRHIBindGroupTemplate : public A_RHIBindGroupTemplate
    {
    private:
        F_DirectX12SharedRHIBindGroupTemplateCompiledData _CompiledData;

    public:
        ABYTEK_FORCE_INLINE const auto& GetCompiledData() const noexcept
        {
            return _CompiledData;
        }
        ABYTEK_FORCE_INLINE auto& InjectCompiledData() noexcept
        {
            return _CompiledData;
        }
        
    public:
        F_DirectX12SharedRHIBindGroupTemplate(
            const F_RHIBindGroupTemplateBuildParams& BuildParams,
            F_DirectX12SharedRHIBindGroupTemplateCompiledData&& CompiledData    
        );
        ABYTEK_FORCE_INLINE F_DirectX12SharedRHIBindGroupTemplate(
            const F_RHIBindGroupTemplateBuildParams& BuildParams,
            const F_DirectX12SharedRHIBindGroupTemplateCompiledData& CompiledData
        ) :
            F_DirectX12SharedRHIBindGroupTemplate(
                BuildParams,
                F_DirectX12SharedRHIBindGroupTemplateCompiledData(CompiledData)
            )
        {
        }
        ~F_DirectX12SharedRHIBindGroupTemplate() override;
        
    public:
        virtual U32 GetEncodedDataSizeInBytes() override;
        virtual U32 GetEncodedDataAlignmentInBytes() override;
    };
}