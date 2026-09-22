#pragma once

#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/GPUData/GPUDataCommon.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/RenderRegistryRuntime.hpp"


namespace Abytek
{
    class F_GPUData;
    class F_GPUDataInstanceSet;

    struct F_GPUDataComponentTypeConfig
    {
        F_Name Name;
        F_GlobalRenderBinding SRVBinding;
        F_GlobalRenderBinding UAVBinding;
        U32 SizeInBytes = 0;
        U32 AlignmentInBytes = 0;
        
        template<typename __F>
        static F_GPUDataComponentTypeConfig Make(
            const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime, 
            const F_Name InName = __F::GetStaticName()
        )
        {
            F_GPUDataComponentTypeConfig Result;
            Result.Name = InName;
            Result.SRVBinding = static_cast<const F_GlobalRenderBinding&>(__F::F_SRVBinding::Instantiate(RenderRegistryRuntime));
            Result.UAVBinding = static_cast<const F_GlobalRenderBinding&>(__F::F_UAVBinding::Instantiate(RenderRegistryRuntime));
            Result.SizeInBytes = sizeof(__F);
            Result.AlignmentInBytes = ABYTEK_ALIGNOF(__F);
            return Result;
        }
    };
    struct F_GPUDataComponentTypeBuildParams : F_GPUDataComponentTypeConfig
    {
        TW<F_GPUData> GPUData;
    };
    class ABYTEK_ENGINE_NFC_API F_GPUDataComponentType final : public A_RenderObject
    {
    public:
        static F_Name GetBindGroupSlotName(const F_Name& ComponentName, const F_RHIFeatureSupports& FeatureSupport)
        {
            if (GPUData::SupportMultiplePages(FeatureSupport))
            {
                return ABYTEK_TEXT("___Abytek_GPUDataPages_") + *ComponentName;
            }
            return ABYTEK_TEXT("___Abytek_GPUDataPageSingle_") + *ComponentName;
        }
        
    private:
        TW<F_GPUData> _GPUData;
        F_Name _Name;
        F_GlobalRenderBinding _SRVBinding;
        F_GlobalRenderBinding _UAVBinding;
        U32 _SizeInBytes = 0;
        U32 _AlignmentInBytes = 0;
        
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
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_GPUDataComponentType, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_GPUDataComponentTypeBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}

#define ABYTEK_GPU_DATA_COMPONENT_TYPE(Name, StaticName, ...) \
            static Abytek::F_Name GetStaticName() { return StaticName; } \
            struct F_SRVBinding : Abytek::F_GlobalRenderBinding \
            { \
                static constexpr Abytek::E_ReflectMode DefaultReflectMode = Abytek::E_ReflectMode::INLINE; \
                ABYTEK_GLOBAL_RENDER_BINDING(F_SRVBinding, *GetStaticName() + ABYTEK_TEXT("::F_SRVBinding")); \
                 \
                static Abytek::F_FeedbackStatus Build(F_Config& Config) \
                { \
                    auto SlotName = Abytek::F_GPUDataComponentType::GetBindGroupSlotName(GetStaticName(), Config.Database->GetFeatureSupports()); \
                    if ( \
                        Abytek::GPUData::SupportMultiplePages(Config.Database->GetFeatureSupports()) \
                    ) \
                    { \
                        Config.Slots.push_back( \
                            Abytek::F_RHIBindGroupTemplateSlot::MakeResourceViewSet( \
                                SlotName, \
                                ~U32(0), \
                                Abytek::F_RHIResourceAccess::MakeSRV() \
                            )  \
                        ); \
                    } \
                    else \
                    { \
                        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Single page for gpu data system is currently not supported")); \
                    } \
                    return Abytek::F_FeedbackStatus::MakeSucceeded(); \
                } \
            }; \
            struct F_UAVBinding : Abytek::F_GlobalRenderBinding \
            { \
                static constexpr Abytek::E_ReflectMode DefaultReflectMode = Abytek::E_ReflectMode::INLINE; \
                ABYTEK_GLOBAL_RENDER_BINDING(F_UAVBinding, *GetStaticName() + ABYTEK_TEXT("::F_UAVBinding")); \
                 \
                static Abytek::F_FeedbackStatus Build(F_Config& Config) \
                { \
                    auto SlotName = Abytek::F_GPUDataComponentType::GetBindGroupSlotName(GetStaticName(), Config.Database->GetFeatureSupports()); \
                    if ( \
                        Abytek::GPUData::SupportMultiplePages(Config.Database->GetFeatureSupports()) \
                    ) \
                    { \
                        Config.Slots.push_back( \
                            Abytek::F_RHIBindGroupTemplateSlot::MakeResourceViewSet( \
                                SlotName, \
                                ~U32(0), \
                                Abytek::F_RHIResourceAccess::MakeUAV() \
                            )  \
                        ); \
                    } \
                    else \
                    { \
                        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Single page for gpu data system is currently not supported")); \
                    } \
                    return Abytek::F_FeedbackStatus::MakeSucceeded(); \
                } \
            }; \
            ABYTEK_BEGIN_REFLECTOR() \
            ABYTEK_END_REFLECTOR(Name) \
            { \
                ABYTEK_REFLECT_CANONICAL(__VA_ARGS__); \
                ReflectionSession->ReferenceType( \
                    ReflectionType->ReflectReferenced<F_SRVBinding>() \
                ); \
                ReflectionSession->ReferenceType( \
                    ReflectionType->ReflectReferenced<F_UAVBinding>() \
                ); \
            }