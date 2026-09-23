#pragma once

#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/GPUData/GPUDataCommon.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/RenderRegistryRuntime.hpp"
#include "Abytek/RHIPipelineStateTemplate.hpp"
#include "Abytek/RHIBindGroupTemplate.hpp"


namespace Abytek
{
    class F_GPUData;
    class F_GPUDataInstanceSet;
    
    enum class E_GPUDataComponentTypeClass : U8
    {
        NONE,
        PER_INSTANCE,
        PER_INSTANCE_SET,
        DEFAULT = PER_INSTANCE
    };

    struct F_GPUDataComponentTypeConfig
    {
        F_Name Name;
        F_GlobalRenderBinding SRVBinding;
        F_GlobalRenderBinding UAVBinding;
        U32 SizeInBytes = 0;
        U32 AlignmentInBytes = 0;
        E_GPUDataComponentTypeClass Class = E_GPUDataComponentTypeClass::DEFAULT;
        
        template<typename __F_GPUData, typename __F_GPUDataComponentType, E_GPUDataComponentTypeClass __Class = E_GPUDataComponentTypeClass::DEFAULT>
        static F_GPUDataComponentTypeConfig Make(
            const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime, 
            const F_Name InName = __F_GPUDataComponentType::GetStaticName(),
            E_GPUDataComponentTypeClass InClass = __Class
        );
    };
    struct F_GPUDataComponentTypeBuildParams : F_GPUDataComponentTypeConfig
    {
        TW<F_GPUData> GPUData;
    };
    class ABYTEK_ENGINE_NFC_API F_GPUDataComponentType final : public A_RenderObject
    {
    public:
        static F_Name GetBindGroupSlotName(
            const F_Name& DataName, 
            const F_Name& ComponentTypeName, 
            const F_RHIFeatureSupports& FeatureSupport
        )
        {
            if (GPUData::SupportMultiplePages(FeatureSupport))
            {
                return ABYTEK_TEXT("___Abytek_GPUDataPages___DATA___") + *DataName + ABYTEK_TEXT("___COMPONENT_TYPE___") + *ComponentTypeName;
            }
            return ABYTEK_TEXT("___Abytek_GPUDataPageSingle___DATA___") + *DataName + ABYTEK_TEXT("___COMPONENT_TYPE___") + *ComponentTypeName;
        }
        
