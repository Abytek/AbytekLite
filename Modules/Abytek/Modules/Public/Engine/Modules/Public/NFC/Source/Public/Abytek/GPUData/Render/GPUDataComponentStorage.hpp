#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/RenderBase/RenderObject.hpp"
#include "Abytek/GPUData/GPUDataCommon.hpp"


namespace Abytek
{
    class F_GPUDataComponentPage;
    class F_GPUDataComponentType;

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
        
        struct ABYTEK_ENGINE_NFC_API F_GlobalSRVBinding : TF_GlobalRenderBinding<F_GlobalSRVBinding>
        {
            ABYTEK_DECLARE_GLOBAL_RENDER_BINDING(F_GlobalSRVBinding);
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                if (
                    SupportMultiplePages(Config.Database->GetFeatureSupports())
                )
                {
                    Config.AllowBindless = true;
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeResourceViewSet(
                            ABYTEK_NAME("GPUDataPages"),
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
        struct ABYTEK_ENGINE_NFC_API F_GlobalUAVBinding : TF_GlobalRenderBinding<F_GlobalUAVBinding>
        {
            ABYTEK_DECLARE_GLOBAL_RENDER_BINDING(F_GlobalUAVBinding);
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                if (
                    SupportMultiplePages(Config.Database->GetFeatureSupports())
                )
                {
                    Config.AllowBindless = true;
                    Config.Slots.push_back(
                        F_RHIBindGroupTemplateSlot::MakeResourceViewSet(
                            ABYTEK_NAME("GPUDataPages"),
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
        
        inline F_FeedbackStatus SetupCompileParams(
            F_RHIPipelineStateTemplateCompileParams& CompileParams,
            B8 EnableWritePages = false
        )
        {
            if (EnableWritePages)
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    CompileParams.AddShaderDefinition(
                        ABYTEK_NAME("ABYTEK_RENDER_GEOMETRY_ENABLE_WRITE_PAGES")
                    )
                );
            }
            if (SupportMultiplePages(CompileParams.Database->GetFeatureSupports()))
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    CompileParams.AddShaderDefinition(
                        ABYTEK_NAME("ABYTEK_RENDER_GEOMETRY_ENABLE_MULTIPLE_PAGES")
                    )
                );
            }
            return F_FeedbackStatus::MakeSucceeded();
        }
    }
    
    struct F_GPUDataComponentStorageBuildParams
    {
        TW<F_GPUDataComponentType> ComponentType;
    };
    
    class ABYTEK_ENGINE_NFC_API F_GPUDataComponentStorage : public A_RenderObject
    {
    private:
        TW<F_GPUDataComponentType> _ComponentType;
        U64 _CapacityLimitInBytes = 0;
        U64 _MinimalPageSizeInBytes = 0;
        U64 _NextPageSizeInBytes = 0;
        U64 _CapacityInBytes = 0;
        U32 _StrideInBytes = 0;
        TF_Vector<TS<F_GPUDataComponentPage>> _Pages;
        B8 _ShouldRecreateBindGroup = false;
        TS<A_RHIBindGroup> _GlobalSRVBindGroup;
        TS<A_RHIBindGroup> _GlobalUAVBindGroup;
        F_YieldReentrantCriticalSection _CriticalSection;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetComponentType() const noexcept
        {
            return _ComponentType;
        }
        ABYTEK_FORCE_INLINE auto GetCapacityLimitInBytes() const noexcept
        {
            return _CapacityLimitInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetMinimalPageSizeInBytes() const noexcept
        {
            return _MinimalPageSizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetNextPageSizeInBytes() const noexcept
        {
            return _NextPageSizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetCapacityInBytes() const noexcept
        {
            return _CapacityInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetStrideInBytes() const noexcept
        {
            return _StrideInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetPages() const noexcept
        {
            return _Pages;
        }
        ABYTEK_FORCE_INLINE const auto& GetGlobalSRVBindGroup() const noexcept
        {
            return _GlobalSRVBindGroup;
        }
        ABYTEK_FORCE_INLINE const auto& GetGlobalUAVBindGroup() const noexcept
        {
            return _GlobalUAVBindGroup;
        }
        
    public:
        F_GPUDataComponentStorage(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_GPUDataComponentStorageBuildParams& BuildParams);
        ~F_GPUDataComponentStorage() override;
         
    public:
        void BeginUpdate();
        void EndUpdate();
        void BeginPostUpdate();
        void EndPostUpdate();
        
    public:
        TF_ConcurrentQueue<F_GPUDataComponentAllocation> DeallocationQueue;
        void FlushDeallocationQueue();
        
    public:
        TF_Optional<F_GPUDataComponentAllocation> Allocate(U32 NumComponents);
        TS<F_GPUDataComponentPage> AddNewPage(U64 SizeInBytes);
        
    public:
        void Update();
        
    public:
        TF_Optional<F_GPUDataComponentAllocation> AddComponent(U32 NumComponents = 1);
    };
}
