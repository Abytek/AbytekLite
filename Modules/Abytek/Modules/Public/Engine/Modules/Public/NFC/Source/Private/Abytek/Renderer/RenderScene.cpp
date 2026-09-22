#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/RenderCoreManager.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/World/WorldSubsystemContainer.hpp"
#include "Abytek/Renderer/RendererManager.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryStorage.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveManager.hpp"


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
        _GeometryStorage = F_RenderGeometryStorage::CreateAndInit(
            GetWorldRenderResource(),
            SubmissionItemContainer, 
            GeometryStorageBuildParams
        );
#ifdef ABYTEK_DEBUG_INFO
        _GeometryStorage->SetDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".GeometryStorage")
        );
#endif
        
        {
            F_RenderPrimitiveManagerBuildParams PrimitiveManagerBuildParams;
            PrimitiveManagerBuildParams.Scene = ABYTEK_WTHIS();
            _PrimitiveManager = F_RenderPrimitiveManager::CreateAndInit(
                GetWorldRenderResource(),
                SubmissionItemContainer, 
                PrimitiveManagerBuildParams
            );
#ifdef ABYTEK_DEBUG_INFO
            _PrimitiveManager->SetDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".PrimitiveManager")
            );
#endif
        }
    }
    void A_RenderScene::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
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
        _GeometryStorage->BeginPostUpdate(SubmissionItemContainer);
        _PrimitiveManager->BeginPostUpdate(SubmissionItemContainer);
    }
    void A_RenderScene::OnEndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _PrimitiveManager->EndPostUpdate(SubmissionItemContainer);
        _GeometryStorage->EndPostUpdate(SubmissionItemContainer);
    }
}
