#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/Geometries/SimpleMeshData.hpp"
#include "Abytek/RenderBase/RenderObject.hpp"
#include "Abytek/Geometries/Render/RenderGeometryCommon.hpp"


namespace Abytek
{
    class A_RenderScene;
    class A_RenderGeometryPool;

    namespace RenderGeometry
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
                            ABYTEK_NAME("RenderGeometryPages"),
                            ~U32(0),
                            F_RHIResourceAccess::MakeSRV()
                        ) 
                    );
                }
                else
                {
                    return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Single page for render geometry system is currently not supported"));
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
                            ABYTEK_NAME("RenderGeometryPages"),
                            ~U32(0),
                            F_RHIResourceAccess::MakeUAV()
                        ) 
                    );
                }
                else
                {
                    return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Single page for render geometry system is currently not supported"));
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
    
    struct F_RenderGeometryManagerBuildParams
    {
        TW<A_RenderScene> Scene;
    };
    
    class ABYTEK_ENGINE_NFC_API F_RenderGeometryManager : public A_RenderObject
    {
    private:
        TW<A_RenderScene> _Scene;
        U64 _CapacityLimitInBytes = 0;
        U64 _MinimalPageSizeInBytes = 0;
        U64 _NextPageSizeInBytes = 0;
        U64 _CapacityInBytes = 0;
        TF_Vector<TS<F_RenderGeometryPage>> _Pages;
        B8 _ShouldRecreateBindGroup = false;
        TS<A_RHIBindGroup> _GlobalSRVBindGroup;
        TS<A_RHIBindGroup> _GlobalUAVBindGroup;
        F_YieldReentrantCriticalSection _CriticalSection;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        ABYTEK_FORCE_INLINE const auto& GetCapacityLimitInBytes() const noexcept
        {
            return _CapacityLimitInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetMinimalPageSizeInBytes() const noexcept
        {
            return _MinimalPageSizeInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetNextPageSizeInBytes() const noexcept
        {
            return _NextPageSizeInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetCapacityInBytes() const noexcept
        {
            return _CapacityInBytes;
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
        F_RenderGeometryManager(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderGeometryManagerBuildParams& BuildParams);
        ~F_RenderGeometryManager() override;
        
    public:
        void BeginUpdate();
        void EndUpdate();
        void BeginPostUpdate();
        void EndPostUpdate();
        
    public:
        TF_ConcurrentQueue<F_RenderGeometryAllocation> DeallocationQueue;
        void FlushDeallocationQueue();
        
    public:
        TF_Optional<F_RenderGeometryAllocation> Allocate(U64 SizeInBytes, U64 AlignmentInBytes = sizeof(F_Vector4_F32));
        TS<F_RenderGeometryPage> AddNewPage(U64 SizeInBytes);
        
    public:
        B8 AddMeshData_Simple(
            const F_SimpleMeshDataROView& MeshDataView,
            F_RenderGeometryAllocation& OutGeometryAllocation,
            F_RenderGeometryAllocationStructure_Simple& OutGeometryAllocationStructure
        );
        
    private:
        void _RecreateBindGroupIfNeeded();
    };
}
