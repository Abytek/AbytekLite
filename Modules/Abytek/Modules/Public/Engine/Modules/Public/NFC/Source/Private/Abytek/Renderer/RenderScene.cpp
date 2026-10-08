#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/RenderCoreManager.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/World/WorldSubsystemContainer.hpp"
#include "Abytek/Renderer/RendererManager.hpp"
#include "Abytek/Renderer/RenderObjectFactory.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryStorage.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveManager.hpp"
#include "Abytek/Renderer/VertexFactory/VertexFactoryTypeManager.hpp"
#include "Abytek/Renderer/RenderPrimitive/Archetypes/Processor_Simple.hpp"


namespace Abytek
{
    void A_RenderScene::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderSceneBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        F_RenderGeometryStorageBuildParams GeometryStorageBuildParams;
        GeometryStorageBuildParams.Scene = ABYTEK_WTHIS();
#ifdef ABYTEK_DEBUG_INFO
        _GeometryStorage = F_RenderGeometryStorage::CreateAndInit_WithDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".GeometryStorage"),
#else
        _GeometryStorage = F_RenderGeometryStorage::CreateAndInit(
#endif
            GetWorldRenderResource(),
            SubmissionItemContainer, 
            GeometryStorageBuildParams
        );
        
        {
            F_RenderPrimitiveManagerBuildParams PrimitiveManagerBuildParams;
            PrimitiveManagerBuildParams.Scene = ABYTEK_WTHIS();
#ifdef ABYTEK_DEBUG_INFO
            _PrimitiveManager = F_RenderPrimitiveManager::CreateAndInit_WithDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".PrimitiveManager"),
#else
            _PrimitiveManager = F_RenderPrimitiveManager::CreateAndInit(
#endif
                GetWorldRenderResource(),
                SubmissionItemContainer, 
                PrimitiveManagerBuildParams
            );
        }
        
        {
            F_VertexFactoryTypeManagerBuildParams VertexFactoryTypeManagerBuildParams;
            VertexFactoryTypeManagerBuildParams.Scene = ABYTEK_WTHIS();
#ifdef ABYTEK_DEBUG_INFO
            _VertexFactoryTypeManager = F_VertexFactoryTypeManager::CreateAndInit_WithDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".VertexFactoryTypeManager"),
#else
            _VertexFactoryTypeManager = F_VertexFactoryTypeManager::CreateAndInit(
#endif
                GetWorldRenderResource(),
                SubmissionItemContainer, 
                VertexFactoryTypeManagerBuildParams
            );
        }
        
        {
            if (auto Processor = GetRenderObjectFactory()->CreatePrimitiveProcessor_Simple())
            {
#ifdef ABYTEK_DEBUG_INFO
                Processor->SetDebugName(
                    *GetDebugName()
                    + ABYTEK_TEXT(".PrimitiveProcessor_Simple")
                );
#endif
                _PrimitiveProcessor_Simple = _PrimitiveManager->AddProcessor(
                    SubmissionItemContainer,
                    Processor
                );
            }
        }
    }
    void A_RenderScene::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _VertexFactoryTypeManager->Release(SubmissionItemContainer);
        _VertexFactoryTypeManager = {};
        
        _PrimitiveManager->Release(SubmissionItemContainer);
        _PrimitiveManager = {};
        
        _GeometryStorage->Release(SubmissionItemContainer);
        _GeometryStorage = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void A_RenderScene::OnBeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _GeometryStorage->BeginUpdate(SubmissionItemContainer);
        _PrimitiveManager->BeginUpdate(SubmissionItemContainer);
    }
    void A_RenderScene::OnEndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _PrimitiveManager->EndUpdate(SubmissionItemContainer);
        _GeometryStorage->EndUpdate(SubmissionItemContainer);
    }
    void A_RenderScene::OnBeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void A_RenderScene::OnEndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _PrimitiveManager->FinalizeFrame(SubmissionItemContainer);
        _GeometryStorage->FinalizeFrame(SubmissionItemContainer);
    }
}
