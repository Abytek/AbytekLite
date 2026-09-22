#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/Assets/SimpleMeshData.hpp"
#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryCommon.hpp"


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
        
        struct ABYTEK_ENGINE_NFC_API F_GlobalSRVBinding : F_GlobalRenderBinding
        {
            ABYTEK_GLOBAL_RENDER_BINDING(F_GlobalSRVBinding, ABYTEK_NAME("Abytek::RenderGeometry::F_GlobalSRVBinding"));
            
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
        struct ABYTEK_ENGINE_NFC_API F_GlobalUAVBinding : F_GlobalRenderBinding
        {
            ABYTEK_GLOBAL_RENDER_BINDING(F_GlobalUAVBinding, ABYTEK_NAME("Abytek::RenderGeometry::F_GlobalUAVBinding"));
            
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
    
    struct F_RenderGeometryStorageBuildParams
    {
        TW<A_RenderScene> Scene;
    };
    
    class ABYTEK_ENGINE_NFC_API F_RenderGeometryStorage : public A_RenderObject
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
        ABYTEK_RENDER_OBJECT_CREATABLE(F_RenderGeometryStorage, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RenderGeometryStorageBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void BeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    private:
        TF_ConcurrentQueue<F_RenderGeometryAllocation> _DeallocationQueue;
        void _FlushDeallocationQueue(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        TF_Optional<F_RenderGeometryAllocation> Allocate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, U64 SizeInBytes, U64 AlignmentInBytes = sizeof(F_Vector4_F32));
        TS<F_RenderGeometryPage> AddNewPage(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, U64 SizeInBytes);
        
    public:
        B8 AddMeshData_Simple(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
            const F_SimpleMeshDataROView& MeshDataView,
            F_RenderGeometryAllocation& OutGeometryAllocation,
            F_RenderGeometryAllocationStructure_Simple& OutGeometryAllocationStructure
        );
        void RemoveMeshData_Simple(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
            const F_RenderGeometryAllocation& GeometryAllocation
        );
        
    private:
        void _RecreateBindGroupIfNeeded(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
}