    private:
        TW<F_GPUData> _GPUData;
        F_Name _Name;
        F_GlobalRenderBinding _SRVBinding;
        F_GlobalRenderBinding _UAVBinding;
        U32 _SizeInBytes = 0;
        U32 _AlignmentInBytes = 0;
        E_GPUDataComponentTypeClass _Class = E_GPUDataComponentTypeClass::NONE;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetGPUData() const noexcept
        {
            return _GPUData;
        }
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            return _Name;
        }
        ABYTEK_FORCE_INLINE const auto& GetSRVBinding() const noexcept
        {
            return _SRVBinding;
        }
        ABYTEK_FORCE_INLINE const auto& GetUAVBinding() const noexcept
        {
            return _UAVBinding;
        }
        ABYTEK_FORCE_INLINE auto GetSizeInBytes() const noexcept
        {
            return _SizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetAlignmentInBytes() const noexcept
        {
            return _AlignmentInBytes;
        } 
        ABYTEK_FORCE_INLINE auto GetClass() const noexcept
        {
            return _Class;
        }
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_GPUDataComponentType, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_GPUDataComponentTypeBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
    
    namespace GPUData
    {
        template<typename __F_GPUData, typename __F_GPUDataComponentType>
        struct TF_SRVBinding : F_GlobalRenderBinding
        {
            static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
            ABYTEK_GLOBAL_RENDER_BINDING(
                TF_SRVBinding, 
                ABYTEK_TEXT("Abytek::GPUData::TF_SRVBinding<")
                + *__F_GPUData::GetStaticName() 
                + ABYTEK_TEXT(", ")
                + *__F_GPUDataComponentType::GetStaticName() 
                + ABYTEK_TEXT(">")
            );
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                auto SlotName = F_GPUDataComponentType::GetBindGroupSlotName(
                    __F_GPUData::GetStaticName(), 
                    __F_GPUDataComponentType::GetStaticName(), 
                    Config.Database->GetFeatureSupports()
                );
                if (
                    SupportMultiplePages(Config.Database->GetFeatureSupports())
                )
                {
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeResourceViewSet(
                            SlotName,
                            ~U32(0),
                            F_RHIResourceAccess::MakeSRV()
                        ) 
                    );
                }
                else
                {
                    return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Single page for gpu data system is currently not supported"));
                }
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        template<typename __F_GPUData, typename __F_GPUDataComponentType>
        struct TF_UAVBinding : F_GlobalRenderBinding
        {
            static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
            ABYTEK_GLOBAL_RENDER_BINDING(
                TF_UAVBinding, 
                ABYTEK_TEXT("Abytek::GPUData::TF_UAVBinding<")
                + *__F_GPUData::GetStaticName() 
                + ABYTEK_TEXT(", ")
                + *__F_GPUDataComponentType::GetStaticName() 
                + ABYTEK_TEXT(">")
            );
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                auto SlotName = F_GPUDataComponentType::GetBindGroupSlotName(
                    __F_GPUData::GetStaticName(), 
                    __F_GPUDataComponentType::GetStaticName(), 
                    Config.Database->GetFeatureSupports()
                );
                if (
                    SupportMultiplePages(Config.Database->GetFeatureSupports())
                )
                {
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeResourceViewSet(
                            SlotName,
                            ~U32(0),
                            F_RHIResourceAccess::MakeUAV()
                        ) 
                    );
                }
                else
                {
                    return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Single page for gpu data system is currently not supported"));
                }
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        
        namespace Internal
        {
            template<typename __F_GPUData, typename __F_GPUDataComponentType>
            static F_FeedbackStatus AddBindGroupToPipelineStateTemplate(
                F_RHIPipelineStateTemplateCompileParams& PipelineStateTemplateCompileParams,
                const F_RHIResourceAccess Access
            )
            {
                if (
                    auto Status = PipelineStateTemplateCompileParams.AddShaderDefinition(
                        ABYTEK_TEXT("ABYTEK_GPU_DATA_COMPONENT_TYPE_SIZE_IN_BYTES_") + *__F_GPUDataComponentType::GetStaticName(),
                        ToText(sizeof(__F_GPUDataComponentType))
                    );
                    !Status
                )
                {
                    return Status;
                }
                if (
                    auto Status = PipelineStateTemplateCompileParams.AddShaderDefinition(
                        ABYTEK_TEXT("ABYTEK_GPU_DATA_COMPONENT_TYPE_ALIGNMENT_IN_BYTES_") + *__F_GPUDataComponentType::GetStaticName(),
                        ToText(ABYTEK_ALIGNOF(__F_GPUDataComponentType))
                    );
                    !Status
                )
                {
                    return Status;
                }
                if (FlagHas(Access.GPU, E_RHIResourceGPUAccess::SRV))
                {
                    PipelineStateTemplateCompileParams.BindGroups.push_back(
                        F_RHIPipelineStateTemplateBindGroup::Make(
                            TF_SRVBinding<__F_GPUData, __F_GPUDataComponentType>::GetTemplateHashCode()
                        )
                    );
                    return F_FeedbackStatus::MakeSucceeded();
                }
                if (FlagHas(Access.GPU, E_RHIResourceGPUAccess::UAV))
                {
                    PipelineStateTemplateCompileParams.BindGroups.push_back(
                        F_RHIPipelineStateTemplateBindGroup::Make(
                            TF_UAVBinding<__F_GPUData, __F_GPUDataComponentType>::GetTemplateHashCode()
                        )
                    );
                    return F_FeedbackStatus::MakeSucceeded();
                }
                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid access"));
            }
        }
    }
    
    template<typename __F_GPUData, typename __F_GPUDataComponentType, E_GPUDataComponentTypeClass __Class = E_GPUDataComponentTypeClass::DEFAULT>
    F_GPUDataComponentTypeConfig F_GPUDataComponentTypeConfig::Make(
        const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime, 
        const F_Name InName,
        E_GPUDataComponentTypeClass InClass
    )
    {
        F_GPUDataComponentTypeConfig Result;
        Result.Name = InName;
        Result.SRVBinding = static_cast<F_GlobalRenderBinding>(
            GPUData::TF_SRVBinding<__F_GPUData, __F_GPUDataComponentType>::Instantiate(RenderRegistryRuntime)
        );
        Result.UAVBinding = static_cast<F_GlobalRenderBinding>(
            GPUData::TF_UAVBinding<__F_GPUData, __F_GPUDataComponentType>::Instantiate(RenderRegistryRuntime)
        );
        Result.SizeInBytes = sizeof(__F_GPUDataComponentType);
        Result.AlignmentInBytes = ABYTEK_ALIGNOF(__F_GPUDataComponentType);
        Result.Class = InClass;
        return Result;
    }
    
    namespace Internal
    {
        template<typename __F_GPUDataComponentType, typename = void>
        struct TH_GPUDataComponentType_GetStaticName
        {
            static E_GPUDataComponentTypeClass Invoke()
            {
                return E_GPUDataComponentTypeClass::DEFAULT;
            }
        };
        template<typename __F_GPUDataComponentType>
        struct TH_GPUDataComponentType_GetStaticName<
            __F_GPUDataComponentType,
            std::void_t<decltype(__F_GPUDataComponentType::___Abytek_GetManualStaticClass())>
        >
        {
            static E_GPUDataComponentTypeClass Invoke()
            {
                return __F_GPUDataComponentType::___Abytek_GetManualStaticClass();
            }
        };
    }
}

#define ABYTEK_GPU_DATA_COMPONENT_TYPE_CLASS(...) \
            static Abytek::E_GPUDataComponentTypeClass ___Abytek_GetManualStaticClass() \
            { \
                return __VA_ARGS__; \
            }
#define ABYTEK_GPU_DATA_COMPONENT_TYPE(Name, StaticName, Canonical, ...) \
             \
            static Abytek::F_Name GetStaticName() { return StaticName; } \
            static Abytek::E_GPUDataComponentTypeClass GetStaticClass() { return Abytek::Internal::TH_GPUDataComponentType_GetStaticName<Name>::Invoke(); } \
             \
            template<typename __F_GPUData> \
            static Abytek::F_FeedbackStatus AddBindGroupToPipelineStateTemplate( \
                Abytek::F_RHIPipelineStateTemplateCompileParams& PipelineStateTemplateCompileParams, \
                const Abytek::F_RHIResourceAccess Access = Abytek::F_RHIResourceAccess::MakeSRV() \
            ) \
            { \
                return Abytek::GPUData::Internal::AddBindGroupToPipelineStateTemplate<__F_GPUData, Name>( \
                    PipelineStateTemplateCompileParams, \
                    Access \
                ); \
            } \
             \
            ABYTEK_BEGIN_REFLECTOR() \
            ABYTEK_END_REFLECTOR(Name) \
            { \
                ABYTEK_REFLECT_CANONICAL(Canonical); \
            }